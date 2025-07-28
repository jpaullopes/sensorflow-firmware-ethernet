# SensorFlow Firmware

![Linguagem](https://img.shields.io/badge/Linguagem-C-blue.svg)
![Plataforma](https://img.shields.io/badge/Plataforma-Raspberry%20Pi%20Pico%20W-green.svg)
![SDK](https://img.shields.io/badge/Pico%20SDK-2.1.1-orange.svg)
![Build](https://img.shields.io/badge/Build-Passing-brightgreen.svg)
![Licença](https://img.shields.io/badge/Licen%C3%A7a-MIT-yellow.svg)

Firmware modular para a placa Raspberry Pi Pico W projetado para ler dados de sensores de ambiente (temperatura, pressão e humidade) e enviá-los para um servidor remoto via **conexão Ethernet com fio**. Este projeto faz parte da solução SensorFlow e utiliza uma arquitetura modular com o módulo W5500 para comunicação Ethernet estável e confiável.

---

## 📑 Índice

- [⚡ Quick Start](#-quick-start)
- [📜 Visão Geral](#-visão-geral)
- [✨ Funcionalidades](#-funcionalidades)
- [🛠️ Hardware Necessário](#️-hardware-necessário)
- [📋 Especificações Técnicas](#-especificações-técnicas)
- [🔌 Esquema de Ligação](#-esquema-de-ligação)
- [⚙️ Configuração do Ambiente](#️-configuração-do-ambiente)
- [🚀 Compilar e Enviar](#-compilar-e-enviar)
- [📡 API Reference](#-api-reference)
- [🐛 Debugging e Troubleshooting](#-debugging-e-troubleshooting)
- [📁 Estrutura do Projeto](#-estrutura-do-projeto)
- [📄 Licença](#-licença)

---

## ⚡ Quick Start

Configuração rápida em 5 minutos:

1. **Instale o ambiente**: VS Code + [Extensão Raspberry Pi Pico](https://marketplace.visualstudio.com/items?itemName=RaspberryPi.raspberry-pi-pico) + [Pico SDK](https://www.raspberrypi.com/documentation/microcontrollers/c_sdk.html)

2. **Clone e configure**:
   ```bash
   git clone https://github.com/jpaullopes/sensorflow-firmware.git
   cd sensorflow-firmware
   ```

3. **Crie o arquivo `secrets.cmake`** com configurações de rede:
   ```cmake
   # Configurações do Servidor de Destino
   set(TARGET_SERVER_IP "192.168.15.1")  # IP do seu servidor
   set(TARGET_PORT 8000)                 # Porta do seu servidor
   set(TARGET_PATH "/api/temperature_reading")  # Caminho da API
   set(API_KEY "SuaChaveAPI")             # Sua chave de API
   ```
4. **Conecte os sensores e módulo Ethernet** conforme [tabela de conexões](#-esquema-de-ligação)

5. **Compile e envie**:
   - Abra no VS Code, pressione `Ctrl+Shift+P` → `CMake: Build`
   - Coloque Pico W em modo BOOTSEL e arraste `build/main.uf2` para `RPI-RP2`

✅ **Pronto!** Conecte cabo Ethernet e monitore via Serial para ver os logs de conexão e envio de dados.

---

## 📜 Visão Geral

O firmware inicializa os sensores, configura uma conexão Ethernet com fio através do módulo W5500 e, em intervalos regulares, envia os dados coletados para uma API backend através de requisições HTTP POST via TCP sockets. Cada dispositivo é unicamente identificado pelo ID da placa. O código foi organizado em módulos independentes para melhor organização e reutilização.

## ✨ Funcionalidades

- **Leitura de Sensores:**
  - Temperatura e Pressão com o sensor **BMP280**.
  - Humidade com o sensor **AHT10**.
- **Conectividade:**
  - Conexão Ethernet com fio via módulo **W5500**.
  - Comunicação TCP/IP direta com controle de sockets.
  - IP estático configurável via código.
- **Envio de Dados:**
  - Construção de corpo de requisição em formato JSON.
  - Envio dos dados via método HTTP POST através de sockets TCP.
  - Identificação única do dispositivo usando o ID da placa.
- **Arquitetura Modular:**
  - **Ethernet Manager**: Gerencia conexão Ethernet e inicialização do W5500.
  - **HTTP Client**: Gerencia requisições TCP/HTTP para o servidor via sockets diretos.
  - **Sensor Manager**: Centraliza a leitura de todos os sensores.
  - **Drivers de Sensores**: Drivers separados para BMP280 e AHT10.
  - **W5500 Config**: Interface SPI para comunicação com o módulo Ethernet.

## 🛠️ Hardware Necessário

- **Raspberry Pi Pico W** (RP2040 + CYW43439)
- **Módulo Ethernet W5500** - Controlador Ethernet via SPI
- **Sensor BMP280** - Pressão e Temperatura (I2C)
- **Sensor AHT10** - Umidade relativa (I2C)
- Fios jumper e protoboard para conexões
- Cabo Ethernet RJ45
- Cabo USB Micro-B para programação

### 📋 Especificações Técnicas

| Componente | Especificação | Observações |
|------------|---------------|-------------|
| **Pico W** | 3.3V, ~100mA | Controlador principal RP2040 |
| **W5500** | 3.3V, ~150mA | Controlador Ethernet via SPI |
| **BMP280** | 1.8-3.6V, <1mA | Pressão: 300-1100 hPa |
| **AHT10** | 2.0-5.5V, <1mA | Umidade: 0-100% RH |
| **Total** | 3.3V, ~252mA | Alimentação via USB ou externa |

### 🔌 Esquema de Ligação

#### **Módulo W5500 Ethernet (SPI0)**
| W5500 | Pino no Pico W | Função |
| :--- | :--- | :--- |
| VCC | 3V3 (OUT) | Alimentação 3.3V |
| GND | GND | Terra |
| MISO | GP16 (SPI0_RX) | SPI Master In, Slave Out |
| MOSI | GP19 (SPI0_TX) | SPI Master Out, Slave In |
| SCK | GP18 (SPI0_SCK) | SPI Clock |
| CS | GP17 | Chip Select |
| RST | GP20 | Reset (opcional) |

#### **Sensores (I2C)**
| Sensor | Pino no Pico W | Função |
| :--- | :--- | :--- |
| **BMP280 (I2C0)** | | |
| VCC | 3V3 (OUT) | Alimentação 3.3V |
| GND | GND | Terra |
| SCL | GP1 (I2C0_SCL) | Clock I2C |
| SDA | GP0 (I2C0_SDA) | Data I2C |
| **AHT10 (I2C1)** | | |
| VCC | 3V3 (OUT) | Alimentação 3.3V |
| GND | GND | Terra |
| SCL | GP3 (I2C1_SCL) | Clock I2C |
| SDA | GP2 (I2C1_SDA) | Data I2C |

#### **Configuração de Rede**
- **IP Estático**: 192.168.15.2
- **Gateway**: 192.168.15.1  
- **Máscara**: 255.255.255.0
- **MAC Address**: 00:08:DC:11:22:33

## ⚙️ Configuração do Ambiente

Este projeto foi desenvolvido utilizando o SDK oficial do Raspberry Pi Pico e o Visual Studio Code.

### 1. Pré-requisitos

-   **VS Code**: Versão 1.80+ com a extensão [Raspberry Pi Pico](https://marketplace.visualstudio.com/items?itemName=RaspberryPi.raspberry-pi-pico).
-   **Pico SDK**: Versão 2.1.1+ - Siga o [guia oficial](https://www.raspberrypi.com/documentation/microcontrollers/c_sdk.html) para instalar o SDK, a toolchain ARM GCC e as outras dependências.
-   **CMake**: Versão 3.13+ (normalmente incluído com o Pico SDK)
-   **Git**: Para clonar o repositório

### 2. Configuração do Projeto

1.  **Clone o repositório:**
    ```bash
    git clone https://github.com/jpaullopes/sensorflow-firmware.git
    cd sensorflow-firmware
    ```

2. **Crie o arquivo de segredos:**
   O projeto utiliza um arquivo `secrets.cmake` para armazenar informações sensíveis como configurações de rede e chaves de API. Este arquivo não é versionado pelo Git para sua segurança.

   - Crie o arquivo `secrets.cmake` na raiz do projeto e preencha com as suas informações:
        ```cmake
        # Configurações do Servidor de Destino
        set(TARGET_SERVER_IP "192.168.15.1")  # IP do seu servidor
        set(TARGET_PORT 8000)                 # Porta do seu servidor
        set(TARGET_PATH "/api/temperature_reading")  # Caminho da API
        set(API_KEY "SUA_CHAVE_DE_API_SECRETA")      # Sua chave de API
        ```

   **⚠️ Importante**: As configurações de rede Ethernet (IP do dispositivo, gateway, máscara) estão hard-coded no firmware. Para alterá-las, edite o arquivo `inc/ethernet_manager/ethernet_manager.c`.

## 🔧 Desenvolvimento e Modificação

### Adicionando Novos Sensores

Para adicionar um novo sensor ao projeto:

1. **Crie um novo driver** em `inc/novo_sensor/`:
   ```c
   // inc/novo_sensor/novo_sensor.h
   void novo_sensor_init(void);
   float novo_sensor_read(void);
   ```

2. **Atualize o Sensor Manager**:
   ```c
   // inc/sensor_manager/sensor_manager.h
   typedef struct {
       float temperature_c;
       float humidity_percent;
       float pressure_hpa;
       float novo_valor;  // Adicione aqui
   } sensors_reading_t;
   ```

3. **Atualize o CMakeLists.txt**:
   ```cmake
   add_executable(main src/main.c
   inc/bmp280/bmp280.c
   inc/aht10/aht10.c
   inc/novo_sensor/novo_sensor.c  # Adicione aqui
   # ... outros arquivos
   ```

### Modificando a Comunicação HTTP

Para alterar o formato JSON ou adicionar novos campos:

1. **Edite o HTTP Client** (`inc/http_client/http_client.c`):
   ```c
   // Modifique a função callback_connected()
   snprintf(json_body, sizeof(json_body),
            "{\"temperature\":%.2f, \"humidity\":\"%.2f\", \"pressure\":\"%.2f\", \"sensor_id\":\"%s\", \"novo_campo\":\"valor\"}",
            // ... parâmetros
   ```

2. **Atualize a estrutura `sensor_data_t`** se necessário.

## 🚀 Compilar e Enviar

Com o ambiente e o projeto configurados, você pode compilar e enviar o firmware para a sua Pico W.

### Método 1: Via VS Code (Recomendado)
1.  **Abra o projeto no VS Code.** A extensão do Raspberry Pi Pico deve detectar e configurar o projeto automaticamente.
2.  **Compile o projeto:**
    -   Pressione `Ctrl+Shift+P` e execute `CMake: Build`
    -   Ou pressione `F7` (se configurado)
3.  **Envie o firmware:**
    -   Coloque sua Pico W em modo `BOOTSEL` (segure o botão BOOTSEL e conecte o cabo USB).
    -   Pressione `Shift+F5` para iniciar o upload (ou use a tarefa `Run Project`).
    -   Alternativamente, arraste o arquivo `build/main.uf2` para o drive `RPI-RP2` que aparece no seu computador.

### Método 2: Via Terminal
```bash
# Configurar o build
cmake -B build

# Compilar
cmake --build build

# O arquivo main.uf2 estará em build/
# Arraste-o para o drive RPI-RP2 quando a Pico estiver em modo BOOTSEL
```

## 🐛 Debugging e Troubleshooting

### Monitoramento via Serial

O firmware envia logs detalhados via USB Serial. Para monitorar:

1. **No VS Code**: Abra o terminal serial através do menu `Terminal > New Terminal`
2. **Aplicativos externos**: Use qualquer terminal serial (PuTTY, Arduino Serial Monitor, etc.) na porta COM correspondente

### 📝 Exemplo de Output Normal

```
====================================================
=     MONITOR DE SENSOR | RASPBERRY PI PICO W     =
=     ID Único da Placa: e660c0e687254725         =
====================================================
[INFO] Inicializando Ethernet...
[INFO] Configurando módulo W5500...
[OK]   W5500 inicializado com sucesso!
[INFO] Configurando IP estático: 192.168.15.2
[INFO] Gateway: 192.168.15.1 | Máscara: 255.255.255.0
[OK]   Ethernet conectado com sucesso!
[INFO] Inicializando sensores...
[OK]   Sensor BMP280 inicializado.
[OK]   Sensor AHT10 inicializado.
[OK]   Sensores inicializados com sucesso.
[INFO] Iniciando ciclos de envio a cada 5 segundos.

[DADOS] Leitura dos sensores -> Temp: 23.45 C | Hum: 56.78 % | Pres: 1013.25 hPa

****************************************************
*           INICIANDO CICLO DE ENVIO               *
****************************************************
[INFO] Conectando via socket TCP ao servidor 192.168.15.1:8000...
[OK]   Socket TCP conectado com sucesso.
[DADOS] Preparando o seguinte JSON para envio:
       -> {"temperature":23.45, "humidity":56.78, "pressure":1013.25, "sensor_id":"e660c0e687254725"}
[INFO] Enviando requisição POST via socket...
[OK]   Requisição enviada. Aguardando resposta.
[INFO] Socket fechado.
[INFO] Ciclo de envio concluído.

Aguardando 5 segundos para o próximo envio...
```

### Mensagens de Log Comuns

- `[INFO] Inicializando Ethernet...` - Iniciando configuração Ethernet
- `[OK] W5500 inicializado com sucesso!` - Módulo Ethernet configurado
- `[INFO] Configurando IP estático: ...` - Configuração de rede
- `[OK] Ethernet conectado com sucesso!` - Conexão Ethernet estabelecida
- `[DADOS] Leitura dos sensores -> ...` - Dados lidos dos sensores  
- `[INFO] Conectando via socket TCP...` - Iniciando conexão TCP
- `[OK] Socket TCP conectado com sucesso.` - Conexão TCP estabelecida
- `[OK] Requisição enviada. Aguardando resposta.` - Dados enviados com sucesso
- `[ERRO] ...` - Indica problemas que precisam ser investigados

### 📝 Logs de Erro Comuns

- `[ERRO] Falha ao inicializar W5500.`
  - Verifique as conexões SPI com o módulo W5500.
  - Confirme se o módulo está alimentado (3.3V).
  - Verifique se o cabo Ethernet está conectado.

- `[ERRO] Sensor não responde.`
  - Verifique as conexões I2C e a alimentação do sensor.
  - Confirme se o endereço I2C do sensor está correto.

- `[ERRO] Falha na conexão TCP.`
  - Verifique a conectividade de rede com o servidor.
  - Confirme IP e porta no arquivo `secrets.cmake`.
  - Certifique-se de que o servidor está ativo e acessível.

### Problemas Comuns

**Ethernet não conecta:**
- Verifique as conexões SPI com o W5500 (especialmente CS, SCK, MOSI, MISO)
- Confirme se o cabo Ethernet está conectado e funcionando
- Verifique se o switch/router está funcionando
- Teste a conectividade com um ping ao gateway (192.168.15.1)

**Módulo W5500 não responde:**
- Confirme alimentação 3.3V no W5500
- Verifique se o pino de reset (GP20) está funcionando
- Teste a comunicação SPI com um osciloscópio
- Confirme se as definições de pinos no `w5500_config.c` estão corretas

**Sensores não funcionam:**
- Verifique as conexões I2C
- Confirme se os sensores estão alimentados (3.3V)
- Teste a continuidade dos fios

**HTTP falha:**
- Verifique se o servidor está rodando
- Confirme IP e porta no `secrets.cmake`
- Teste a conectividade de rede
- Verifique se o servidor aceita requisições da rede local

## 📁 Estrutura do Projeto

```
.
├── .vscode/                    # Configurações do VS Code (tasks, launch, settings)
├── build/                      # Diretório dos arquivos de compilação (gerado)
├── drivers/                    # Bibliotecas externas
│   └── ioLibrary_Driver/      # WIZnet ioLibrary para W5500 (baixada automaticamente)
├── inc/                        # Bibliotecas, drivers e módulos
│   ├── aht10/                 # Driver do sensor de humidade AHT10
│   │   ├── aht10.h
│   │   └── aht10.c
│   ├── bmp280/                # Driver do sensor de temperatura/pressão BMP280
│   │   ├── bmp280.h
│   │   └── bmp280.c
│   ├── ethernet_manager/      # Módulo de gerenciamento Ethernet
│   │   ├── ethernet_manager.h
│   │   ├── ethernet_manager.c
│   │   ├── w5500_config.h
│   │   ├── w5500_config.c
│   │   └── wizchip_macros.h
│   ├── http_client/           # Módulo cliente HTTP/TCP via sockets
│   │   ├── http_client.h
│   │   └── http_client.c
│   └── sensor_manager/        # Módulo de gerenciamento de sensores
│       ├── sensor_manager.h
│       └── sensor_manager.c
├── src/                       # Código fonte principal
│   └── main.c                 # Arquivo principal da aplicação
├── CMakeLists.txt             # Script de compilação do CMake
├── lwipopts.h                 # Configurações da pilha TCP/IP LwIP (legado)
├── secrets.cmake              # Arquivo de configurações sensíveis (servidor, API)
└── README.md                  # Este arquivo
```

## 🏗️ Arquitetura Modular

O firmware foi organizado em módulos independentes para facilitar manutenção e expansão:

### 🌐 Ethernet Manager (`inc/ethernet_manager/`)

- **Responsabilidade**: Gerenciar conexão Ethernet e comunicação com W5500
- **Funções principais**:
  - `ethernet_init()`: Inicializa SPI e configura W5500 com IP estático
  - `ethernet_get_status()`: Verifica status da conexão Ethernet
  - `ethernet_cleanup()`: Limpa recursos e desconecta
- **Componentes**:
  - `w5500_config.c/h`: Interface SPI de baixo nível com W5500
  - `wizchip_macros.h`: Macros para mapeamento das funções ioLibrary

### 🌐 HTTP Client (`inc/http_client/`)

- **Responsabilidade**: Gerenciar comunicação HTTP/TCP via sockets diretos
- **Funções principais**:
  - `http_send_sensor_data()`: Envia dados dos sensores via HTTP POST usando sockets TCP
- **Características**:
  - Comunicação TCP direta sem callbacks assíncronos
  - Timeout de conexão configurável
  - Construção manual de headers HTTP
  - Controle direto de sockets da ioLibrary

### 📊 Sensor Manager (`inc/sensor_manager/`)

- **Responsabilidade**: Centralizar leitura de todos os sensores
- **Funções principais**:
  - `sensors_init()`: Inicializa todos os sensores
  - `sensors_read_all()`: Lê dados de todos os sensores de uma vez
- **Vantagens**:
  - Interface unificada para múltiplos sensores
  - Tratamento centralizado de erros  
  - Estrutura de dados consolidada

### 🔧 Drivers de Sensores (`inc/aht10/`, `inc/bmp280/`)

- **Responsabilidade**: Interface direta com os sensores via I2C
- **Características**:
  - Drivers independentes e reutilizáveis
  - Calibração automática
  - Estruturas de dados específicas para cada sensor

### ⚡ W5500 Config (`inc/ethernet_manager/w5500_config.c`)

- **Responsabilidade**: Interface SPI para comunicação com módulo W5500
- **Características**:
  - Funções callback para a ioLibrary WIZnet  
  - Controle de CS (Chip Select) e pinos SPI
  - Comunicação SPI a 50MHz via spi0

## ❓ FAQ

### 1. O firmware não conecta via Ethernet. O que fazer?

- Verifique as conexões SPI entre Pico W e W5500
- Confirme se o cabo Ethernet está conectado e funcionando
- Teste se o switch/router está acessível
- Verifique a alimentação do módulo W5500 (3.3V)

### 2. Os sensores não estão funcionando. Como resolver?

- Confirme as conexões I2C e a alimentação dos sensores
- Teste a continuidade dos fios com um multímetro

### 3. O servidor não responde às requisições HTTP. O que verificar?

- Certifique-se de que o servidor está ativo e acessível na rede
- Verifique o IP, porta e caminho da API no arquivo `secrets.cmake`
- Teste a conectividade com o servidor usando ferramentas como `ping` ou `curl`

## 📡 API Reference

### Exemplo de Requisição HTTP

```http
POST /api/temperature_reading HTTP/1.1
Host: 192.168.15.1:8000
Content-Type: application/json
API-Key: SuaChaveAPI

{
  "temperature": 23.45,
  "humidity": 56.78,
  "pressure": 1013.25,
  "sensor_id": "e660c0e687254725"
}
```

### Exemplo de Resposta Esperada

```json
{
  "status": "success",
  "message": "Dados recebidos com sucesso."
}
```

### Configurações de Rede

O firmware está configurado com as seguintes configurações de rede por padrão:

- **IP do Dispositivo**: 192.168.15.2
- **Gateway**: 192.168.15.1
- **Máscara de Sub-rede**: 255.255.255.0
- **MAC Address**: 00:08:DC:11:22:33

Para alterar essas configurações, edite o arquivo `inc/ethernet_manager/ethernet_manager.c`.

## 📄 Licença

Este projeto está licenciado sob a licença MIT. Veja o texto completo da licença em [LICENSE](./LICENSE).
