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

// 2025-03-24 первоначальные настройки 
//const String channelID = "2005329";             // Change this to your channel ID
//const String writeApiKey = "V6YOTILH9I7D51F9";  // Change this to your Write API key
//const String readApiKey = "34W6LGLIFXD56MPM";   // Change this to your Read API key

// Канал: Probatve
const String channelID = "2890169";             // Change this to your channel ID
const String writeApiKey = "B1EA27SPA485JSQS";  // Change this to your Write API key
const String readApiKey = "PV6U89DCSBUY71XO";   // Change this to your Read API key

// В примере по умолчанию используется одно поле данных с именем "field1"
// Для вашего собственного сервера вы, конечно, можете создать больше таких полей.
int field1 = 0;

int numberOfResults = 3;  // Количество результатов, которые нужно прочитать
int fieldNumber = 1;      // Номер поля, который будет считан

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

void readResponse(NetworkClient *client) {
  unsigned long timeout = millis();
  while (client->available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client->stop();
      return;
    }
  }

  // Read all the lines of the reply from server and print them to Serial
  while (client->available()) {
    String line = client->readStringUntil('\r');
    Serial.print(line);
  }

  Serial.printf("\nClosing connection\n\n");
}

void loop() 
{
  NetworkClient client;
  String footer = String(" HTTP/1.1\r\n") + "Host: " + String(host) + "\r\n" + "Connection: close\r\n\r\n";

  // WRITE --------------------------------------------------------------------------------------------
  if (!client.connect(host, httpPort)) 
  {
    Serial.println("Нет подключения к "+String(host)+": "+String(httpPort));
    return;
  }

  client.print("GET /update?api_key=" + writeApiKey + "&field1=" + field1 + footer);
  readResponse(&client);

  // READ --------------------------------------------------------------------------------------------

  String readRequest = "GET /channels/" + channelID + "/fields/" + fieldNumber + ".json?results=" + numberOfResults + " HTTP/1.1\r\n" + "Host: " + host + "\r\n"
                       + "Connection: close\r\n\r\n";

  if (!client.connect(host, httpPort)) {
    return;
  }

  client.print(readRequest);
  readResponse(&client);

  // -------------------------------------------------------------------------------------------------

  ++field1;
  delay(10000);
}
