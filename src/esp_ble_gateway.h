/**
 * This sample code scans for BLE devices and forwards detected messages over
 * the serial line to the other ESP32 in a ThingPulse ESP32  Gateway.
 **/

#include <Arduino.h>
#include <WiFi.h>
#include <FastLED.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include "settings.h"

EspMessage message;

CRGB leds[LED_COUNT];

int scanTime = 2; //In seconds
BLEScan* pBLEScan;

QueueHandle_t queue;

void sendDebugMessage() {
  message.counter++;
  message.lastCycleDuration++;
  const char* payload = "{\"value\": \"yes\"}";
  strncpy(message.payload, payload, sizeof(message.payload));
  Serial2.write((uint8_t *) &message, sizeof(message));
  Serial.println("Message transmitted");
}


void forwarder_task(void *pvParameter)
{
    EspMessage localMessage;
    if(queue == NULL){
        printf("Queue is not ready");
        return;
    }
    while(1){
        // if the queue is not empty consume the next message
        if (xQueueReceive(queue,&localMessage,(TickType_t )(100/portTICK_PERIOD_MS)) == pdTRUE) {; 
          Serial2.write((uint8_t *) &localMessage, sizeof(localMessage));
        }
    }
}

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice) {
      message.counter++;
      message.lastCycleDuration++;
      String json = "{\"rssi\": \"" + String(advertisedDevice.getRSSI()) + "\","
        +"\"mac\": \"" + String(advertisedDevice.getAddress().toString().c_str()) + "\""
        +"}";

      strncpy(message.payload, json.c_str(), sizeof(message.payload));

      // place the message in the RTOS queue to be processed by the
      // forwarder task. Please note: if the forwarder cannot consume
      // messages in time this code will throw away additional messages.
      xQueueSend(queue, &message, 0);
      Serial.printf("Advertised Device: %s \n", advertisedDevice.toString().c_str());
      //sendDebugMessage();
    }
};





void showStatus(CRGB led1, CRGB led2) {
    FastLED.clear();
    leds[0] = led1;
    leds[1] = led2;
    FastLED.show();
}

// Init ESP BLE Scanner
void initBleScanner() {
  BLEDevice::init("");
  pBLEScan = BLEDevice::getScan(); //create new scan
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks(), true);
  pBLEScan->setActiveScan(false); //active scan uses more power of the node, but get results faster
  pBLEScan->setInterval(100);
  pBLEScan->setWindow(99);  // less or equal setInterval value
}



void setup() {
  Serial.begin(115200);
  FastLED.addLeds<WS2812B, ESP1_LED_PIN, GRB>(leds, LED_COUNT);
  FastLED.setBrightness(255);
  WiFi.disconnect(true);  // Disconnect from the network
  WiFi.mode(WIFI_OFF);    // Switch WiFi off  
  queue = xQueueCreate( 50, sizeof( EspMessage ) );

  showStatus(CRGB::White, CRGB::White);

  Serial2.begin(BAUDRATE, SERIAL_8N1, GATEWAY_RXD2, GATEWAY_TXD2);
  // Setup a queue to process messages. This is necessary
  // to decouple the thread that scans from the thread which forwards the message
  xTaskCreate(&forwarder_task,"forwarder_task",2048,NULL,5,NULL);

  initBleScanner();

}



void loop() {
  //sendDebugMessage();
  showStatus(CRGB::Red, CRGB::White);
  pBLEScan->start(scanTime, false);
  showStatus(CRGB::White, CRGB::White);
  pBLEScan->clearResults();   // delete results fromBLEScan buffer to release memory
  delay(1000);
}
