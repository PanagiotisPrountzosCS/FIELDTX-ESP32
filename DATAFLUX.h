/*  DATAFLUX.h
 *
 *  Panagiotis Prountzos 2025
 *
 */

#ifndef _DATAFLUX_H
#define _DATAFLUX_H

#include "modules/ALSMD/ALSMD.h"

#define LED 15

enum errorCodes {
    ESP_NOW_INIT_ERROR = 0x01,
    ESP_NOW_TRANSMIT_ERROR,
    SENSOR_INIT_ERROR,
    SENSOR_I2C_ERROR
};

typedef struct message {
    float x;
    float y;
    float z;
    uint32_t id;
} message;

void initSlave() {
    Wire.begin();
    WiFi.mode(WIFI_STA);
}

bool initESPNOW(esp_now_peer_info_t& peerInfo, const uint8_t* receiver) {
    if (esp_now_init() != ESP_OK) return false;

    peerInfo.channel = 0;
    peerInfo.encrypt = false;
    // Register Receiver Device
    memcpy(peerInfo.peer_addr, receiver, 6);
    if (esp_now_add_peer(&peerInfo) != ESP_OK) return false;

    return true;
}

bool initSensor(LSM303& sensor) {
    sensor.configure(ACCEL_RATE_0HZ, MAG_RATE_3HZ, ACCEL_MODE_POWERDOWN, MAG_MODE_SINGLE,
                     ACCEL_SCALE_2G, MAG_SCALE_1_3);
    return sensor.init();
}

void indicateError(uint8_t errorCode) {
    pinMode(LED, OUTPUT);
    for (int i = 0; i < errorCode; i++) {
        digitalWrite(LED, HIGH);
        delay(200);
        digitalWrite(LED, LOW);
        delay(200);
    }
    delay(2000);
}

void publishMessage(const message* msg, const uint8_t* receiver) {
    esp_err_t result = esp_now_send(receiver, (uint8_t*)msg, sizeof(message));
    if (result != ESP_OK) indicateError(ESP_NOW_TRANSMIT_ERROR);
}

void deepSleep(uint32_t durationMS) { delay(durationMS); }

#endif