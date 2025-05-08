/*  DATAFLUX.ino
 *
 *  Panagiotis Prountzos 2025
 *
 */

#include <WiFi.h>
#include <esp_now.h>

#include "DATAFLUX.h"

LSM303 sensor(true, false);
uint8_t masterMAC[] = {0x54, 0x32, 0x04, 0x3d, 0x5c, 0xf4};
uint32_t selfID = 4;
bool ESPNOWNeedsInit = true;
bool sensorIsInactive = true;

fvec3 magReading;
message msg;

esp_now_peer_info_t masterInfo;

void setup() {
    Serial.begin(115200);
    initSlave();

    msg.id = selfID;

    while (ESPNOWNeedsInit) {
        ESPNOWNeedsInit = !initESPNOW(masterInfo, masterMAC);
        if (ESPNOWNeedsInit)
            indicateError(ESP_NOW_INIT_ERROR);
        else
            break;
    }

    while (sensorIsInactive) {
        sensorIsInactive = !initSensor(sensor);
        if (sensorIsInactive)
            indicateError(SENSOR_INIT_ERROR);
        else
            break;
    }
}

void loop() {
    if (!sensor.checkI2CCommunication()) {
        indicateError(SENSOR_I2C_ERROR);
        return;
    }
    sensor.readMag(magReading);

    msg.x = magReading.x;
    msg.y = magReading.y;
    msg.z = magReading.z;

    publishMessage(&msg, masterMAC);
    Serial.println(magReading.x);
    Serial.println(magReading.y);
    Serial.println(magReading.z);

    // sleep
    deepSleep(1000);
}
