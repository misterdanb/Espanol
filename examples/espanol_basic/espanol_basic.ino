#include <ESP8266WiFi.h>
#include <Espanol.h>

String ssid     = "somefunnyssid";
String password = "someunsecurepassword";
String broker   = "somecrazyhostname";
String hostname = "somecoolhostname";

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
