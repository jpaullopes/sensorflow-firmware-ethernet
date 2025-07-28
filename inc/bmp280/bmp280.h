#ifndef BMP280_H
#define BMP280_H

#include "pico/stdlib.h"
#include "hardware/i2c.h"

// Estrutura para guardar os dados finais de temperatura e pressão
typedef struct {
    float temperature_c;
    float pressure_hpa;
} bmp280_data_t;

/**
 * @brief Inicializa o barramento I2C e o sensor BMP280.
 * Lê os dados de calibração e configura o sensor para medição contínua.
 * Deve ser chamada uma vez no início do programa.
 */
void bmp280_init();

/**
 * @brief Lê os dados brutos do sensor e preenche a estrutura com os valores compensados.
 * @param data Ponteiro para a estrutura bmp280_data_t onde os dados serão armazenados.
 */
void bmp280_read(bmp280_data_t *data);

#endif // BMP280_H