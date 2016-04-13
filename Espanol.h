/*
 Espanol.h - A esp8266 arduino library establishing a
             wifi and mqtt connection and keeping it up.
  danb
  http://hasi.it
*/

#ifndef Espanol_h
#define Espanol_h

#include <functional>
#include <Arduino.h>
#include <stdint.h>
#include <ESP8266WiFi.h>
#include "utility/PubSubClient.h"

class _Espanol
{
public:
    _Espanol();

    void begin(String, String, String, String, int);
    void end();

    void setDebug(bool);
    bool getDebug();

    void loop();
    bool connected();

    void setCallback(std::function<void (char *, uint8_t *, unsigned int)>);

    void subscribe(String);
    void unsubscribe(String);
    bool publish(String, String);
    bool publish(String, uint8_t *, unsigned int);
    bool publish(String, uint8_t *, unsigned int, bool);

private:
    void connectWiFi();
    void connectMQTT();

    static void internalCallback(char *, uint8_t *, unsigned int);

public:
    std::function<void (char *, uint8_t *, unsigned int)> _callback;

private:
    bool _debug;
    String _ssid;
    String _password;
    String _hostname;
    String _broker;
    int _port;
    String _topics[10];
    WiFiClient _client;
    PubSubClient _mqtt;
};

extern _Espanol Espanol;

#endif
