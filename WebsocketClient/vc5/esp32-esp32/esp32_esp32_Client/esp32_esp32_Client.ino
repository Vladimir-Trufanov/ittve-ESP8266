// esp32_esp32_Client

// Подключаем библиотеки
#include <WiFi.h>
#include <WebServer.h>
#include <WebSocketsClient.h>

// Определяем название и пароль точки доступа
//const char* ssid     = "OPPO A9 2020";
//const char* password = "b277a4ee84e8";

const char* ssid     = "TP-Link_B394";
const char* password = "18009217";

// Определяем адрес сервера
const char* ADDR = "192.168.4.1";

// Определяем url подключения
const char* URL = "/";

// Определяем порт
const uint16_t PORT = 81;

// Создаём массив для отправки
int sdata[2] {1984, 11276};

// Создаём массив для получения
unsigned long rdata[3] {0};

// Кол-во элементов массива для получения
const size_t glen = sizeof(rdata) / sizeof(rdata[0]);

// Создаём экземпляр класса клиента
WebSocketsClient webSocket;

/* Callback функция события WebSocket.
 * Парамтетры:
 * type - тип событыя
 * payload - указатель на данные
 * length - размер данных
 */
void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {

    // Если тип данных двоичный и размер больше нуля
    if (type == WStype_BIN && length > 0) {

        // Преобразуем данные из байтов в десятичные числа без знака
        const unsigned long* tmp = (unsigned long*) payload;

        // Вычисляем размер данных в новом формате
        const size_t len = length / sizeof(*rdata);

        // Записываем в глобальную переменную
        for (size_t i = 0; i < len && i < glen; i++) {
            rdata[i] = tmp[i];
        }
        Serial.println();
    }
}

void setup() {

    // Инициируем последовательный порт
    Serial.begin(115200);

    // Устанавливаем режим работы в качестве клиента
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    // Ждём подключения WiFi
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(500);
    }

    Serial.println();
    Serial.print("IP адрес: ");
    Serial.println(WiFi.localIP());

    // Подключаемся к серверу
    webSocket.begin(ADDR, PORT, URL);

    // Метод событий WebSocket
    webSocket.onEvent(webSocketEvent);

    // Если соединение прервано, повторить попытку через 5 сек.
    webSocket.setReconnectInterval(5000);
}


void loop() {

    // Цикл WebSocket
    webSocket.loop();

    // Если прошла одна секунда и сервер хотя бы раз прислал данные
    if (millis() % 1000 == 0) {

        // Отправляем данные в двоичном формате серверу
        webSocket.sendBIN((uint8_t*)sdata, sizeof(sdata));

        Serial.println("Данные отправлены.");

        // Если данные сервера были получены хотябы один раз
        if (rdata[0] != 0) {

            Serial.println("Текущие данные сервера:");

            // Выводим массив данных в последовательный порт
            for (size_t i = 0; i < glen; i++)
                Serial.println(rdata[i]);
        }
    }
    // Если соединение WiFi прервано
    if (WiFi.status() != WL_CONNECTED)

        // Вызываем функцию setup(), для повторного подключения
        setup();
}