// transmitter_bare_minimum.ino
// Wolfgang Ewald [https://wolles-elektronikkiste.de/en/esp-now]

/*
Реализация ESP-NOW на ESP8266 не сильно отличается от реализации на ESP32. 
Однако разница достаточно велика, чтобы унифицированные скетчи (с помощью 
аннотаций или конструкций #ifdef ESP32...#else ) вызывали путаницу.

Вот основные различия:

- Названия подключаемых библиотек.
- Параметры функций обратного вызова.
- Для ESP82866 его роль в качестве приемника, передатчика или приемопередатчика 
  должна быть явно определена с помощью esp_now_set_self_role().
- Возвращаемые значения некоторых функций.
- Первый параметр функции, зарегистрированной с помощью esp_now_register_recv_cb() 
  (здесь: messageReceived()), — это uint8_t* macAddr, а не const esp_now_recv_info* info.
*/

//#include <esp_now.h>
//#include <WiFi.h>

#include <espnow.h>
#include "ESP8266WiFi.h"

/*
[7] ESP32-2432S028 (CYD)         MAC-Address: B8:D6:1A:5C:F9:54
                  Formated: {0xB8, 0xD6, 0x1A, 0x5C, 0xF9, 0x54} 
*/
uint8_t receiverAddress[] = {0xB8, 0xD6, 0x1A, 0x5C, 0xF9, 0x54};
//esp_now_peer_info_t peerInfo;

//uint8_t receiverAddress[] = {0xA4, 0xCF, 0x12, 0xDF, 0x5D, 0x89};

/*
#include <espnow.h>
#include "ESP8266WiFi.h"

uint8_t receiverAddress[] = {0xA4, 0xCF, 0x12, 0xDF, 0x5D, 0x89};

typedef struct messageToBeSent {
    char text[64];
    int intVal;
    float floatVal;
} messageToBeSent;

typedef struct receivedMessage {
    char text[64];
    long runTime;
} receivedMessage;

messageToBeSent myMessageToBeSent; 
receivedMessage myReceivedMessage; 

void messageSent(uint8_t *macAddr, uint8_t status) {
    Serial.print("Send status: ");
    if(status == 0){
        Serial.println("Success");
    }
    else{
        Serial.println("Error");
    }
}

void messageReceived(uint8_t* macAddr, uint8_t* incomingData, uint8_t len){
    memcpy(&myReceivedMessage, incomingData, sizeof(myReceivedMessage));
    Serial.printf("Incoming Message from: %02X:%02X:%02X:%02X:%02X:%02X \n\r", 
            macAddr[0], macAddr[1], macAddr[2], macAddr[3], macAddr[4], macAddr[5]);    
    Serial.print("Message: ");
    Serial.println(myReceivedMessage.text);
    Serial.print("RunTime [s]: ");
    Serial.println(myReceivedMessage.runTime);
    Serial.println();
}

void setup()
{
    Serial.begin(115200);
    // delay(1000); // uncomment if your serial monitor is empty
    WiFi.mode(WIFI_STA);
    
    if (esp_now_init() == 0) {
        Serial.println("ESPNow Init success");
    }
    else {
        Serial.println("ESPNow Init fail");
        return;
    }

    esp_now_set_self_role(ESP_NOW_ROLE_COMBO);
    uint8_t result = esp_now_add_peer(receiverAddress, ESP_NOW_ROLE_COMBO, 0, NULL, 0);
    if(result != 0){
        Serial.println("Failed to add peer");
    }
    
    esp_now_register_send_cb(messageSent);  
    esp_now_register_recv_cb(messageReceived); 

   
}
 
void loop(){
    char textMsg[] = "Hi, here's my data for you: ";
    memcpy(&myMessageToBeSent.text, textMsg, sizeof(textMsg));
    myMessageToBeSent.intVal = 4242;
    myMessageToBeSent.floatVal = 42.42;
    esp_now_send(receiverAddress, (uint8_t *) &myMessageToBeSent, sizeof(myMessageToBeSent));
    delay(5000);
}
*/


void setup()
{
    Serial.begin(115200);
    // delay(1000); // uncomment if your serial monitor is empty
    WiFi.mode(WIFI_STA);
    
    //esp_now_init();
    if (esp_now_init() == 0) 
    {
        Serial.println("ESPNow Init success");
    }
    else 
    {
        Serial.println("ESPNow Init fail");
        return;
    }


    esp_now_set_self_role(ESP_NOW_ROLE_COMBO);
    uint8_t result = esp_now_add_peer(receiverAddress, ESP_NOW_ROLE_COMBO, 0, NULL, 0);
    if(result != 0)
    {
        Serial.println("Failed to add peer");
    }
    
    esp_now_register_send_cb(messageSent);  
   
    /*
    memcpy(peerInfo.peer_addr, receiverAddress, 6);
    peerInfo.channel = 0;
    peerInfo.encrypt = false;
    esp_now_add_peer(&peerInfo);
    */
}
 
void loop()
{
/*
    char message[] = "Hi, this is a message from the transmitting ESP";
    esp_now_send(receiverAddress, (uint8_t *) message, sizeof(message)-1); // -1 to not send the NULL terminator
    Serial.println(message);
*/
    delay(5000);
}
