#include <ESP8266WiFi.h>
#include <Espanol.h>

char* ssid     = "somefunnyssid";
char* password = "someunsecurepassword";
char* broker   = "somecrazyhostname";
int port       = 1883;

void setup()
{
    Serial.begin(115200);
    Espanol.begin(ssid, password, broker, port);

    Espanol.setDebug(true);
    Espanol.subscribe("foo/bar/#");

    Espanol.setCallback([](char *topic, byte *payload, unsigned int length) {
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
