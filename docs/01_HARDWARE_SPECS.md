# 01 — Especificações Técnicas e Arquitetura de Hardware

## 1. Identificação do Produto

* **Nome Comercial**: M5Stack Stack-chan
* **SKUs**:
  * `K151`: Unidade Stack-chan standard (com CoreS3 pré-instalado).
  * `K151-R`: Stack-chan Remote Controller Kit (inclui comando remoto sem fios com **StickC-Plus** e **Hat Mini JoyC**).
* **Dimensões e Peso**:
  * Unidade Stack-chan: `54.0 x 70.5 x 61.5 mm` | Peso: `187.2 g` (Bruto embalado: `272.4 g`).
  * Comando Remoto: Peso: `37.6 g` (Bruto kit remoto: `372.9 g`).

---

## 2. Tabela Geral de Especificações

| Componente | Especificação Detalhada |
| :--- | :--- |
| **Controlador Principal** | Espressif **ESP32-S3** Xtensa® dual-core 32-bit LX7 @ 240 MHz |
| **Memória Integrada** | **16 MB Flash** + **8 MB Quad-SPI PSRAM** |
| **Conetividade Sem Fios** | Wi-Fi 2.4 GHz (802.11 b/g/n) + Bluetooth® 5 LE + Emissor/Recetor Infravermelhos (IR NEC) |
| **Ecrã** | IPS LCD 2.0 polegadas, resolução **320x240**, 65K cores (controlador ILI9342C / ILI9342E) |
| **Touch Screen** | Painel tátil capacitivo multi-toque com vidro de alta resistência (controlador FT6336U) |
| **Câmara** | Sensor GC0308, resolução **640x480** (0.3 Megapixels) |
| **Áudio e Voz** | Codec áudio ES7210 com **duplo microfone** frontal; amplificador I2S AW88298 (16-bit) com altifalante de **1W** |
| **Sensores Host** | Sensor de proximidade e luz ambiente LTR-553ALS-WA; IMU de 9 eixos (Acelerómetro + Giroscópio BMI270 e Magnetómetro BMM150); RTC BM8563 |
| **Sensores Corpo (Base)**| Painel tátil de 3 zonas no topo (controlador Si12T); Módulo NFC completo (ST25R3916); Monitor de bateria (INA226) |
| **Atuadores (Motores)** | 2x Servos inteligentes com bus série e retorno de posição/torque (SCS9009 / SCS0009): Pan horizontal 360° + Tilt vertical 90° |
| **Iluminação** | 12x LEDs RGB WS2812C em duas filas (geridos via expansor PY32L020); LED indicador de estado no topo |
| **Alimentação** | Bateria interna de lítio 550 mAh; Gestão de energia PMIC AXP2101; 2x portas USB Type-C (Host e Base) |
| **Expansão Externa** | Ranhura microSD; 3x portas Grove HY2.0-4P (Port A, Port B, Port C); Furação compatível com peças LEGO® |

---

## 3. Mapeamento de Pinos (PinMap)

### Mapeamento do Corpo do Robô (Ligação ESP32-S3 CoreS3 -> Base)

| Sinal | Pino ESP32-S3 | Função |
| :--- | :--- | :--- |
| **Servo_TX** | `G6` | Transmissão de comandos UART para os Servos SCS9009 |
| **Servo_RX** | `G7` | Receção de telemetria UART dos Servos SCS9009 |
| **IR_SEND** | `G5` | Transmissão de sinais de Infravermelhos (IRM56384 transmitter) |
| **IR_REC** | `G10` | Receção de sinais de Infravermelhos (recetor IR NEC) |
| **I2C_SCL** | `G11` | Relógio I2C do barramento da base |
| **I2C_SDA** | `G12` | Dados I2C do barramento da base |

### Expansor de E/S da Base (PY32L020)

O microcontrolador auxiliar PY32L020 atua como expansor de pinos na base:
* **IO1**: `VM_EN` (Ativação de alimentação dos motores).
* **IO14**: `RGB` (Controlo da fita de 12 LEDs WS2812C).
* **Endereço I2C**: `0x6F` (padrão, com pino `ADD_SEL` a nível baixo) ou `0x71` (`ADD_SEL` a nível alto).

### Endereços I2C dos Componentes da Base

| Componente | Chip / Descrição | Endereço I2C (7-bit) |
| :--- | :--- | :--- |
| **Monitor de Bateria** | INA226AIDGSR | `0x41` |
| **Módulo NFC** | ST25R3916-AQWT | `0x50` |
| **Sensor de Toque no Topo** | Si12T (compatível TSM12) | `0x68` |
| **Expansor IO & RGB** | PY32L020 | `0x6F` (ou `0x71`) |

### Portas de Expansão Grove (HY2.0-4P)

