#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"

// --- Constantes do Projeto ---
#define ADC_PIN 28                  // Pino GPIO conectado ao sensor (ADC2)
#define ADC_CHANNEL 2               // Canal ADC correspondente ao GPIO28
#define ADC_VREF 3.3f               // Tensão de referência do ADC
#define ADC_CONVERSION_FACTOR (ADC_VREF / (1 << 12)) // Fator de conversão para um ADC de 12 bits

#define RESISTOR_VALUE 100.0f       // Valor do resistor de carga em Ohms

// Mapeamento Sensor (Corrente -> Temperatura)
#define MIN_CURRENT 0.004f          // 4 mA
#define MAX_CURRENT 0.020f          // 20 mA
#define MIN_TEMP -50.0f
#define MAX_TEMP 200.0f

int main() {
    // Inicializa a E/S padrão (para printf via USB)
    stdio_init_all();

    // Inicializa o hardware do ADC
    adc_init();

    // Habilita o pino GPIO para a função de ADC
    adc_gpio_init(ADC_PIN);

    // Seleciona o canal do ADC para leitura
    adc_select_input(ADC_CHANNEL);

    printf("Iniciando leitura do sensor TA2532...\n");

    while (1) {
        // 1. Lê o valor bruto do ADC (0-4095)
        uint16_t raw_adc_value = adc_read();

        // 2. Converte o valor bruto para tensão
        float voltage = raw_adc_value * ADC_CONVERSION_FACTOR;

        // 3. Calcula a corrente usando a Lei de Ohm (I = V/R)
        float current = voltage / RESISTOR_VALUE;

        // 4. Mapeia a corrente para a faixa de temperatura
        // Fórmula de interpolação linear: y = y0 + (y1 - y0) * ((x - x0) / (x1 - x0))
        float temperature = MIN_TEMP + (MAX_TEMP - MIN_TEMP) * ((current - MIN_CURRENT) / (MAX_CURRENT - MIN_CURRENT));

        // Imprime os resultados
        printf("ADC Bruto: %d | Tensão: %.2f V | Corrente: %.2f mA | Temperatura: %.2f C\n", 
               raw_adc_value, 
               voltage, 
               current * 1000.0f, // Converte para mA para exibição
               temperature);

        sleep_ms(2000); // Aguarda 2 segundos antes da próxima leitura
    }

    return 0;
}