// https://github.com/hellerchr/esp8266-websocketclient/tree/master

#include <Arduino.h>
#include "WebSocketClient.h"
#include "ESP8266WiFi.h"

WebSocketClient ws(true);

void setup() {
	Serial.begin(115200);
	//WiFi.begin("TP-Link_B394", "18009217");
  WiFi.begin("TP-Link_B394", "18009217");

	Serial.print("Connecting");
	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		Serial.print(".");
	}
}

void loop() {
	if (!ws.isConnected()) {
		ws.connect("echo.websocket.org", "/", 443);
	} else {
		ws.send("hello");

		String msg;
		if (ws.getMessage(msg)) {
			Serial.println(msg);
		}
	}
	delay(500);
}
