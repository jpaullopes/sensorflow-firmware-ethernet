#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "pico/unique_id.h"

#include "inc/ethernet_manager/ethernet_manager.h"
#include "inc/http_client/http_client.h"
#include "inc/sensor_manager/sensor_manager.h"

#define SEND_INTERVAL_MS 5000

int main() {
    stdio_init_all();
    sleep_ms(3000);

    char board_id_str[2 * PICO_UNIQUE_BOARD_ID_SIZE_BYTES + 1];
    pico_get_unique_board_id_string(board_id_str, sizeof(board_id_str));

    printf("\n====================================================\n");
    printf("=       MONITOR DE SENSOR | RASPBERRY PI PICO W5500  =\n");
    printf("= ID Unico da Placa: %s =\n", board_id_str);
    printf("=====================================================\n");

    // Configuração de rede Ethernet
    ethernet_config_t eth_config = {
        .mac = {ETHERNET_MAC_0, ETHERNET_MAC_1, ETHERNET_MAC_2, 
                ETHERNET_MAC_3, ETHERNET_MAC_4, ETHERNET_MAC_5},
        .ip = {DEVICE_IP_0, DEVICE_IP_1, DEVICE_IP_2, DEVICE_IP_3},
        .subnet = {SUBNET_MASK_0, SUBNET_MASK_1, SUBNET_MASK_2, SUBNET_MASK_3},
        .gateway = {GATEWAY_IP_0, GATEWAY_IP_1, GATEWAY_IP_2, GATEWAY_IP_3},
        .dns = {8, 8, 8, 8},  // Google DNS
        .dhcp = NETINFO_STATIC  // IP estático
    };

    if (ethernet_init(&eth_config) != 0) {
        printf("[ERRO] Falha na inicialização do Ethernet. Sistema interrompido.\n");
        while(1);
    }

    if (sensors_init() != 0) {
        printf("[ERRO] Falha na inicializacao dos sensores.\n");
        while(1);
    }

    printf("[INFO] Iniciando ciclos de envio a cada %d segundos.\n", SEND_INTERVAL_MS / 1000);
    
    while (1) {
        // Verifica status da conexão Ethernet
        ethernet_status_t eth_status = ethernet_get_status();
        if (eth_status != ETHERNET_CONNECTED) {
            printf("[AVISO] Ethernet desconectado. Tentando reconectar...\n");
            if (ethernet_restart() != 0) {
                printf("[ERRO] Falha na reconexão Ethernet. Aguardando...\n");
                sleep_ms(SEND_INTERVAL_MS);
                continue;
            }
        }

        sensors_reading_t sensor_data;
        if (sensors_read_all(&sensor_data) != 0) {
            printf("[ERRO] Falha na leitura dos sensores. Tentando novamente...\n");
            sleep_ms(SEND_INTERVAL_MS);
            continue;
        }

        // Prepara estrutura para envio HTTP
        sensor_data_t http_data = {
            .temperature = sensor_data.temperature_c,
            .humidity = sensor_data.humidity_percent,
            .pressure = sensor_data.pressure_hpa,
            .sensor_id = board_id_str
        };
        
        int result = http_send_sensor_data(&http_data);
        if (result < 0) {
            printf("[ERRO] Falha no envio dos dados.\n");
        }
        
        printf("\nAguardando %d segundos para o proximo envio...\n", SEND_INTERVAL_MS / 1000);
        sleep_ms(SEND_INTERVAL_MS);
    }
    return 0;
}
