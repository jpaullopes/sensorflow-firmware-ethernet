#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include "pico/cyw43_arch.h"

/**
 * @brief Inicializa e conecta ao Wi-Fi
 * @return 0 em caso de sucesso, 1 em caso de erro
 */
int wifi_init(void);

/**
 * @brief Desconecta do Wi-Fi e limpa recursos
 */
void wifi_cleanup(void);

#endif // WIFI_MANAGER_H
