#include "RVStarArduino.h"

int main(void) {
    while (1) {
        for (int i = 0; i < 255; i++) {
            analogWrite(11, i);
            delay(10);
        }
    }
}