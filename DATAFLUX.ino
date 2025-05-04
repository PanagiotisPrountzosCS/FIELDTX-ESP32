/*  DATAFLUX.ino
 *
 *  Panagiotis Prountzos 2025
 *
 *  MASTER NODE - THIS BRANCH HOLDS THE FIRMWARE FOR THE MASTER ESP32
 *
 */

#include <WiFi.h>
#include <esp_now.h>

#include "DATAFLUX.h"

#define TICK_PERIOD_MS 50
#define MESSAGE_QUEUE_FLUSH_INTERVAL_MS 200
#define MESSAGE_QUEUE_FLUSH_SIZE 20
#define MESSAGE_QUEUE_MAX_SIZE 50

QueueHandle_t dataQueue;
uint32_t lastClear;

void onReceive(const esp_now_recv_info_t *info, const uint8_t *data, int len) {
    if (len != sizeof(message)) {
        Serial.println("Received invalid message size");
        return;
    }

    message msg;

    memcpy(&msg, data, sizeof(message));

    xQueueSend(dataQueue, &msg, portMAX_DELAY);
}

void setup() {
    Serial.begin(115200);
    while (!Serial) {
        delay(10);
    }
    WiFi.mode(WIFI_STA);

    dataQueue = xQueueCreate(MESSAGE_QUEUE_MAX_SIZE, sizeof(message));

    initEspNow();

    esp_now_register_recv_cb(onReceive);
    lastClear = millis();
}

void loop() {
    if (millis() - lastClear > MESSAGE_QUEUE_FLUSH_INTERVAL_MS ||
        uxQueueMessagesWaiting(dataQueue) >= MESSAGE_QUEUE_FLUSH_SIZE) {
        message msg;
        //generate payload
        Serial.println("================Data dump================");
        while (xQueueReceive(dataQueue, &msg, 0) == pdPASS) {
            Serial.println("MESSAGE");
            Serial.println(msg.x);
            Serial.println(msg.y);
            Serial.println(msg.z);
            Serial.println(msg.id);
        }
        //transmit payload
        Serial.println("================Data dump================");
        lastClear = millis();
    }
    delay(TICK_PERIOD_MS);
}
