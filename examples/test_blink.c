#include "RVStarArduino.h"

int main(void) {
    pinMode(13, OUTPUT);

    while (1) {
        digitalWrite(13, HIGH);
        delay(1000);
        digitalWrite(13, LOW);
        delay(1000);
    }
}
