/*
 Espanol.h - A esp8266 arduino library establishing a
             wifi and mqtt connection and keeping it up.
  danb
  http://hasi.it
*/

#ifndef Espanol_h
#define Espanol_h

#include <Arduino.h>
#include <stdint.h>
#include <ESP8266WiFi.h>
#include "utility/PubSubClient.h"

class Espanol
{
public:
    Espanol(char*, char*, char*, int, void (*)(char*, uint8_t*, unsigned int));

    void setDebug(boolean);
    boolean getDebug();

    void loop();
    boolean connected();

    void subscribe(char*);
    void unsubscribe(char*);
    boolean publish(char *, char *);
    boolean publish(char *, uint8_t *, unsigned int);
    boolean publish(char *, uint8_t *, unsigned int, boolean);


private:
    void connectWiFi();
    void connectMQTT();

private:
    boolean _debug;
    char* _ssid;
    char* _password;
    char* _broker;
    int _port;
    char* _topics[20];
    WiFiClient _client;
    PubSubClient _mqtt;
};

#endif
