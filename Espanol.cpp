#include "Espanol.h"

Espanol::Espanol(char* ssid, char* password, char* broker, int port, void (*callback)(char*, uint8_t*, unsigned int))
    : _debug(false),
      _ssid(ssid),
      _password(password),
      _broker(broker),
      _port(port),
      _topics{ 0, },
      _client(),
      _mqtt(broker, port, callback, _client)
{
}

void Espanol::setDebug(boolean debug)
{
    _debug = debug;
}

boolean Espanol::getDebug()
{
    return _debug;
}

void Espanol::loop()
{
    if (WiFi.status() != WL_CONNECTED)
    {
        connectWiFi();
    }

    if (!_mqtt.connected())
    {
        connectMQTT();
    }

    _mqtt.loop();
}

boolean Espanol::connected()
{
    return (WiFi.status() == WL_CONNECTED) && _mqtt.connected();
}

void Espanol::subscribe(char* newTopic)
{
    for (char* topic : _topics)
    {
        if (topic != 0)
        {
            if (strcmp(topic, newTopic) == 0)
            {
                return;
            }
        }
    }

    for (char*& topic : _topics)
    {
        if (topic == 0)
        {
            topic = newTopic;
            _mqtt.subscribe(newTopic);

            return;
        }
    }
}

void Espanol::unsubscribe(char* currentTopic)
{
    for (char*& topic : _topics)
    {
        if (strcmp(topic, currentTopic) == 0)
        {
            _mqtt.unsubscribe(currentTopic);
            topic = 0;

            return;
        }
    }
}

boolean Espanol::publish(char* topic, char* payload)
{
    return _mqtt.publish(topic, payload);
}

boolean Espanol::publish(char* topic, uint8_t* payload, unsigned int length)
{
    return _mqtt.publish(topic, payload, length);
}

boolean Espanol::publish(char* topic, uint8_t* payload, unsigned int length, boolean retained)
{
    return _mqtt.publish(topic, payload, length, retained);
}

void Espanol::connectWiFi()
{
    if (_debug)
    {
        Serial.print("Connecting to ");
        Serial.println(_ssid);
    }

    WiFi.begin(_ssid, _password);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);

        if (_debug)
        {
            Serial.print(".");
        }
    }

    if (_debug)
    {
        Serial.println();
        Serial.println("WiFi connected");
        Serial.println("IP address: ");
        Serial.println(WiFi.localIP());
    }
}

void Espanol::connectMQTT()
{
    String clientName;
    clientName += "esp8266-";
    clientName += String(micros() & 0xffff, 16);

    char* clientNameCharArray = new char[clientName.length() + 1];
    clientName.toCharArray(clientNameCharArray, clientName.length());

    if (_debug)
    {
        Serial.print("Connecting to broker");
    }

    while (!_mqtt.connect(clientNameCharArray))
    {
        delay(500);

        if (_debug)
        {
            Serial.print(".");
        }
    }

    if (_debug)
    {
        Serial.println();
        Serial.println("Connected to MQTT broker");
    }

    for (char* topic : _topics)
    {
        if (topic != 0)
        {
            _mqtt.subscribe(topic);
        }
    }
}
