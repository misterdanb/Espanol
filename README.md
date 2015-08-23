# Espanol
A wrapper libary keeping the WiFi and MQTT connection up under all circumstances

## Features
* Reestablishes all lost connections
* Automatically subscribes topice again when the broker has died for example

## Example

~~~ Arduino
#include <SPI.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Espanol.h>

char* ssid            = "HaSi-Kein-Internet-Legacy";
char* password        = "bugsbunny";
char* broker          = "10.23.42.210";
int port              = 1883;

Espanol denada(ssid, password, broker, port, callback);

void callback(char* topic, byte* payload, unsigned int length)
{
    String msg = topic;
    msg += " - ";
    msg += (char*) payload;
    
    Serial.println(msg);
}

void setup()
{
    Serial.begin(115200);

    denada.setDebug(true);
    denada.subscribe("foo/bar/#");
}

void loop()
{
    denada.loop();
}
~~~
