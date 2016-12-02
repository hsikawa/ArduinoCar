#include <ESP8266WiFi.h>
#include <WebSocketClient.h>
#include <Servo.h> //サーボモーター用ライブラリ読み込み
#include <ArduinoJson.h> // JSON parser

const char* ssid     = "0024A5326C0B";
const char* password = "1pcem23pnjh2n";
char path[] = "/";
char host[] = "192.168.0.21";
int port = 3000;

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

/** TCP Client use WiFiClient*/
WiFiClient client;
/** WebSocket クライアント */
WebSocketClient webSocketClient;

/** JSONパース用バッファ */
StaticJsonBuffer<200> jsonBuffer;
// 
const char* JSON_SPEED_KEY = "speed";
const char* JSON_HANDLE_KEY = "handle";

void setup() {
  Serial.begin(115200);
  delay(10);

  // サーボピンの準備
  myservo.attach(SERVO_PIN);

  // Connect WiFi
  connectWifi(ssid, password);
  delay(2000);

  // Connect WebSocket
  connectWebSocket(host, port, path);
}

int position = 0;
String sendData = "aaa";

void loop() {
  String recvData = "";
  if (client.connected()) {
    Serial.println("Before getData");
    webSocketClient.getData(recvData);
    if (recvData.length() > 0) {
      Serial.print("Received data: ");
      Serial.println(recvData);
      // parse to json.
      JsonObject& root = jsonBuffer.parseObject(recvData);
      int servo = root[JSON_HANDLE_KEY];
      Serial.print("Servo control = ");
      Serial.println(servo);
      servoControl(servo);
      int speed = root[JSON_SPEED_KEY];
      Serial.print("Motor speed = ");
      Serial.println(speed);
      speedCntrol(speed);
    }
  } else {
    Serial.println("Client disconnected.");
    while (1) {
      // Hang on disconnect.
    }
  }

  // wait to fully let the client disconnect
  delay(3000);
  position++;
}

/**
 * WiFi接続
 */
void connectWifi(const char* ssid, const char* pass) {
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

/**
 * WebSocket接続を行う
 */
void connectWebSocket(char host[], int port, char path[]) {
  Serial.println("connectWebSocket start.");
  // Connect to the websocket server
  if (client.connect(host, port)) {
    Serial.println("TCP Connected.");
  } else {
    Serial.println("Connection failed.");
    while(1) {
      // Hang on failure
    }
  }

  // Handshake with the server
  webSocketClient.path = path;
  webSocketClient.host = host;
  if (webSocketClient.handshake(client)) {
    Serial.println("Handshake successful");
  } else {
    Serial.println("Handshake failed.");
    while(1) {
      // Hang on failure
    }  
  }
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

