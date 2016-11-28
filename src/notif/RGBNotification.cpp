#include "RGBNotification.h"



RGBNotification::RGBNotification(uint8_t r, uint8_t g, uint8_t b) {
        this->r = r;
        this->g = g;
        this->b = b;
        pinMode(r, OUTPUT);
        pinMode(g, OUTPUT);
        pinMode(b, OUTPUT);
};
RGBNotification::~RGBNotification() {
};
void RGBNotification::ready() {
        this->rgb(false, true, false);
};
void RGBNotification::off() {
        this->rgb(false, false, false);
};
void RGBNotification::error() {
        this->rgb(true, false, false);
};
void RGBNotification::processing() {
        this->rgb(false, false, true);
};
void RGBNotification::loop() {
};
void RGBNotification::setup() {
        this->rgb(false, false, false);
};
void RGBNotification::rgb(bool r, bool g, bool b) {
        digitalWrite(this->r, r ? HIGH : LOW);
        digitalWrite(this->g, g ? HIGH : LOW);
        digitalWrite(this->b, b ? HIGH : LOW);
};
