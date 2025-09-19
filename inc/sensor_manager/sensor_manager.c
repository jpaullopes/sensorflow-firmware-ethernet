#include "sensor_manager.h"
#include <stdio.h>
#include "../bmp280/bmp280.h"
#include "../aht10/aht10.h"
#include "../concentration/concentration.h"
#include "../flow/flow.h"

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
    
    reading->temperature_c = bmp_data.temperature_c;
    reading->pressure_hpa = bmp_data.pressure_hpa; 
    reading->concentration_percent = concentration_value();
    reading->flow_liter = flow_value();
    
    printf("[DADOS] Leitura dos sensores -> Temp: %.2f C | Pres: %.2f hPa | Conc: %.2f | Vaz: %.2f\n", 
           reading->temperature_c, 
           reading->pressure_hpa,
           reading->concentration_percent,
           reading->flow_liter
        );
    
    return 0;
}
