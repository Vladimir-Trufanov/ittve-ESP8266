## WiFiClient

Этот пример демонстрирует чтение и запись данных из веб-сервиса и в него, которые могут использоваться для регистрации данных, создания аналитических данных и принятия мер на основе этих данных.

#### Поддерживаемые цели: | ESP32 | ESP32-S2 | ESP32-C3 | ESP32-S3 |

### Структура класса

```
class WiFiClass : public WiFiGenericClass, public WiFiSTAClass, public WiFiScanClass, public WiFiAPClass

Зависимости:

WiFi => WiFiClient => NetWorkClient:
   int NetworkClient::connect(const char *host, uint16_t port);
   

```


### How to Use Example

Запустите этот пример и посмотрите на последовательный вывод. Вы также можете ознакомиться со значениями на [https://thingspeak.com/channels/2005329](https://thingspeak.com/channels/2005329)

Пожалуйста, обратите внимание, что доступ к этому общедоступному каналу может получить любой желающий, и не исключено, что еще больше людей напишут о своих ценностях.

### Configure the Project:

- сhange `SSID` and `password` to connect to your Wi-Fi.

Значения по умолчанию позволят вам использовать этот пример без каких-либо изменений. Если вы хотите использовать свой собственный канал, но у вас его еще нет, выполните следующие действия:

* Create an account on [thingspeak.com](https://www.thingspeak.com).

* После входа в систему нажмите на кнопку "Новый канал", чтобы создать новый канал для ваших данных. Именно здесь будут храниться и отображаться ваши данные.

```
tve05021958@gmail.com
x92-P362
```

* Заполните название, описание и другие поля для вашего канала по желанию, затем нажмите кнопку "Сохранить канал".

```
Probatve
Пока изучение возможностей
https://doortry.ru
https://github.com/Vladimir-Trufanov
```

* Заполните название, описание и другие поля для вашего канала по желанию, затем нажмите кнопку "Сохранить канал".

```
Probatve
Channel ID: 2890169
Author: mwa0000037313108
Access: Private

Write API Key
B1EA27SPA485JSQS

Read API Keys
PV6U89DCSBUY71XO

```

* Замените идентификатор канала на вкладке "Настройки канала" и закрытый ключ на "Чтение ключей API" на вкладке "Ключи API".

* Замените переменную host на имя хоста сервера thingspeak "api.thingspeak.com"

* Загрузите эскиз на свою плату ESP32 и убедитесь, что плата подключена к Интернету. Теперь ESP32 должен отправлять данные на ваш канал Thingspeak с интервалами, указанными функцией loop.

* Перейдите на страницу просмотра каналов на thingspeak и проверьте "Поле 1" на наличие новых входящих данных.

* Вы можете использовать инструменты визуализации и анализа данных, предоставляемые Thingspeak, для отображения и обработки ваших данных различными способами.

* Пожалуйста, обратите внимание, что Thingspeak принимает только целочисленные значения.

#### Config example:

You can find the data to be changed at the top of the file:

```cpp
const char* ssid     = "your-ssid"; // Change this to your WiFi SSID
const char* password = "your-password"; // Change this to your WiFi password

const char* host = "api.thingspeak.com"; // This should not be changed
const int httpPort = 80; // This should not be changed
const String channelID   = "2005329"; // Change this to your channel ID
const String writeApiKey = "V6YOTILH9I7D51F9"; // Change this to your Write API key
const String readApiKey = "34W6LGLIFXD56MPM"; // Change this to your Read API key

// The default example accepts one data filed named "field1"
// For your own server you can ofcourse create more of them.
int field1 = 0;

int numberOfResults = 3; // Number of results to be read
int fieldNumber = 1; // Field number which will be read out
```

#### Using Arduino IDE

To get more information about the Espressif boards see [Espressif Development Kits](https://www.espressif.com/en/products/devkits).

* Before Compile/Verify, select the correct board: `Tools -> Board`.
* Select the COM port: `Tools -> Port: xxx` where the `xxx` is the detected COM port.

#### Using Platform IO

* Select the COM port: `Devices` or set the `upload_port` option on the `platformio.ini` file.

## Example Log Output

The initial output which is common for all examples can be ignored:
```
SP-ROM:esp32c3-api1-20210207
Build:Feb  7 2021
rst:0x1 (POWERON),boot:0xc (SPI_FAST_FLASH_BOOT)
SPIWP:0xee
mode:DIO, clock div:1
load:0x3fcd5810,len:0x438
load:0x403cc710,len:0x918
load:0x403ce710,len:0x24e4
entry 0x403cc710
```
Follows the setup output where connection to your Wi-Fi happens:
```
******************************************************
Connecting to your-ssid
.
WiFi connected
IP address:
192.168.1.2
```
Then you can see the write log:
```
HTTP/1.1 200 OK
Date: Fri, 13 Jan 2023 13:12:31 GMT
Content-Type: text/plain; charset=utf-8
Content-Length: 1
Connection: close
Status: 200 OK
Cache-Control: max-age=0, private, must-revalidate
Access-Control-Allow-Origin: *
Access-Control-Max-Age: 1800
X-Request-Id: 188e3464-f155-44b0-96f6-0f3614170bb0
Access-Control-Allow-Headers: origin, content-type, X-Requested-With
Access-Control-Allow-Methods: GET, POST, PUT, OPTIONS, DELETE, PATCH
ETag: W/"5feceb66ffc86f38d952786c6d696c79"
X-Frame-Options: SAMEORIGIN

0
Closing connection
```
Last portion is the read log:
```
HTTP/1.1 200 OK
Date: Fri, 13 Jan 2023 13:12:32 GMT
Content-Type: application/json; charset=utf-8
Transfer-Encoding: chunked
Connection: close
Status: 200 OK
Cache-Control: max-age=7, private
Access-Control-Allow-Origin: *
Access-Control-Max-Age: 1800
X-Request-Id: 91b97016-7625-44f6-9797-1b2973aa57b7
Access-Control-Allow-Headers: origin, content-type, X-Requested-With
Access-Control-Allow-Methods: GET, POST, PUT, OPTIONS, DELETE, PATCH
ETag: W/"8e9c308fe2c50309f991586be1aff28d"
X-Frame-Options: SAMEORIGIN

1e3
{"channel":{"id":2005329,"name":"WiFiCLient example","description":"Default setup for Arduino ESP32 NetworkClient example","latitude":"0.0","longitude":"0.0","field1":"data0","created_at":"2023-01-11T15:56:08Z","updated_at":"2023-01-13T08:13:58Z","last_entry_id":2871},"feeds":[{"created_at":"2023-01-13T13:11:30Z","entry_id":2869,"field1":"359"},{"created_at":"2023-01-13T13:11:57Z","entry_id":2870,"field1":"361"},{"created_at":"2023-01-13T13:12:23Z","entry_id":2871,"field1":"363"}]}
0


Closing connection
```
After this the write+read log repeat every 10 seconds.


## Troubleshooting

***Important: Make sure you are using a good quality USB cable and that you have a reliable power source***

* **Wi-Fi not connected:** Check the SSID and password and also that the signal has sufficient strength.
* **400 Bad Request:** Check the writeApiKey.
* **404 Not Found:** Check the channel ID.
* **No data on chart / reading NULL:** Data must be sent as an integer, without commas.

If the error persists, you can ask for help at the official [ESP32 forum](https://esp32.com) or see [Contribute](#contribute).

## Contribute

To know how to contribute to this project, see [How to contribute.](https://github.com/espressif/arduino-esp32/blob/master/CONTRIBUTING.rst)

If you have any **feedback** or **issue** to report on this example/library, please open an issue or fix it by creating a new PR. Contributions are more than welcome!

Before creating a new issue, be sure to try the Troubleshooting and to check if the same issue was already created by someone else.

## Resources

* Official ESP32 Forum: [Link](https://esp32.com)
* Arduino-ESP32 Official Repository: [espressif/arduino-esp32](https://github.com/espressif/arduino-esp32)
* ESP32 Datasheet: [Link to datasheet](https://www.espressif.com/sites/default/files/documentation/esp32_datasheet_en.pdf)
* ESP32-S2 Datasheet: [Link to datasheet](https://www.espressif.com/sites/default/files/documentation/esp32-s2_datasheet_en.pdf)
* ESP32-C3 Datasheet: [Link to datasheet](https://www.espressif.com/sites/default/files/documentation/esp32-c3_datasheet_en.pdf)
* ESP32-S3 Datasheet: [Link to datasheet](https://www.espressif.com/sites/default/files/documentation/esp32-s3_datasheet_en.pdf)
* Official ESP-IDF documentation: [ESP-IDF](https://idf.espressif.com)
