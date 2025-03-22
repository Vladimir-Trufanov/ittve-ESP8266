// va1.ino

// После установки всех библиотек нам нужно включить их в наш код для Arduino
#include <WiFi.h> // Include WIFi Library for ESP32
#include <WebServer.h> // Include WebSwever Library for ESP32
#include "ArduinoJson.h" // Include ArduinoJson Library
#include "DHT.h" // Include DHT Library
#include "WebSocketsServer.h" // Include Websocket Library

// Далее мы определяем тип датчика DHT и вывод, который мы используем для его подключения
#define DHTPIN 21 // DHT PIn 
#define DHTTYPE DHT22 // DHT Type

// Далее мы зададим SSID и пароль для нашей сети
const char* ssid     = "OPPO A9 2020";
const char* password = "b277a4ee84e8";

//const char* ssid     = "TP-Link_B394";
//const char* password = "18009217";


// Мы будем использовать функцию millis() для обновления данных о температуре за определенный период, поэтому нам нужно объявить несколько переменных
int interval = 1000; // virtual delay
unsigned long previousMillis = 0; // Tracks the time since last event fired

// Далее мы объявляем переменную строкового типа web в которой мы будем хранить всю веб-страницу. Содержимое этой переменной строкового типа - это то, что вы будете видеть на веб-странице
String web = "<!DOCTYPE html><html><head> <title>Arduino and ESP32 Websocket</title> <meta name='viewport' content='width=device-width, initial-scale=1.0' /> <meta charset='UTF-8'> <style> body { background-color: #E6D8D5; text-align: center; } </style></head><body> <h1>Temperature: <span id='temp'>-</span></h1> <h1>Humidity: <span id='hum'>-</span></h1> <h1>Received message: <span id='message'>-</span></h1> <button type='button' id='BTN_1'> <h1>ON</h1> </button> <button type='button' id='BTN_2'> <h1>OFF</h1> </button></body><script> var Socket; // Добавляем два события addEventListener(), которые будут вызываться // при нажатии любой из двух соответствующих кнопок. document.getElementById('BTN_1').addEventListener('click', button_1_pressed); document.getElementById('BTN_2').addEventListener('click', button_2_pressed); // Объявляем функцию init(), которая будет вызываться при загрузке HTML-страницы. // Внутри функции определяем socket, который сможет подключиться к нашему серверу // ESP32 и получать данные. В WebSocket('ws://' + window.location.hostname + ':81/') // запрашивается подключение к серверу веб-сокетов с IP-адресом на порту 81. // С помощью события onmessage, вызывается функция. Каждый раз, когда поступает // обновление, возникает событие, которое вызывает функцию processCommand(). // На стороне processCommand () разбирается входящий файл JSON и помещается // его значение в соответствующее место function init() { Socket = new WebSocket('ws://' + window.location.hostname + ':81/'); Socket.onmessage = function(event) { processCommand(event); }; } function processCommand(event) { var obj = JSON.parse(event.data); document.getElementById('message').innerHTML = obj.PIN_Status; document.getElementById('temp').innerHTML = obj.Temp; document.getElementById('hum').innerHTML = obj.Hum; console.log(obj.PIN_Status); console.log(obj.Temp); console.log(obj.Hum); } // Последние две функции button_1_pressed() и button_2_pressed() запускают // события, и когда они происходят, через WebSocket отправляется 1 или 0. function button_1_pressed() { Socket.send('1'); } function button_2_pressed() { Socket.send('0'); } window.onload = function(event) { init(); }</script></html>";

// Далее мы снова объявляем некоторые переменные строкового типа для хранения некоторых ценных данных
String jsonString; // Temporary storage for the JSON String
String pin_status = ""; // Holds the status of the pin
float t; // holds the temperature value
float h;// holds the Humidity value

// Далее мы создаём экземпляры для нашего датчика DHT, веб-сервера и сервера WebSocket
DHT dht(DHTPIN, DHTTYPE); // create instance for DHT sensor
WebServer server(80); // create instance for web server on port "80"
WebSocketsServer webSocket = WebSocketsServer(81); //create instance for webSocket server on port"81"

