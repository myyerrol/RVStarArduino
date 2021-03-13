#ifndef WIRE_H
#define WIRE_H

#include "RVStarArduino.h"

#define BUFFER_LENGTH 32

class TwoWire {
    public:
        TwoWire();
        void begin(void);
        void begin(uint8_t address);
        void begin(int address);
        void end(void);
    private:
        // std::map<uint8_t, uint8_t> a;
        // static uint8_t rxBuffer[];
        // static uint8_t rxBufferIndex;
        // static uint8_t rxBufferLength;

        // static uint8_t txAddress;
        // static uint8_t txBuffer[];
        // static uint8_t txBufferIndex;
        // static uint8_t txBufferLength;
};

extern TwoWire Wire;

#endif // WIRE_H
