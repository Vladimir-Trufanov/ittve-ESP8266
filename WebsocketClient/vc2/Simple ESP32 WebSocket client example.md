## [Simple ESP32 WebSocket client example](https://www.esp32.com/viewtopic.php?f=19&t=3269&p=16083&hilit=websocket#p16083)

Привет, ребята, далее будет мой код. Он отлично работает с моим сервером. Теперь мне нужно понять, как сделать это с помощью SSL.

Вот библиотека, которой я пользовался...
[https://github.com/morrissinger/ESP8266-Websocket](https://github.com/morrissinger/ESP8266-Websocket)

Вы вносите какие-то изменения в эту библиотеку?
Я пытаюсь использовать её так же, но получаю ошибки от компилятора...

Привет, да, мне пришлось закомментировать некоторые повторяющиеся ссылки. Если я правильно помню, это были ссылки на SHA и Hash в заголовочных файлах.

Чтобы моя Arduino IDE могла компилировать с использованием ESP8266-Websocket, я изменил:

```
* Arduino\libraries\ESP8266-Websocket\MD5.h
-Строка 30
из "void MD5Init (MD5_CTX *);"
в "void MD5InitXXX (MD5_CTX *);"
-Строка 31
из "void MD5Update (MD5_CTX *, unsigned char *, unsigned int);"
в "void MD5UpdateXXX (MD5_CTX *, unsigned char *, unsigned int);"
-Строка 32
от "void MD5Final (неподписанный символ [16], MD5_CTX *);"
до "void MD5FinalXXX (неподписанный символ [16], MD5_CTX *);"

* Arduino\libraries\ESP8266-Websocket\MD5.c
-Строка 92
из "void MD5Init (MD5_CTX *context)"
в "void MD5InitXXX (MD5_CTX *context)"
-Строка 106
из "void MD5Update (MD5_CTX *context, unsigned char * input,unsigned int inputLen)"
в "void MD5UpdateXXX (MD5_CTX *context, unsigned char * input,unsigned int inputLen) inputLen)"
-Строка 139
из "void MD5Final (неподписанный дайджест символов [16], MD5_CTX * контекст)"
в "void MD5FinalXXX (неподписанный дайджест символов [16], MD5_CTX * контекст)"
```

Привет, ребята, вот мой код. Он отлично работает с моим сервером. Теперь мне нужно понять, как сделать это с помощью SSL.

```
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebSocketClient.h>
#include <Preferences.h>

//eeprom
Preferences pref;


byte mac[6];
char macaddress[30];

WebSocketClient webSocketClient;

// Use WiFiClient class to create TCP connections
WiFiClient client;

void setup() {
  Serial.begin(115200);
  delay(10);

  //start eeprom
  pref.begin("xxx", false);
  delay(10);
  
  // We start by connecting to a WiFi network
  //get password and username from eeprom
  String pwd = pref.getString("pw", "xxx");
  int i = pwd.length() + 1;
  char password[i];
  pwd.toCharArray(password, i);
  String ssi = pref.getString("ssid", "xxx");
  i = ssi.length() + 1;
  char ssid[i];
  ssi.toCharArray(ssid, i);
  //get host from eeprom
  String eehos = pref.getString("eehost", "xxx.co.za");
  i = eehos.length() + 1;
  char eehost[i];
  eehos.toCharArray(eehost, i);
  //get path to websockets from eeprom
  String eepat = pref.getString("eepath", "/xxx/xxx.ashx?UN=");
  i = eepat.length() + 1;
  char eepath[i];
  eepat.toCharArray(eepath, i);

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

  delay(1000);

  //get the mac address of the Magafter
  WiFi.macAddress(mac);


  // Connect to the websocket server
  if (client.connect(eehost, 80)) {
    Serial.println("xxx Connected");
  } else {
    Serial.println("xxx Connection failed.");
    while (1) {
      // Hang on failure
    }
  }

  char path[50];
  memset(path, 0, sizeof(path));

  strcpy(path, eepath);
  
  //turn the mac array into a char array with separators
  memset(macaddress, 0, sizeof(macaddress));

  char result[5];
  for (int i = 0; i < 6; i++)
  {
    memset(result, 0, sizeof(result));
    sprintf(result, "%x", (char)mac[i]);
    //macaddress[i] = (char)mac[i];
    strcat(macaddress, result);
    strcat(macaddress, "-");
  }

  strcat(path, macaddress);

  // Handshake with the server
  webSocketClient.path = path;
  webSocketClient.host = eehost;
  if (webSocketClient.handshake(client)) {
    Serial.println("Handshake successful");
  } else {
    Serial.println("Handshake failed.");
    while (1) {
      // Hang on failure
    }
  }


}

String data;

void loop() {
  data = "";

  //esp_task_wdt_feed(); so much for watchdogs :-(

  if (client.connected()) {

    webSocketClient.getData(data);
    if (data.length() > 0) {
      Serial.print("Received data: ");
      Serial.println(data);
      if (data.indexOf("#qq") > -1)
      {
      //just silly test stuff
        String sdata = "{\"mBody\":\"Status is-" + String(millis()) + "\",\"mUser\":\"" + macaddress + "\"}" ;
        webSocketClient.sendData(sdata);
      }
      if (data.indexOf("#gdc") > -1)
      {
        String sdata = "{\"mBody\":\"Garage Door Close-" + String(millis()) + "\",\"mUser\":\"" + macaddress + "\"}" ;
        webSocketClient.sendData(sdata);
      }
    }



  } else {
    Serial.println("Client disconnected. Restarting system...");
    delay(2000);
    esp_restart(); //testing the restart command more than anything else
  }

 delay(1000);

}
```


