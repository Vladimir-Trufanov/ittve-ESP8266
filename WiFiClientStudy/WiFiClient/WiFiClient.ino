/*
    - Перейдите по ссылке thingspeak.com и создайте учетную запись, если у вас ее еще нет.
    - После входа в систему нажмите на кнопку "Новый канал", чтобы создать новый канал для 
    ваших данных. Именно здесь будут храниться и отображаться ваши данные.
    - Заполните название, описание и другие поля для вашего канала по желанию, затем нажмите 
    кнопку "Сохранить канал".
    - Обратите внимание на "Написать ключ API", расположенный на вкладке "Ключи API", 
    это ключ, который вы будете использовать для отправки данных на свой канал.
    - Замените идентификатор канала на вкладке "Настройки канала" и PrivateKey на 
    "Чтение ключей API" на вкладке "Ключи API".
    - Замените переменную host на имя хоста сервера thingspeak "api.thingspeak.com"
    - Загрузите эскиз на свою плату ESP32 и убедитесь, что плата подключена к Интернету. 
    Теперь ESP32 должен отправлять данные на ваш канал Thingspeak с интервалами, 
    указанными функцией loop.
    - Перейдите на страницу просмотра канала на thingspeak и проверьте "Field1" на 
    наличие новых входящих данных.
    Вы можете использовать инструменты визуализации и анализа данных, предоставляемые 
    Thingspeak, для отображения и обработки ваших данных различными способами.
    Пожалуйста, обратите внимание, что Thingspeak принимает только целые значения.

    You can later check the values at https://thingspeak.com/channels/2005329
    Please note that this public channel can be accessed by anyone and it is 
    possible that more people will write their values.
 */

#include <WiFi.h>

const char* ssid     = "TP-Link_B394";
const char* password = "18009217";

const char *host = "api.thingspeak.com";        // This should not be changed
const int httpPort = 80;                        // This should not be changed

//const char *host = "http://127.0.0.1";
//const char *host = "127.0.0.1";
//const char *host = "http://localhost";
//const int httpPort = 7771;                       
//const char *host = "https://probatv.ru";
//const int httpPort = 80;                       

// 2025-03-24 первоначальные настройки 
const String channelID = "2005329";             // Change this to your channel ID
const String writeApiKey = "V6YOTILH9I7D51F9";  // Change this to your Write API key
const String readApiKey = "34W6LGLIFXD56MPM";   // Change this to your Read API key

// Канал: Probatve
//const String channelID = "2890169";             // Change this to your channel ID
//const String writeApiKey = "B1EA27SPA485JSQS";  // Change this to your Write API key
//const String readApiKey = "PV6U89DCSBUY71XO";   // Change this to your Read API key

// В примере по умолчанию используется одно поле данных с именем "field1"
// Для вашего собственного сервера вы, конечно, можете создать больше таких полей.
int field1 = 0;
int nLine=0;

int numberOfResults = 3;  // Количество результатов, которые нужно прочитать
int fieldNumber = 1;      // Номер поля, который будет считан
bool lFirst;              // true - первый заголовок текущего ответа сервера

NetworkClient client;

void setup() 
{
  Serial.begin(115200);
  while (!Serial) delay(100);

  // Начинаем с подключения к сети Wi-Fi
  Serial.println();
  Serial.println("******************************************************");
  Serial.print("Подключаемся к "); Serial.println(ssid);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("WiFi подключена по IP адресу: "); Serial.println(WiFi.localIP());
}

// Принять ответ от сервера
void readResponse(NetworkClient *client) 
{
  unsigned long timeout = millis();
  while (client->available() == 0) 
  {
    if (millis() - timeout > 5000) 
    {
      Serial.println(">>> Client Timeout !");
      client->stop();
      return;
    }
  }
  // Прочитать все строки ответа с сервера и распечатать их 
  lFirst=true;
  while (client->available()) 
  {
    String line = client->readStringUntil('\r');
    viewSrv(line);
  }
  Serial.printf("Closing connection\n");
}

// Инкрементировать и форматировать счетчик
String incLine()
{
  String Line;
  nLine++;  
  if (nLine<10) Line=".."+String(nLine);
  else if (nLine<100) Line="."+String(nLine);
  else Line=String(nLine);
  return Line;
}

// Показать отправленный заголовок на сервер
void toServer(String str)
{
  client.print(str);
  Serial.println(incLine()+"["+str.substring(0,str.length()-1)+"]ВК+ПС");
}
// Показать принятый заголовок от сервера
void viewSrv(String str)
{
  if (lFirst) 
  {
    Serial.println(incLine()+"["+str+"]ВК");
    lFirst=false;
  }
  else Serial.println(incLine()+"["+str.substring(1)+"]ВК");
}

void loop() 
{
  // Начинаем очередной цикл обмена
  Serial.println("------------------------------------ Начинается цикл обмена: "+String(field1));

  // Подключаемся к серверу
  if (!client.connect(host, httpPort)) 
  {
    Serial.println("Нет подключения к "+String(host)+": "+String(httpPort));
    return;
  }
  // WRITE --------------------------------------------------------------------------------------------
  Serial.println("Запрос на передачу значения серверу: WRITE");
  // String footer = String(" HTTP/1.1\r\n") + "Host: " + String(host) + "\r\n" + "Connection: close\r\n\r\n";
  // toServer="GET /update?api_key=" + writeApiKey + "&field1=" + field1 + footer;
  toServer("GET /update?api_key=" + writeApiKey + "&field1=" + field1 + String(" HTTP/1.1\r\n"));
  toServer("Host: " + String(host) + "\r\n");
  toServer("Connection: close\r\n");
  toServer("\r\n");

  Serial.println("Ответ сервера");
  readResponse(&client);

  // READ --------------------------------------------------------------------------------------------
  Serial.println("Запрос на прием ответа от сервера: READ");
  if (!client.connect(host, httpPort)) 
  {
    Serial.println("Нет подключения к "+String(host)+": "+String(httpPort));
    return;
  }
  //String readRequest = "GET /channels/" + channelID + "/fields/" + fieldNumber + 
  //   ".json?results=" + numberOfResults + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n";
  toServer("GET /channels/" + channelID + "/fields/" + fieldNumber + ".json?results=" + numberOfResults + " HTTP/1.1\r\n");
  toServer("Host: " + String(host) + "\r\n");
  toServer("Connection: close\r\n");
  toServer("\r\n");

  Serial.println("Ответ сервера");
  readResponse(&client);

  // -------------------------------------------------------------------------------------------------

  ++field1;
  delay(10000);
}
