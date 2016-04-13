#include "Espanol.h"

_Espanol::_Espanol()
    : _debug(false),
      _hostname("esp8266"),
      _topics{ "", },
      _client(),
      _mqtt(_client)
{
    for (int i = 0; i < 10; i++)
    {
        _topics[i] = "";
    }
}

void _Espanol::begin(String ssid, String password, String hostname, String broker, const int port)
{
    _ssid = ssid;
    _password = password;
    _hostname = hostname;
    _broker = broker;
    _port = port;

    WiFi.hostname(_hostname);
    WiFi.mode(WIFI_STA);

    _mqtt.setServer(_broker.c_str(), _port);
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
    connectWiFi();

    if (WiFi.status() == WL_CONNECTED)
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

void _Espanol::subscribe(String newTopic)
{
    for (String topic : _topics)
    {
        if (topic.equals(newTopic))
        {
            return;
        }
    }

    for (int i = 0; i < 10; i++)
    {
        if (_topics[i].equals(""))
        {
            _topics[i] = newTopic;

            if (_mqtt.connected())
            {
                _mqtt.subscribe(newTopic.c_str());
            }

            return;
        }
    }
}

void _Espanol::unsubscribe(String currentTopic)
{
    for (int i = 0; i < 10; i++)
    {
        if (_topics[i].equals(currentTopic))
        {
            _mqtt.unsubscribe(currentTopic.c_str());
            _topics[i] = "";

            return;
        }
    }
}

bool _Espanol::publish(String topic, String payload)
{
    return _mqtt.publish(topic.c_str(), payload.c_str());
}

bool _Espanol::publish(String topic, uint8_t *payload, unsigned int length)
{
    return _mqtt.publish(topic.c_str(), payload, length);
}

bool _Espanol::publish(String topic, uint8_t *payload, unsigned int length, bool retained)
{
    return _mqtt.publish(topic.c_str(), payload, length, retained);
}

void _Espanol::connectWiFi()
{
    static int lastConnectedState = -1;
    static bool connectingToWiFi = false;
    static int lastMillis = millis();

    if (lastConnectedState != WL_CONNECTED && !connectingToWiFi)
    {
        connectingToWiFi = true;

        if (_debug)
        {
            Serial.print("Connecting to ");
            Serial.print(_ssid);
            Serial.print(" with passphrase ");
            Serial.println(_password);
        }

        WiFi.begin(_ssid.c_str(), _password.c_str());
    }
    else
    {
        if (WiFi.status() != WL_CONNECTED)
        {
            if (_debug)
            {
                int newMillis = millis();

                if (newMillis - lastMillis > 5000)
                {
                    lastMillis = newMillis;
                    Serial.print(".");
                }
            }
        }
        else
        {
            connectingToWiFi = false;
        }
    }

    if (lastConnectedState != WL_CONNECTED && WiFi.status() == WL_CONNECTED)
    {
        if (_debug)
        {
            Serial.println();
            Serial.println("WiFi connected");
            Serial.println("IP address: ");
            Serial.println(WiFi.localIP());
        }
    }

    lastConnectedState = WiFi.status();
}

void _Espanol::connectMQTT()
{
    static int lastConnectedState = false;
    static bool connectingToMQTT = false;
    static int lastMillis = millis();

    if (!lastConnectedState && !connectingToMQTT)
    {
        connectingToMQTT = true;

        if (_debug)
        {
            Serial.print("Connecting to broker");
        }
    }
    else
    {
        if (!_mqtt.connected())
        {
            int newMillis = millis();

            if (newMillis - lastMillis > 1000)
            {
                bool succeeded = _mqtt.connect(_hostname.c_str());

                if (!succeeded && _debug)
                {
                    Serial.print(".");
                }

                if (succeeded)
                {
                    connectingToMQTT = false;
                }

                lastMillis = newMillis;
            }
        }
        else
        {
            connectingToMQTT = false;
        }
    }

    if (!lastConnectedState && _mqtt.connected())
    {
        for (String topic : _topics)
        {
            if (!topic.equals(""))
            {
                _mqtt.subscribe(topic.c_str());
            }
        }

        if (_debug)
        {
            Serial.println();
            Serial.println("Connected to MQTT broker");
        }
    }

    lastConnectedState = _mqtt.connected();
}

void _Espanol::internalCallback(char *topic, uint8_t *bytes, unsigned int length)
{
    Espanol._callback(topic, bytes, length);
}

_Espanol Espanol;
