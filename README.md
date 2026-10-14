# Stack-chan M5 — Documentação Técnica Completa

Bem-vindo à documentação oficial consolidada e técnica do **M5Stack Stack-chan** (SKU: `K151` / `K151-R`). Este repositório contém todas as especificações de hardware, pinouts, guias de firmware de fábrica, desenvolvimento em Arduino C++, integração com Home Assistant e resolução de problemas.

---

## 📑 Índice da Documentação

| Ficheiro | Descrição |
| :--- | :--- |
| **[`01_HARDWARE_SPECS.md`](docs/01_HARDWARE_SPECS.md)** | Especificações completas, arquitetura ESP32-S3 CoreS3, pinout dos motores e I2C, limites de segurança dos servos, diagramas elétricos e esquemas. |
| **[`02_FACTORY_FIRMWARE_AI_AGENT.md`](docs/02_FACTORY_FIRMWARE_AI_AGENT.md)** | Manual do Firmware de Fábrica, emparelhamento na App *StackChan World*, IA XiaoZhi com comandos de voz, gestos, ESP-NOW e Home Assistant MCP. |
| **[`03_ARDUINO_DEVELOPMENT.md`](docs/03_ARDUINO_DEVELOPMENT.md)** | Guia de desenvolvimento Arduino IDE / PlatformIO, biblioteca `M5StackChan`, API Motion, TouchSensor, exemplos completos de calibração e controlo IR. |
| **[`04_HOME_ASSISTANT_ESPHOME.md`](docs/04_HOME_ASSISTANT_ESPHOME.md)** | Flash em 1 clique via Web (EspWebTool), configuração YAML ESPHome com FTServo (SCS9009) e entidade assistente de voz. |
| **[`05_TROUBLESHOOTING_MAINTENANCE.md`](docs/05_TROUBLESHOOTING_MAINTENANCE.md)** | Restauro com M5Burner, modo de download forçado, diagnóstico de hardware, resolução de problemas de emparelhamento e Wi-Fi 2.4 GHz. |

---

## 🤖 Visão Geral do Produto

O **Stack-chan** é um robô de secretária com Inteligência Artificial co-criado pela **M5Stack** e a comunidade open-source (idealizado originalmente por Shinya Ishikawa `@meganetaaan` e Takao Akaki `@mongonta555`).

O robô combina:
1. **Unidade Central (Host Module)**: Baseada no **M5Stack CoreS3** (ESP32-S3 @ 240MHz, ecrã IPS tátil capacitivo de 2.0", câmara de 0.3 MP, altifalante de 1W com amplificador I2S, microfone duplo com cancelamento de ruído, sensores de proximidade/luz e IMU de 9 eixos).
2. **Corpo Motorizado (Body Module)**: Base com dois servos seriais inteligentes com realimentação de posição (Pan de 360° contínuos e Tilt de 90° verticais), sensor capacitivo de toque de 3 zonas no topo, bateria interna de 550 mAh, 12 LEDs RGB WS2812C, emissor/recetor IR e leitor NFC ST25R3916.

```
                      +-----------------------------------+
                      |         M5CoreS3 (Host)           |
                      |  - ESP32-S3 (16MB Flash, 8MB PSRAM)|
                      |  - 2.0" IPS Touch (ILI9342E/FT6336U)|
                      |  - Dual Mic (ES7210) + Spk (AW88298)|
                      |  - 0.3MP Cam + 9-DOF IMU + RTC     |
                      +-----------------+-----------------+
                                        | (Bus Interno)
                      +-----------------+-----------------+
                      |         Stack-chan Base           |
                      |  - 2x Servos SCS9009 (Pan/Tilt)   |
                      |  - Touch Panel 3 Zonas (Si12T)    |
                      |  - 12x RGB LEDs (PY32L020 / WS2812)|
                      |  - Bateria 550mAh + Monitor INA226|
                      |  - NFC (ST25R3916) + IR TX/RX     |
                      +-----------------------------------+
```

---

## 🚀 Como Começar Rapidamente

* Se pretendes utilizar o **Firmware Oficial com App Móvel e IA de Voz**: segue o [Guia de Firmware de Fábrica](docs/02_FACTORY_FIRMWARE_AI_AGENT.md).
* Se pretendes **Programar o teu próprio código C++**: consulta o [Guia Arduino](docs/03_ARDUINO_DEVELOPMENT.md).
* Se pretendes integrar na tua **Domótica Home Assistant**: consulta o [Guia ESPHome](docs/04_HOME_ASSISTANT_ESPHOME.md).
* Se precisares de **Restaurar o firmware original ou resolver falhas**: consulta o [Guia de Resolução de Problemas](docs/05_TROUBLESHOOTING_MAINTENANCE.md).