| Conetor | Preto (GND) | Vermelho (VCC) | Amarelo | Branco | Utilização Típica |
| :--- | :--- | :--- | :--- | :--- | :--- |
| **PORT.A** | GND | 5V | `G2` | `G1` | Sensores I2C externos |
| **PORT.B** | GND | 5V | `G9` | `G8` | Entradas/Saídas Digitais (GPIO / DAC / ADC) |
| **PORT.C** | GND | 5V | `G17` | `G18` | Comunicação UART série externa |

---

## 4. Servos e Limites Críticos de Segurança

> [!CAUTION]
> **Aviso de Risco Mecânico e Sobreaquecimento dos Motores**:
> 1. **Rotação Manual Proibida sob Tensão**: Nunca force manualmente a rotação de qualquer junta do robô se sentir resistência mecânica. Forçar o motor energizado danifica irreversivelmente as engrenagens internas.
> 2. **Limite de Ângulo do Eixo Y (Tilt - Cabeça Cima/Baixo)**: O movimento do servo do Eixo Y deve ser limitado estritamente entre **5° e 85°** (limite absoluto de 0° a 90°). Forçar o servo acima de 90° ou abaixo de 0° provoca bloqueio mecânico (*servo stall*), queimando o driver interno.
> 3. **Eixo X (Pan - Horizontal)**: Suporta rotação contínua livre de 360° em ambas as direções sem restrição de fim-de-curso.

---

## 5. Gestão de Energia e Estados de Funcionamento

O robô possui dois botões físicos principais:
* **Botão de Energia (Power)**: Localizado na lateral esquerda, junto à porta USB-C do CoreS3.
  * **Ligar**: 1 clique curto.
  * **Desligar**: Premir e manter durante **6 segundos**.
* **Botão de Reinício (RST)**: Localizado na parte inferior da unidade, junto à ranhura do cartão microSD.
  * **Reset Normal**: 1 clique curto.
  * **Modo de Download (Bootloader para Flashing)**: Com o cabo USB ligado ao computador, **premir e manter o botão RST durante 3 segundos**. O LED indicador interior acenderá a **verde**. Solte o botão; o LED apaga-se e o ESP32-S3 entra em modo de receção de firmware.

> [!TIP]
> Embora ambas as portas USB-C (na cabeça e na base) permitam dados e carregamento, **recomenda-se sempre ligar o cabo à porta da base**, evitando que o movimento dos motores puxe ou parta o cabo.

---

## 6. Esquemas Elétricos e Folhas de Dados Oficiais

### Ficheiros de Esquemas (PDF)
* [CoreS3 Host Schematic PDF](https://m5stack-doc.oss-cn-shenzhen.aliyuncs.com/490/Sch_M5_CoreS3_v1.0.pdf)
* [StackChan Base Adapter Board Schematic PDF](https://m5stack-doc.oss-cn-shenzhen.aliyuncs.com/1205/SCH_Adapter.pdf)
* [StackChan Base Power Board Schematic PDF](https://m5stack-doc.oss-cn-shenzhen.aliyuncs.com/1205/SCH_Power.pdf)
* [StackChan Base Ring Board Schematic PDF](https://m5stack-doc.oss-cn-shenzhen.aliyuncs.com/1205/SCH_Ring.pdf)
* [StackChan Base Touch Board Schematic PDF](https://m5stack-doc.oss-cn-shenzhen.aliyuncs.com/1205/SCH_Touch.pdf)

### Datasheets dos Componentes Principais
* **ESP32-S3**: [Manual Técnico Oficial](https://m5stack-doc.oss-cn-shenzhen.aliyuncs.com/490/esp32-s3_technical_reference_manual_en.pdf)
* **Servo SCS0009 / SCS9009**: [Datasheet PDF](https://m5stack-doc.oss-cn-shenzhen.aliyuncs.com/1205/SCS0009.pdf)
* **Audio Codec ES7210**: [Datasheet PDF](https://m5stack.oss-cn-shenzhen.aliyuncs.com/resource/docs/datasheet/core/K128%20CoreS3/ES7210.PDF)
* **Amplificador Áudio AW88298**: [Datasheet PDF](https://m5stack.oss-cn-shenzhen.aliyuncs.com/resource/docs/datasheet/core/K128%20CoreS3/AW88298.PDF)
* **PMIC AXP2101**: [Datasheet PDF](https://m5stack.oss-cn-shenzhen.aliyuncs.com/resource/docs/datasheet/core/K128%20CoreS3/AXP2101_Datasheet_V1.0_en.pdf)
* **Sensor de Toque Si12T**: [Datasheet PDF](https://m5stack-doc.oss-cn-shenzhen.aliyuncs.com/1205/Si12T_Datasheet_EN.pdf)
* **NFC ST25R3916**: [Datasheet PDF](https://m5stack-doc.oss-cn-shenzhen.aliyuncs.com/1205/ST25R3916_EN.pdf)
* **Monitor de Bateria INA226**: [Datasheet PDF](https://m5stack-doc.oss-cn-shenzhen.aliyuncs.com/1171/INA226.pdf)
