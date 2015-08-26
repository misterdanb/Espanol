# Espanol
A wrapper libary for the esp8266 keeping the WiFi and MQTT connection up under all circumstances

This wrapper depends on [knolleary's PubSubClient-Library](https://github.com/knolleary/pubsubclient), so make sure you have that installed.

NOTICE: Currently there is a bug in the PubSubClient-Library, where it won't compile until *PROGMEM* is removed from the *publish_P* method in *PubSubClient.h* and *PubSubClient.cpp*. Also note, that you then can't use this method anymore.

## Features
* Reestablishes all lost connections
* Automatically subscribes topice again when the broker has died for example

## Example

~~~ Arduino
#include <SPI.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Espanol.h>

char* ssid     = "somefunnyssid";
char* password = "someunsecurepassword";
char* broker   = "somecrazyhostname";
int port       = 1883;

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
