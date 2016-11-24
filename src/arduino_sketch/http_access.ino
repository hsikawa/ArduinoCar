#include <ESP8266WiFi.h>

#define WIFI_SSID "0024A5326C0B"
#define WIFI_PSK "1pcem23pnjh2n"
#define DEST_HOST "api.fixer.io"
#define DEST_PORT 80
#define DEST_URL "/latest?base=USD&symbols=JPY" 

//sample json data used in this sketch
// {"base":"USD","date":"2015-08-28","rates":{"JPY":120.84}}

void setup() {
  
  Serial.begin(115200);
  Serial.println("");
  delay(100);

  WiFi.begin(WIFI_SSID, WIFI_PSK);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("");

  WiFiClient client;
  String line;

  if (!client.connect(DEST_HOST, DEST_PORT)) {
    Serial.println("connection failed");
    return;
  }
  
  client.print(String("GET ") + DEST_URL + " HTTP/1.1\r\n" +
               "Host: " + DEST_HOST + "\r\n" + 
               "Connection: close\r\n\r\n");
  delay(10);

  //get rid of the HTTP headers
  while(client.available()){
    line = client.readStringUntil('\r');
    Serial.print(line);
    line.trim();
    if (line.length() == 0) {
      break;
    }
  }

  //get http content
  String buffer="";
  while(client.available()){
    line = client.readStringUntil('\r');
    line.trim();
    buffer.concat(line);
  }

  char json[buffer.length() + 1];
  buffer.toCharArray(json, sizeof(json));
  Serial.println(json);
  
  Serial.println("closing connection");

}

void loop() {
}

