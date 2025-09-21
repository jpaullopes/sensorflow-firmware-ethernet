#ifndef TEMPERATURE_H
#define TEMPERATURE_H

#include "pico/stdlib.h"
#include "hardware/i2c.h"

/**
 * @brief Inicializa o barramento I2C 1 e o sensor AHT10.
 * Realiza um soft reset e calibra o sensor.
 * Deve ser chamada uma vez no início do programa.
 */
void aht10_init();

#endif // TEMPERATURE_H