// Далее у нас есть раздел настройки. В разделе настройки мы определили все 
// необходимые входные и выходные контакты. Мы инициализируем последовательный 
// порт. Затем мы подключаемся к Wi-Fi и выводим IP-адрес в окне последовательного 
// монитора. После этого мы инициализируем веб-сервер и сервер WebSocket и, 
// наконец, инициализируем датчик DHT. На этом процесс настройки завершается.
void setup() 
{
  
 // put your setup code here, to run once:
 pinMode(22, OUTPUT); // Set PIN22 As output(LED Pin)
 Serial.begin(115200); // Init Serial for Debugging.
 WiFi.begin(ssid, password); // Connect to Wifi 
 while (WiFi.status() != WL_CONNECTED) { // Check if wifi is connected or not
 delay(500);
 Serial.print(".");
 }
 Serial.println();
 // Print the IP address in the serial monitor windows.
 Serial.print("IP address: ");
 Serial.println(WiFi.localIP());
 // Initialize a web server on the default IP address. and send the webpage as a response.
 server.on("/", []() {
 server.send(200, "text\html", web);
 });
 server.begin(); // init the server
 webSocket.begin(); // init the Websocketserver
 webSocket.onEvent(webSocketEvent); // init the webSocketEvent function when a websocket event occurs 
 dht.begin(); // Init DHT sensor
  
}

// В разделе цикла мы вызываем метод веб-сервера для обработки клиентов, чтобы 
// обработать всех клиентов веб-сервера, а также вызываем метод цикла сервера 
// WebSocket , чтобы обработать всех клиентов WebSocket. Затем мы вызываем 
// функцию millis для обновления данных о температуре и влажности.
void loop() 
{
  
 server.handleClient(); // webserver methode that handles all Client
 webSocket.loop(); // websocket server methode that handles all Client
 unsigned long currentMillis = millis(); // call millis and Get snapshot of time
 if ((unsigned long)(currentMillis - previousMillis) >= interval) { // How much time has passed, accounting for rollover with subtraction!
 update_temp_hum(); // update temperature data.
 update_webpage(); // Update Humidity Data
 previousMillis = currentMillis; // Use the snapshot to set track time until next event
 
 }
 
 // Далее у нас есть функция webSocketEvent() . Эта функция вызывается при возникновении события WebSocket. В этом разделе мы проверяем статус и выводим количество подключенных клиентов. Затем мы проверяем, подключен ли клиент через WebSocket. Если да, мы обновляем статус на веб-странице. Затем мы проверяем, отправил ли веб-сервер какие-либо данные в ответ. Если это так, мы обрабатываем его и в зависимости от состояния включаем или выключаем подключенный светодиод.
 // This function gets a call when a WebSocket event occurs
void webSocketEvent(byte num, WStype_t type, uint8_t * payload, size_t length) {
 switch (type) {
 case WStype_DISCONNECTED: // enum that read status this is used for debugging.
 Serial.print("WS Type ");
 Serial.print(type);
 Serial.println(": DISCONNECTED");
 break;
 case WStype_CONNECTED: // Check if a WebSocket client is connected or not
 Serial.print("WS Type ");
 Serial.print(type);
 Serial.println(": CONNECTED");
 if (digitalRead(22) == HIGH) { //check if pin 22 is high or low
 pin_status = "ON";
 update_webpage(); // update the webpage accordingly
 }
 else { 
 pin_status = "OFF"; //check if pin 22 is high or low
 update_webpage();// update the webpage accordingly
 }
 break;
 case WStype_TEXT: // check responce from client
 Serial.println(); // the payload variable stores teh status internally
 Serial.println(payload[0]);
 if (payload[0] == '1') { 
 pin_status = "ON";
 digitalWrite(22, HIGH); 
 }
 if (payload[0] == '0') {
 pin_status = "OFF";
 digitalWrite(22, LOW); 
 }
 break;
 }
}

// Функция update_temp_hum() предназначена для обновления данных о температуре и влажности
void update_temp_hum(){
  h = dht.readHumidity(); // Read temperature as Celsius (the default)
  t = dht.readTemperature(); // Read temperature as Fahrenheit (isFahrenheit = true
}

// Далее у нас есть функция update_webpage(). В этой функции мы обновляем 
// веб-страницу со всеми значениями. Внутри этой функции мы создали объект JSON 
// и выводим информацию из этого объекта для отладки. Затем мы передаём данные 
// из объекта JSON с помощью функции webSocket.broadcasrTXT(jsonString);
void update_webpage()
{
  StaticJsonDocument<100> doc;
  // create an object
  JsonObject object = doc.to<JsonObject>();
  object["PIN_Status"] = pin_status ;
  object["Temp"] = t ;
  object["Hum"] = h ;
  serializeJson(doc, jsonString); // serialize the object and save teh result to teh string variable.
  Serial.println( jsonString ); // print the string for debugging.
  webSocket.broadcastTXT(jsonString); // send the JSON object through the websocket
  jsonString = ""; // clear the String.
}

