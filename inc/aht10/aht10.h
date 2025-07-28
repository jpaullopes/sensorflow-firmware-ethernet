#ifndef AHT10_H
#define AHT10_H

#include "pico/stdlib.h"
#include "hardware/i2c.h"

/**
 * @brief Inicializa o barramento I2C 1 e o sensor AHT10.
 * Realiza um soft reset e calibra o sensor.
 * Deve ser chamada uma vez no início do programa.
 */
void aht10_init();

/**
 * @brief Lê a humidade do sensor AHT10.
 * @return O valor da humidade relativa em percentagem (%).
 */
float aht10_read_humidity();

#endif // AHT10_H