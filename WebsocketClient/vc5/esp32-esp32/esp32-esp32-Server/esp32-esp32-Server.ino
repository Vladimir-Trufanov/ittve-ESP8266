// esp32-esp32-Server

// Подключаем библиотеки
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>
#include <WebSocketsServer.h>

// Определяем название и пароль точки доступа
//const char* ssid     = "OPPO A9 2020";
//const char* password = "b277a4ee84e8";

const char* ssid     = "TP-Link_B394";
const char* password = "18009217";

// Массив для отправки
unsigned long sdata[3] {1984, 1968, 0};

// Массив для получения
int rdata[2] {0};

// Кол-во элементов массива для получения
const size_t glen = sizeof(rdata) / sizeof(rdata[0]);

// Создаём объект сервера
WebSocketsServer webSocket = WebSocketsServer(81);

/* Callback функция события WebSocket.
 * Парамтетры:
 * num - номер клиента
 * type - тип событыя
 * payload - указатель на данные
 * length - размер данных
 */
void webSocketServerEvent(uint8_t num, WStype_t type, uint8_t* payload, size_t length) {

    // Записываем текущие millis в третий элемент массива отправки
    sdata[2] = millis();

    // Если тип данных двоичный и их размер не нулевой
    if (type == WStype_BIN && length > 0) {

        // Преобразуем тип данных
        const int* tmp = (int*) payload;

        // Вычисляем размер данных нового типа
        size_t len = length / sizeof(&rdata);

        // Записываем в глобальную переменную
        for (size_t i = 0; i < len && i < glen; i++) {
            rdata[i] = tmp[i];
        }

        // Отвечаем клиенту
        webSocket.sendBIN(num, (uint8_t*)sdata, sizeof(sdata));
    }
}

void setup() {

    // Инициируем последовательный порт
    Serial.begin(115200);
    Serial.println();

    Serial.println("Инициируем точку доступа WiFi");

    // Инициируем точку доступа WiFi
    WiFi.softAP(ssid, password);
    IPAddress myIP = WiFi.softAPIP();

    // Выводим IP-адрес Веб-сервера
    Serial.print("IP-адрес точки доступа: ");
    Serial.println(myIP);

    // Инициируем сервер
    webSocket.begin();

    // Метод событий WebSocket
    webSocket.onEvent(webSocketServerEvent);

    Serial.println("Сервер запущен.");
}

void loop() {

    // Цикл WebSocket
    webSocket.loop();

    // Если прошло 5 секунд...
    if (millis() % 1000 == 0) {

        // Если данные были получены хотя бы один раз
        if (rdata[0] != 0) {

            Serial.println("Текущие данные от клиента: ");

            // Выводим данные в последовательный порт
            for (size_t i = 0; i < glen; i++) {
                Serial.println(rdata[i]);
            }
            Serial.println();
        }
    }

}