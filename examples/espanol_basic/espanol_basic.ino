#include <SPI.h>
#include <ESP8266WiFi.h>
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
