#include <Servo.h> //サーボモーター用ライブラリ読み込み

/** サーボ用のオブジェクト. **/
Servo myservo;
/** デジタル9番ピンをxxxとして設定. **/
int MOTOR_LEFT = 9;
/** デジタル10番ピンをxxxとして設定. **/
int MOTOR_RIGHT = 10;
/** デジタル3番ピンをxxxとして設定. **/
int MOTOR_PWM = 3;
/** デジタル2番ピンをサーボの角度命令出力ピンとして設定. **/
int SERVO_PIN = 2;

void setup() {
  pinMode(MOTOR_LEFT, OUTPUT);
  pinMode(MOTOR_RIGHT, OUTPUT);
  myservo.attach(SERVO_PIN);
}

void loop() {
  speedCntrol(1);
  speedCntrol(0);
  speedCntrol(-1);

  servoControl(0);
  servoControl(1);
  servoControl(2);
  servoControl(3);
  servoControl(4);
  servoControl(5);
  servoControl(0);
  servoControl(-1);
  servoControl(-2);
  servoControl(-3);
  servoControl(-4);
  servoControl(-5);
}

/**
 * スピードを制御する.
 * 
 * @param num 
 */
void speedCntrol(int num) {
  boolean leftPin = HIGH;
  boolean rightPin = LOW;
  int speed = abs(num) * 10;

  if (speed < 0) {
      leftPin = LOW;
      rightPin = HIGH;
  } else if (speed == 0) {
      leftPin = LOW;
      rightPin = LOW;
  }

  analogWrite(MOTOR_PWM, speed);
  digitalWrite(MOTOR_LEFT, leftPin);
  digitalWrite(MOTOR_RIGHT, rightPin);
  
  delay(2000);
}

/**
 * ハンドル制御.
 * 
 * 0:90  1:80    2:70   3:60   4:50   5:40
 * 0:90 -1:-100 -2:110 -3:120 -4:130 -5:140
 *
 * @param num
 */
void servoControl(int num) {
  int angle = 90 + (num * 10);
  myservo.write(angle);
  delay(2000);
}

