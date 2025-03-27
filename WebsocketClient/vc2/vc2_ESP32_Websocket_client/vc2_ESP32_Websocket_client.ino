// vc2_ESP32_Websocket_client.ino  (на основе WebSocketClient_Demo.ino)
// https://techtutorialsx.com/2017/11/01/esp32-arduino-websocket-client/

/*
Чтобы нам не пришлось беспокоиться о низкоуровневых деталях протокола websocket, мы воспользуемся вспомогательной библиотекой, которая возьмёт на себя эти детали.
Вы можете ознакомиться со страницей библиотеки на GitHub здесь: https://github.com/morrissinger/ESP8266-Websocket. Обратите внимание, что это библиотека 
для ESP8266, и на момент написания статьи официальной поддержки ESP32 не было. Тем не менее, с некоторыми изменениями мы сможем запустить её на ESP32.

К сожалению, эта библиотека недоступна в менеджере библиотек Arduino IDE. Поэтому нам придётся установить её вручную.


*/

//#include <ESP8266WiFi.h>
//#include <WebSocketClient.h>
#include <WiFi.h>
#include <WebSocketClient.h>

const char* ssid     = "TP-Link_B394";
const char* password = "18009217";

char path[] = "/";
char host[] = "echo.websocket.org";

// Нам также понадобится объект класса WebSocketClient, который будет предоставлять функции, 
// необходимые для взаимодействия с сервером. Наконец, нам понадобится объект класса WiFiClient, 
// который будет использоваться объектом WebSocketClient.  
// Use WiFiClient class to create TCP connections
WebSocketClient webSocketClient;
WiFiClient client;

void setup() 
{
  Serial.begin(115200);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
 
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
 
  delay(5000);
 
  if (client.connect(host, 80)) 
  {
    Serial.println("Connected");
  } 
  else 
  {
    Serial.println("Connection failed.");
  }
 
  webSocketClient.path = path;
  webSocketClient.host = host;
  if (webSocketClient.handshake(client)) 
  {
    Serial.println("Handshake successful");
  } 
  else 
  {
    Serial.println("Handshake failed.");
  }
}

/*
void setup() 
{
  Serial.begin(115200);
  delay(10);

  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  delay(5000);

  // После подключения к сети Wi-Fi мы установим TCP-соединение с хостом, вызвав метод connect нашего WiFiClient.
  // Этот метод принимает в качестве первого аргумента строку с именем хоста, к которому мы пытаемся подключиться 
  // (которое мы ранее объявили глобальной переменной), и порт, на котором он прослушивает запросы. Тестовый сервер, 
  // который мы используем, прослушивает запросы на порту 80, который является портом HTTP по умолчанию.

  //Мы также включаем проверку ошибок для этой попытки подключения. Если она завершится неудачей, 
  // в последовательный порт будет выведено сообщение, но программа продолжит работу и в конечном 
  // итоге завершится ошибкой. Мы делаем это для простоты, поскольку находимся в контролируемой 
  // тестовой среде. Чтобы устойчивая программа могла использоваться в реальных условиях, 
  // мы должны включить более надёжную проверку ошибок и действовать соответствующим образом при обнаружении ошибки.
  // Connect to the websocket server
  if (client.connect("echo.websocket.org", 80)) 
  {
    Serial.println("Connected");
  } 
  else 
  {
    Serial.println("Connection failed.");
    while(1) 
    {
      // Hang on failure
    }
  }

  // Далее нам нужно выполнить рукопожатие веб-сокета. Это делается путём вызова метода рукопожатия 
  // объекта WebSocketClient, который мы объявили ранее. Этот метод получает на вход наш WiFiClient 
  // и использует его для связи с сервером.

  // Одна из самых важных частей заключается в том, что сервер отправляет HTTP-запрос для обновления 
  // соединения до веб-сокета. Это обновление необходимо, поскольку протокол веб-сокета был разработан таким образом, 
  // что сервер может использовать один и тот же порт для связи как с HTTP-клиентами, так и с клиентами веб-сокета. 
  // Таким образом, клиенту необходимо указать, что он хочет использовать веб-сокет.

  // Но прежде чем мы перейдём к вызову метода рукопожатия, нам нужно присвоить элементам данных path и host 
  // нашего WebSocketClient хост и путь URL-адреса, которые мы ранее определили в глобальных переменных.
  // Handshake with the server
  webSocketClient.path = path;
  webSocketClient.host = host;
  if (webSocketClient.handshake(client)) 
  {
    Serial.println("Handshake successful");
  } 
  else 
  {
    Serial.println("Handshake failed.");
    while(1) 
    {
      // Hang on failure
    }  
  }
}
*/

/*
void loop() 
{
  String data;

  if (client.connected()) 
  {
    
    webSocketClient.getData(data);
    if (data.length() > 0) 
    {
      Serial.print("Received data: ");
      Serial.println(data);
    }
    
    // capture the value of analog 1, send it along
    pinMode(1, INPUT);
    data = String(analogRead(1));
    
    webSocketClient.sendData(data);
    
  } 
  else 
  {
    Serial.println("Client disconnected.");
    while (1) 
    {
      // Hang on disconnect.
    }
  }
  
  // wait to fully let the client disconnect
  delay(3000);
}
*/


// Перейдя к основному циклу, мы теперь будем обрабатывать отправку и получение данных на сервер. 
// Мы начнём с объявления переменной типа String, которая будет использоваться в качестве буфера 
// данных для содержимого, которое мы будем получать с сервера.

// Далее мы проверим, установлено ли соединение с сервером. Помните, что ранее мы подключились 
// к серверу с помощью нашего объекта WiFiClient. Таким образом, мы проверим соединение, 
// вызвав его метод connected, который не принимает аргументов и возвращает true, 
// если соединение активно, и false в противном случае.

// Может показаться странным, что мы используем WiFiClient на этом этапе после инициализации 
// WebSocketClient. Тем не менее, WebSocketClient можно рассматривать как оболочку поверх WiFiClient, 
// которая обрабатывает особенности протокола веб-сокетов. Таким образом, проверка доступности соединения
// — это функция более низкого уровня, и поэтому мы обращаемся непосредственно к объекту WiFiClient.

// Итак, если клиент всё ещё подключён к серверу, мы отправим ему некоторые данные. 
// Для этого мы просто вызываем метод sendData нашего объекта WebSocketClient, 
// передавая в качестве входных данных строку с содержимым, которое мы хотим отправить. 
// Эта функция возвращает значение void.

// Как мы уже говорили, сервер, к которому мы обращаемся, просто повторяет данные, 
// которые мы ему отправляем. Таким образом, мы можем получать данные от клиента, 
// и они должны точно соответствовать тому, что мы отправили ранее.

// Итак, чтобы получить данные с сервера, мы вызываем функцию getData и передаём в качестве 
// входных данных наш строковый буфер, в который будет помещён контент, возвращённый сервером.
// После этого мы можем просто вывести полученные данные, если их длина больше 0. Ниже вы можете 
// ознакомиться с полной функцией основного цикла, в которую уже включена эта функция 
// вывода. Обратите внимание, что мы также добавили небольшую задержку перед каждой итерацией функции основного цикла.

void loop() 
{
  String data;
 
  if (client.connected()) 
  {
 
    webSocketClient.sendData("Info to be echoed back");
 
    webSocketClient.getData(data);
    if (data.length() > 0) 
    {
      Serial.print("Received data: ");
      Serial.println(data);
    }
 
  } 
  else 
  {
    Serial.println("Client disconnected.");
  }
 
  delay(3000);
}
