#include "Wire.h"

TwoWire Wire;

// uint8_t TwoWire::rxBuffer[BUFFER_LENGTH];
// uint8_t TwoWire::rxBufferIndex = 0;
// uint8_t TwoWire::rxBufferLength = 0;

// uint8_t TwoWire::txAddress = 0;
// uint8_t TwoWire::txBuffer[BUFFER_LENGTH];
// uint8_t TwoWire::txBufferIndex = 0;
// uint8_t TwoWire::txBufferLength = 0;

TwoWire::TwoWire() {

}

void TwoWire::begin(void) {
    rcu_periph_clock_enable(RCU_GPIOC);
    rcu_periph_clock_enable(RCU_I2C0);

    gpio_init(GPIOC, GPIO_MODE_AF_OD, GPIO_OSPEED_50MHZ, GPIO_PIN_4 |
                                                         GPIO_PIN_5);

    /* I2C clock configure */
    i2c_clock_config(I2C0, 1000000, I2C_DTCY_16_9);
    /* I2C address configure */
    // i2c_mode_addr_config(I2C0, I2C_I2CMODE_ENABLE, I2C_ADDFORMAT_7BITS, I2C0_SLAVE_ADDRESS7);
    /* enable I2C0 */
    i2c_enable(I2C0);
    /* enable acknowledge */
    i2c_ack_config(I2C0, I2C_ACK_ENABLE);
}

void TwoWire::begin(uint8_t address) {
    begin();
    i2c_mode_addr_config(I2C0, I2C_I2CMODE_ENABLE, I2C_ADDFORMAT_7BITS, address);
}

void TwoWire::begin(int address) {
    begin((uint8_t)address);
}

