#include "RVStarArduino.h"

int main(void) {
    pinMode(A0, OUTPUT);

    while (1) {
       uint16_t value = analogRead(A0);
       printf("value: %d\n", value);
       delay(1000);
    }
}
