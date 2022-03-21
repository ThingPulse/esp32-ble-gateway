# ThingPulse ESPGateway

The ThingPulse ESPGateway is a versatile device that comes with two ESP32-WROVER-IB modules.
The two ESP32s are connected by two lines used for inter-ESP32 communication over Serial
or other protocols. Each of the ESP32 can also control two WS2812B RGB Leds at the front 
of the device. 

The ESPGateway is available for purchase at [ThingPulse ESPGateway](https://thingpulse.com/product/espgateway/)

![Gateway](/resources/GatewayWithAntenna.jpg)

## The Project

This project shows how the gateway can be used to track BLE devices. One ESP32 is scanning for
BLE devices and forwards messages with device information over the serial line to the other ESP32.
That other ESP32 is connected over WiFi to a MQTT broker, to where it will send the device messages.

![Architecture](/resources/Architecture.png)


## Building the project

This project contains actually code for two different ESP32 modules. Control which firmware
shall be flashed by selecting the right target in platformio.ini:

```
;default_envs = esp32-wifi-client
default_envs = esp32-ble-gateway
````
for instance will build and flash the ESP32 module scanning for BLE devices.

