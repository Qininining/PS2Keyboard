#include<Arduino.h>
#include <PS2X_lib.h>  //for v1.6
#include <BleKeyboard.h>

/******************************************************************
 * set pins connected to PS2 controller:  *设置连接到PS2控制器的引脚：
 *   - 1e column: original
 *   - 2e colmun: Stef?
 * replace pin numbers by the ones you use  *用您使用的密码替换密码
 ******************************************************************/

//  ESP32 pin
// https://github.com/espressif/arduino-esp32/blob/master/docs/esp32_pinmap.png

// #define PS2_DAT        19  //MISO
// #define PS2_CMD        23  //MOSI
// #define PS2_SEL         5  //SS
// #define PS2_CLK        18  //SLK

#define PS2_DAT        19  //MISO
#define PS2_CMD        23  //MOSI
#define PS2_SEL         5  //SS
#define PS2_CLK        18  //SLK

/******************************************************************
 * select modes of PS2 controller:  *选择PS2控制器的模式：
 *   - pressures = analog reading of push-butttons
 *   - rumble    = motor rumbling
 * uncomment 1 of the lines for each mode selection
 ******************************************************************/
#define pressures   false
#define rumble      false

BleKeyboard bleKeyboard;
PS2X ps2x; // create PS2 Controller Class   //创建PS2控制器类

//right now, the library does NOT support hot pluggable controllers, meaning  //目前，该库不支持热插拔控制器，这意味着
//you must always either restart your Arduino after you connect the controller,  //您必须始终在连接控制器后重新启动Arduino，
//or call config_gamepad(pins) again after connecting the controller.  //或在连接控制器后再次调用config_gamepad（pins）。

int error = -1;
byte type = 0;
byte vibrate = 0;
int tryNum = 1;
int state = LOW;
int t = 0;

int LX_ = 0, LY_ = 0, RX_ = 0, RY_ = 0;

void setup(){

  Serial.begin(115200);
  pinMode(2, OUTPUT); // 设置引脚为输出模式

  while (error != 0) {
    delay(1000);// 1 second wait
    //setup pins and settings: GamePad(clock, command, attention, data, Pressures?, Rumble?) check for error  //设置引脚和设置：GamePad（clock, command, attention, data, Pressures?, Rumble?）检查错误
    error = ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT, pressures, rumble);
    Serial.print("#try config ");
    Serial.println(tryNum);
    tryNum ++;
  }

  type = ps2x.readType();
  if(type == 1){
    Serial.println(" DualShock Controller found ");
  }else{
    Serial.println(" No Controller found ");
  }

  Serial.println("Starting BLE work!");
  bleKeyboard.begin();
}

