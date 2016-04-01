#include "Espanol.h"

_Espanol::_Espanol()
    : _debug(false),
      _topics{ NULL, },
      _client(),
      _mqtt()
{
}

void _Espanol::begin(const char *ssid, const char *password, const char *broker, const int port)
{
    _mqtt.setServer(_broker, _port);
    _mqtt.setCallback(internalCallback);
}

void _Espanol::end()
{
}

void _Espanol::setDebug(bool debug)
{
    _debug = debug;
}

bool _Espanol::getDebug()
{
    return _debug;
}

void _Espanol::loop()
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

bool _Espanol::connected()
{
    return (WiFi.status() == WL_CONNECTED) && _mqtt.connected();
}

void _Espanol::setCallback(std::function<void (char *, uint8_t *, unsigned int)> callback)
{
    _callback = callback;
    _mqtt.setCallback(internalCallback);
}

void _Espanol::subscribe(char *newTopic)
{
    for (char *topic : _topics)
    {
        if (topic != 0)
        {
            if (strcmp(topic, newTopic) == 0)
            {
                return;
            }
        }
    }

    for (char *&topic : _topics)
    {
        if (topic == 0)
        {
            topic = newTopic;
            _mqtt.subscribe(newTopic);

            return;
        }
    }
}

void _Espanol::unsubscribe(char *currentTopic)
{
    for (char *&topic : _topics)
    {
        if (strcmp(topic, currentTopic) == 0)
        {
            _mqtt.unsubscribe(currentTopic);
            topic = 0;

            return;
        }
    }
}

bool _Espanol::publish(char *topic, char *payload)
{
    return _mqtt.publish(topic, payload);
}

bool _Espanol::publish(char *topic, uint8_t* payload, unsigned int length)
{
    return _mqtt.publish(topic, payload, length);
}

bool _Espanol::publish(char *topic, uint8_t* payload, unsigned int length, bool retained)
{
    return _mqtt.publish(topic, payload, length, retained);
}

void _Espanol::connectWiFi()
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

void _Espanol::connectMQTT()
{
    String clientName;
    clientName += "esp8266-";
    clientName += String(micros() & 0xffff, 16);

    char *clientNameCharArray = new char[clientName.length() + 1];
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

    for (char *topic : _topics)
    {
        if (topic != 0)
        {
            _mqtt.subscribe(topic);
        }
    }
}

void _Espanol::internalCallback(char *topic, uint8_t *bytes, unsigned int length)
{
    Espanol._callback(topic, bytes, length);
}

_Espanol Espanol;
