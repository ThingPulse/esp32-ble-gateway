#define WIFI_SSID "yourssid"
#define WIFI_PASSWORD "yourpassw0rd"

#define MQTT_HOST IPAddress(192, 168, 0, 100)
#define MQTT_PORT 1883
#define MQTT_TOPIC "ESPBLE/1"

#define GATEWAY_RXD2 14
#define GATEWAY_TXD2 15

#define ESP1_LED_PIN 32
#define ESP2_LED_PIN 32
#define LED_COUNT   2
#define CHANNEL     0


#define BAUDRATE 115200


//Structure example to send data
//Must match the receiver structure
typedef struct EspMessage {
    uint16_t counter;
    uint16_t lastCycleDuration;
    uint16_t errorCount;
    char payload[200];
} EspMessage;