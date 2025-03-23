
#include <ESP8266WiFi.h>
#include <WebSocketClient.h>

//const char* ssid     = "OPPO A9 2020";
//const char* password = "b277a4ee84e8";

const char* ssid     = "TP-Link_B394";
const char* password = "18009217";

char path[] = "/";
//char path[] = "ws://";
char host[] = "echo.websocket.org";
  
WebSocketClient webSocketClient;

// Use WiFiClient class to create TCP connections
WiFiClient client;

void setup() {
  Serial.begin(115200);
  delay(10);

  // We start by connecting to a WiFi network

  Serial.println();
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

  delay(5000);
  

  // Connect to the websocket server


  //wss://echo.websocket.org
  if (client.connect("echo.websocket.org", 80)) 
  //if (client.connect("wss://echo.websocket.org", 80)) 
  {
    Serial.println("Connected");
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


void loop() {
  String data;

  if (client.connected()) {
    
    webSocketClient.getData(data);
    if (data.length() > 0) {
      Serial.print("Received data: ");
      Serial.println(data);
    }
    
    // capture the value of analog 1, send it along
    pinMode(1, INPUT);
    data = String(analogRead(1));
    
    webSocketClient.sendData(data);
    
  } else {
    Serial.println("Client disconnected.");
    while (1) {
      // Hang on disconnect.
    }
  }
  
  // wait to fully let the client disconnect
  delay(3000);
  
}



/*
#include <ESP8266WiFi.h>
#include <WebSocketClient.h>

//const char* ssid     = "OPPO A9 2020";
//const char* password = "b277a4ee84e8";

const char* ssid     = "TP-Link_B394";
const char* password = "18009217";
 
char path[] = "/echo";
char host[] = "demos.kaazing.com";
 
WebSocketClient webSocketClient;
WiFiClient client;
 
void setup() {
  Serial.begin(115200);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
 
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
 
  delay(5000);
 
  if (client.connect(host, 80)) {
    Serial.println("Connected");
  } else {
    Serial.println("Connection failed.");
  }
 
  webSocketClient.path = path;
  webSocketClient.host = host;
  if (webSocketClient.handshake(client)) {
    Serial.println("Handshake successful");
  } else {
    Serial.println("Handshake failed.");
  }
 
}
 
void loop() {
  String data;
 
  if (client.connected()) {
 
    webSocketClient.sendData("Info to be echoed back");
 
    webSocketClient.getData(data);
    if (data.length() > 0) {
      Serial.print("Received data: ");
      Serial.println(data);
    }
 
  } else {
    Serial.println("Client disconnected.");
  }
 
  delay(3000);
 
}
*/

