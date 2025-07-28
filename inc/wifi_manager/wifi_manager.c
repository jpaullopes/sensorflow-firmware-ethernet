#include "wifi_manager.h"
#include <stdio.h>
#include "pico/stdlib.h"
#include "lwip/netif.h"
#include "lwip/ip4_addr.h"

int wifi_init(void) {
    printf("[INFO] Inicializando Wi-Fi...\n");
    if (cyw43_arch_init()) {
        printf("[ERRO] Falha na inicializacao do chip Wi-Fi.\n");
        return 1;
    }
    
    cyw43_arch_enable_sta_mode();
    printf("[INFO] Conectando a rede \"%s\"...\n", WIFI_SSID);
    
    if (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, 30000)) {
        printf("[ERRO] Nao foi possivel conectar ao Wi-Fi.\n");
        return 1;
    }
    
    printf("[OK]   Wi-Fi conectado com sucesso!\n");
    printf("[INFO] Endereco IP: %s\n", ip4addr_ntoa(netif_ip4_addr(netif_default)));
    return 0;
}

void wifi_cleanup(void) {
    cyw43_arch_deinit();
    printf("[INFO] Wi-Fi desconectado e recursos limpos.\n");
}
