#ifndef WIRE_H
#define WIRE_H

#include "RVStarArduino.h"

#define BUFFER_LENGTH 32

#ifdef __cplusplus
class TwoWire {
    private:
        static uint8_t rxBuffer[];
        static uint8_t rxBufferIndex;
        static uint8_t rxBufferLength;
        static uint8_t txAddress;
        static uint8_t txBuffer[];
        static uint8_t txBufferIndex;
        static uint8_t txBufferLength;
    public:
        TwoWire();
        void begin(void);
        void begin(uint8_t address);
        void begin(int address);
        void end(void);
};

extern TwoWire Wire;
#endif

#endif // WIRE_H
