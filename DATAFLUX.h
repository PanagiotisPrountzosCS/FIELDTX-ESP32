/*  DATAFLUX.h
 *
 *  Panagiotis Prountzos 2025
 *
 */

#ifndef _DATAFLUX_H
#define _DATAFLUX_H

#define LEDPIN 15
#define ERRORSLEEPDURATIONMS 2500
#define NORMALSLEEPDURATIONMS 1000

typedef struct {
    float x;
    float y;
    float z;
    uint32_t id;
} message;

enum ErrorCodes {};

void handleError(uint8_t errorCode) {
    pinMode(LEDPIN, OUTPUT);
    for (uint8_t i = 0; i < errorCode; i++) {
        digitalWrite(LEDPIN, HIGH);
        delay(200);
        digitalWrite(LEDPIN, LOW);
        delay(200);
    }
}

void initEspNow() {
    if (esp_now_init() != ESP_OK) {
        handleError(1);
        //deepSleep(ERRORSLEEPDURATIONMS);
    }
}

#endif