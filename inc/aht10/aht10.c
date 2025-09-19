#include "aht10.h"
#include <stdio.h>

// --- Definições Privadas do Módulo AHT10 ---
const uint8_t SENSOR_ADDR = 0x38;
const uint8_t AHT10_CMD_CALIBRATE = 0xE1;
const uint8_t AHT10_CMD_TRIGGER   = 0xAC;
const uint8_t AHT10_CMD_SOFTRESET = 0xBA;

#define I2C1_SDA_PIN 2
#define I2C1_SCL_PIN 3

// --- Funções Públicas ---

void aht10_init() {
    printf("Configurando Sensor de Umidade AHT10 na I2C 1...\n");
    // Inicializa a instância i2c1
    i2c_init(i2c1, 100 * 1000);
    gpio_set_function(I2C1_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(I2C1_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(I2C1_SDA_PIN);
    gpio_pull_up(I2C1_SCL_PIN);
    

    // Envia um comando de Soft Reset
    uint8_t reset_cmd = AHT10_CMD_SOFTRESET;
    i2c_write_blocking(i2c1, SENSOR_ADDR, &reset_cmd, 1, false);
    sleep_ms(20);

    // Envia comando de calibração
    uint8_t calibrate_cmd[] = {AHT10_CMD_CALIBRATE, 0x08, 0x00};
    i2c_write_blocking(i2c1, SENSOR_ADDR, calibrate_cmd, 3, false);
    sleep_ms(400);
    printf("Sensor AHT10 pronto.\n");
}
