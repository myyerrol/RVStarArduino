#include "Wire.h"

TwoWire::TwoWire() {
}

void TwoWire::begin(void) {

}

void TwoWire::begin(uint8_t address) {

}

void TwoWire::begin(int address) {
    begin((uint8_t)address);
}

