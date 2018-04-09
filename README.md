# Espanol
A wrapper libary for the esp8266 keeping the WiFi and MQTT connection up under all circumstances

This wrapper comes with [knolleary's PubSubClient-Library](https://github.com/knolleary/pubsubclient) preinstalled in form of a [git-subdir](https://github.com/andreyvit/git-subdir).

NOTICE: Currently there is a bug in the PubSubClient-Library, where it won't compile for an ESP8266 until *PROGMEM* is removed from the *publish_P* method in *PubSubClient.h* and *PubSubClient.cpp*.

## Features
* Reestablishes all lost connections
* Automatically subscribes to the topic again when the broker has died for example

## Example

~~~ Arduino
#include <ESP8266WiFi.h>
#include <Espanol.h>

String ssid     = "somefunnyssid";
String password = "someunsecurepassword";
String broker   = "somecrazyhostname";
String hostname = "somecoolhostname"

int port        = 1883;

void setup()
{
    Serial.begin(115200);
    Espanol.begin(ssid, password, hostname, broker, port);

    Espanol.setDebug(true);
    Espanol.subscribe("foo/bar/#");

    Espanol.setCallback([](String topic, byte *payload, unsigned int length) {
        String msg = topic;
        msg += " - ";
        msg += (char*) payload;

        Serial.println(msg);
    });
}

void loop()
{
    Espanol.loop();
}
~~~
