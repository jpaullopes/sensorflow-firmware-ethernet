#include "bmp280.h"
#include <stdio.h>
#include <string.h>

// --- Definições Privadas do Módulo ---
const uint8_t BMP280_SENSOR_ADDR = 0x76;
const uint8_t REG_CALIB_DATA = 0x88;
const uint8_t REG_CTRL_MEAS = 0xF4;
const uint8_t REG_PRESS_MSB = 0xF7;

#define I2C_SDA_PIN 0
#define I2C_SCL_PIN 1

// Estrutura privada para guardar os dados de calibração
static struct {
    uint16_t dig_T1;
    int16_t  dig_T2, dig_T3;
    uint16_t dig_P1;
    int16_t  dig_P2, dig_P3, dig_P4, dig_P5, dig_P6, dig_P7, dig_P8, dig_P9;
    int32_t t_fine;
} calib_data;

// --- Funções Auxiliares Privadas (static) ---

static void read_calibration_data() {
    uint8_t buffer[24];
    uint8_t start_reg = REG_CALIB_DATA;
    i2c_write_blocking(i2c0, BMP280_SENSOR_ADDR, &start_reg, 1, true);
    i2c_read_blocking(i2c0, BMP280_SENSOR_ADDR, buffer, 24, false);

    calib_data.dig_T1 = (buffer[1] << 8) | buffer[0];
    calib_data.dig_T2 = (buffer[3] << 8) | buffer[2];
    calib_data.dig_T3 = (buffer[5] << 8) | buffer[4];
    calib_data.dig_P1 = (buffer[7] << 8) | buffer[6];
    calib_data.dig_P2 = (buffer[9] << 8) | buffer[8];
    calib_data.dig_P3 = (buffer[11] << 8) | buffer[10];
    calib_data.dig_P4 = (buffer[13] << 8) | buffer[12];
    calib_data.dig_P5 = (buffer[15] << 8) | buffer[14];
    calib_data.dig_P6 = (buffer[17] << 8) | buffer[16];
    calib_data.dig_P7 = (buffer[19] << 8) | buffer[18];
    calib_data.dig_P8 = (buffer[21] << 8) | buffer[20];
    calib_data.dig_P9 = (buffer[23] << 8) | buffer[22];
}

static float compensate_temp(int32_t adc_T) {
    int32_t var1, var2;
    var1 = ((((adc_T >> 3) - ((int32_t) calib_data.dig_T1 << 1))) * ((int32_t) calib_data.dig_T2)) >> 11;
    var2 = (((((adc_T >> 4) - ((int32_t) calib_data.dig_T1)) * ((adc_T >> 4) - ((int32_t) calib_data.dig_T1))) >> 12) * ((int32_t) calib_data.dig_T3)) >> 14;
    calib_data.t_fine = var1 + var2;
    return (calib_data.t_fine * 5 + 128) >> 8;
}

static float compensate_pressure(int32_t adc_P) {
    int64_t var1, var2, p;
    var1 = ((int64_t) calib_data.t_fine) - 128000;
    var2 = var1 * var1 * (int64_t) calib_data.dig_P6;
    var2 = var2 + ((var1 * (int64_t) calib_data.dig_P5) << 17);
    var2 = var2 + (((int64_t) calib_data.dig_P4) << 35);
    var1 = ((var1 * var1 * (int64_t) calib_data.dig_P3) >> 8) + ((var1 * (int64_t) calib_data.dig_P2) << 12);
    var1 = (((((int64_t) 1) << 47) + var1)) * ((int64_t) calib_data.dig_P1) >> 33;
    if (var1 == 0) return 0;
    p = 1048576 - adc_P;
    p = (((p << 31) - var2) * 3125) / var1;
    var1 = (((int64_t) calib_data.dig_P9) * (p >> 13) * (p >> 13)) >> 25;
    var2 = (((int64_t) calib_data.dig_P8) * p) >> 19;
    p = ((p + var1 + var2) >> 8) + (((int64_t) calib_data.dig_P7) << 4);
    return (float) p / 256.0;
}

// --- Funções Públicas ---

void bmp280_init() {
    printf("Configurando Sensor de Pressao BMP280...\n");
    i2c_init(i2c0, 400 * 1000);
    gpio_set_function(I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA_PIN);
    gpio_pull_up(I2C_SCL_PIN);

    read_calibration_data();

    uint8_t config_cmd[] = {REG_CTRL_MEAS, 0x93};
    i2c_write_blocking(i2c0, BMP280_SENSOR_ADDR, config_cmd, 2, false);
    printf("Sensor BMP280 pronto.\n");
}

void bmp280_read(bmp280_data_t *data) {
    uint8_t buffer[6];
    uint8_t start_reg = REG_PRESS_MSB;
    i2c_write_blocking(i2c0, BMP280_SENSOR_ADDR, &start_reg, 1, true);
    i2c_read_blocking(i2c0, BMP280_SENSOR_ADDR, buffer, 6, false);

    int32_t adc_P = (buffer[0] << 12) | (buffer[1] << 4) | (buffer[2] >> 4);
    int32_t adc_T = (buffer[3] << 12) | (buffer[4] << 4) | (buffer[5] >> 4);

    data->temperature_c = compensate_temp(adc_T) / 100.0f;
    data->pressure_hpa = compensate_pressure(adc_P) / 100.0f;
}