void loop() {
  if(bleKeyboard.isConnected()) {
    ps2x.read_gamepad(false, vibrate); //read controller and set large motor to spin at 'vibrate' speed  //读取控制器并设置大型电机以“振动”速度旋转

    //will be TRUE as long as button is pressed  //只要按下按钮就为TRUE
    if(ps2x.NewButtonState(PSB_START)){
      if(ps2x.Button(PSB_START)){
        Serial.println("Start pressed");
        bleKeyboard.press(KEY_NUM_ENTER);
      }
      else{
        Serial.println("Start released");
        bleKeyboard.release(KEY_NUM_ENTER);
      }
    }
      
    if(ps2x.NewButtonState(PSB_SELECT)){
      if(ps2x.Button(PSB_SELECT)){
        Serial.println("Select pressed");
        bleKeyboard.press(KEY_TAB); // 示例：选择键映射到ESC键
      }
      else{
        Serial.println("Select released");
        bleKeyboard.release(KEY_TAB);
      }
    }
    
    // 方向键处理
    if(ps2x.NewButtonState(PSB_PAD_UP)){
      if(ps2x.Button(PSB_PAD_UP)){
        Serial.println("Up pressed");
        bleKeyboard.press(KEY_UP_ARROW);
      }
      else{
        Serial.println("Up released");
        bleKeyboard.release(KEY_UP_ARROW);
      }
    }
    
    if(ps2x.NewButtonState(PSB_PAD_RIGHT)){
      if(ps2x.Button(PSB_PAD_RIGHT)){
        Serial.println("Right pressed");
        bleKeyboard.press(KEY_RIGHT_ARROW);
      }
      else{
        Serial.println("Right released");
        bleKeyboard.release(KEY_RIGHT_ARROW);
      }
    }
    
    if(ps2x.NewButtonState(PSB_PAD_DOWN)){
      if(ps2x.Button(PSB_PAD_DOWN)){
        Serial.println("Down pressed");
        bleKeyboard.press(KEY_DOWN_ARROW);
      }
      else{
        Serial.println("Down released");
        bleKeyboard.release(KEY_DOWN_ARROW);
      }
    }
    
    if(ps2x.NewButtonState(PSB_PAD_LEFT)){
      if(ps2x.Button(PSB_PAD_LEFT)){
        Serial.println("Left pressed");
        bleKeyboard.press(KEY_LEFT_ARROW);
      }
      else{
        Serial.println("Left released");
        bleKeyboard.release(KEY_LEFT_ARROW);
      }
    }
    
    // L3, R3, L2, R2 处理
    if(ps2x.NewButtonState(PSB_L3)){
      if(ps2x.Button(PSB_L3)){
        Serial.println("L3 pressed");
        // 可以在这里添加L3对应的动作
      }
      else{
        Serial.println("L3 released");
        // 可以在这里添加L3释放后需要执行的动作
      }
    }
    
    if(ps2x.NewButtonState(PSB_R3)){
      if(ps2x.Button(PSB_R3)){
        Serial.println("R3 pressed");
        // 可以在这里添加R3对应的动作
      }
      else{
        Serial.println("R3 released");
        // 可以在这里添加R3释放后需要执行的动作
      }
    }
    
    if(ps2x.NewButtonState(PSB_L2)){
      if(ps2x.Button(PSB_L2)){
        Serial.println("L2 pressed");
        bleKeyboard.press('e');
      }
      else{
        Serial.println("L2 released");
        bleKeyboard.release('e');
      }
    }
    
    if(ps2x.NewButtonState(PSB_R2)){
      if(ps2x.Button(PSB_R2)){
        Serial.println("R2 pressed");
        bleKeyboard.press('q');
      }
      else{
        Serial.println("R2 released");
        bleKeyboard.release('q');
      }
    }

   // L1 按钮处理
    if(ps2x.NewButtonState(PSB_L1)){
      if(ps2x.Button(PSB_L1)){
        Serial.println("L1 pressed");
        bleKeyboard.press(KEY_LEFT_CTRL); // 示例：L1按下时映射到左Ctrl键
      }
      else{
        Serial.println("L1 released");
        bleKeyboard.release(KEY_LEFT_CTRL);
      }
    }

    // R1 按钮处理
    if(ps2x.NewButtonState(PSB_R1)){
      if(ps2x.Button(PSB_R1)){
        Serial.println("R1 pressed");
        bleKeyboard.press(KEY_RIGHT_CTRL); // 示例：R1按下时映射到右Ctrl键
      }
      else{
        Serial.println("R1 released");
        bleKeyboard.release(KEY_RIGHT_CTRL);
      }
    }

    // 形状按钮 △ 处理
    if(ps2x.NewButtonState(PSB_TRIANGLE)){
      if(ps2x.Button(PSB_TRIANGLE)){
        Serial.println("△ pressed");
        bleKeyboard.press('t'); // 示例：三角形按钮映射到'T'键
      }
      else{
        Serial.println("△ released");
        bleKeyboard.release('t');
      }
    }

    // 形状按钮 ○ 处理
    if(ps2x.NewButtonState(PSB_CIRCLE)){
      if(ps2x.Button(PSB_CIRCLE)){
        Serial.println("○ pressed");
        bleKeyboard.press('o'); // 示例：圆圈按钮映射到'O'键
      }
      else{
        Serial.println("○ released");
        bleKeyboard.release('o');
      }
    }

    // 形状按钮 X 处理
    if(ps2x.NewButtonState(PSB_CROSS)){
      if(ps2x.Button(PSB_CROSS)){
        Serial.println("X pressed");
        bleKeyboard.press('x'); // 示例：交叉按钮映射到'X'键
      }
      else{
        Serial.println("X released");
        bleKeyboard.release('x');
      }
    }

    // 形状按钮 □ 处理
    if(ps2x.NewButtonState(PSB_SQUARE)){
      if(ps2x.Button(PSB_SQUARE)){
        Serial.println("□ pressed");
        bleKeyboard.press('z'); // 示例：方块按钮映射到'S'键
      }
      else{
        Serial.println("□ released");
        bleKeyboard.release('z');
      }
    }



    LX_ = ps2x.Analog(PSS_LX);
    LY_ = ps2x.Analog(PSS_LY);
    RX_ = ps2x.Analog(PSS_RX);
    RY_ = ps2x.Analog(PSS_RY);

    if(LX_ < 128){
      Serial.print("LX:");Serial.println(LX_);
      bleKeyboard.release('d');
      bleKeyboard.press('a');
    }else if(LX_ > 128){
      Serial.print("LX:");Serial.println(LX_);
      bleKeyboard.release('a');
      bleKeyboard.press('d');
    }
    else{
      bleKeyboard.release('a');
      bleKeyboard.release('d');
    }

    if(LY_ < 127){
      Serial.print("LY:");Serial.println(LY_);
      bleKeyboard.release('s');
      bleKeyboard.press('w');
    }else if(LY_ > 127){
      Serial.print("LY:");Serial.println(LY_);
      bleKeyboard.release('w');
      bleKeyboard.press('s');
    }
    else{
      bleKeyboard.release('w');
      bleKeyboard.release('s');
    }

    if(RX_ < 128){
      Serial.print("RX:");Serial.println(RX_);
      bleKeyboard.release(KEY_NUM_3);
      bleKeyboard.press(KEY_NUM_1);
    }else if(RX_ > 128){
      Serial.print("RX:");Serial.println(RX_);
      bleKeyboard.release(KEY_NUM_1);
      bleKeyboard.press(KEY_NUM_3);
    }
    else{
      bleKeyboard.release(KEY_NUM_1);
      bleKeyboard.release(KEY_NUM_3);
    }

    if(RY_ < 127){
      Serial.print("RY:");Serial.println(RY_);
      bleKeyboard.release(KEY_NUM_2);
      bleKeyboard.press(KEY_NUM_5);
    }else if(RY_ > 127){
      Serial.print("RY:");Serial.println(RY_);
      bleKeyboard.release(KEY_NUM_5);
      bleKeyboard.press(KEY_NUM_2);
    }
    else{
      bleKeyboard.release(KEY_NUM_5);
      bleKeyboard.release(KEY_NUM_2);
    }




    delay(50);
    t++;
    if(!(t % 10)){
      digitalWrite(2, state);
      state = !state;
    }

  }else{
    Serial.println("Waiting Connect...");
    
    for (int i = 0; i < 30; i++)
    {
      t++;
      delay(50);
      digitalWrite(2, state);
      state = !state;
    }
    
  }
}