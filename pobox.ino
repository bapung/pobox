#include <ESP8266WiFi.h>
#include <UniversalTelegramBot.h>
#include <WiFiClientSecure.h>

#include "wifi.h"
#include "telegram.h"


const unsigned long WAIT_TIME = 20e3;

char ssid[] = "THEEEK";
char password[] = "Kuda@liar!1";
long deepsleepMin = 2e9;

String chatID = "";
String botToken = "";

X509List cert(TELEGRAM_CERTIFICATE_ROOT);

void erraticMode() {
  const int LED1 = 2;
  unsigned long stop = millis() + 5000;

  pinMode(LED1, OUTPUT);

  while (millis() < stop) {
    digitalWrite(LED1, LOW);
    delay(50);
    digitalWrite(LED1, HIGH);
    delay(50);
  }
}

void waitUserResponse() {
  delay(WAIT_TIME);
}

void setup() {
  Serial.begin(115200);

  if (!WNetwork.Connect(ssid, password)) {

    configTime(0, 0, "pool.ntp.org");

    TBot.Init(botToken, chatID, &cert);
    delay(500);
    TBot.NotifyButtonPressed();

    waitUserResponse();

    TBot.PollNewMessages();
    if (TBot.IsOpenAuthorized()) {
      erraticMode();
    }
  }

  ESP.deepSleep(deepsleepMin);
}

void loop() {}
