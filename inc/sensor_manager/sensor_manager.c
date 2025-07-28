#include "sensor_manager.h"
#include <stdio.h>
#include "../bmp280/bmp280.h"
#include "../aht10/aht10.h"

int sensors_init(void) {
    printf("[INFO] Inicializando sensores...\n");
    
    bmp280_init();
    printf("[OK]   Sensor BMP280 inicializado.\n");
    
    aht10_init();
    printf("[OK]   Sensor AHT10 inicializado.\n");
    
    printf("[OK]   Sensores inicializados com sucesso.\n");
    return 0;
}

int sensors_read_all(sensors_reading_t* reading) {
    if (!reading) {
        printf("[ERRO] Ponteiro para leitura dos sensores e nulo.\n");
        return 1;
    }
    
    // Lê dados do BMP280 (temperatura e pressão)
    bmp280_data_t bmp_data;
    bmp280_read(&bmp_data);
    
    // Lê dados do AHT10 (umidade)
    float humidity = aht10_read_humidity();
    if (humidity < 0) {
        printf("[ERRO] Falha na leitura do sensor AHT10.\n");
        return 1;
    }
    
    // Preenche a estrutura de retorno
    reading->temperature_c = bmp_data.temperature_c;
    reading->pressure_hpa = bmp_data.pressure_hpa;
    reading->humidity_percent = humidity;
    
    printf("[DADOS] Leitura dos sensores -> Temp: %.2f C | Hum: %.2f %% | Pres: %.2f hPa\n", 
           reading->temperature_c, 
           reading->humidity_percent,
           reading->pressure_hpa);
    
    return 0;
}
