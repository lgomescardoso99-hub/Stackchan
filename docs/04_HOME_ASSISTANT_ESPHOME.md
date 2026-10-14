# 04 — Integração com Home Assistant e ESPHome

O **M5Stack Stack-chan** pode ser integrado nativamente no ecossistema **Home Assistant**, funcionando como uma entidade de voz (*Voice Assistant Satellite*) com controlo motorizado da cabeça para expressar reações ou seguir utilizadores.

---

## 1. Instalação em 1 Clique via Web (EspWebTool)

A M5Stack disponibiliza um instalador direto via browser (requer Google Chrome ou Microsoft Edge):
1. Aceda à página oficial: [Home Assistant Kit StackChan](https://docs.m5stack.com/en/homeassistant/kit/stackchan).
2. Ligue o Stack-chan via USB ao computador.
3. Clique no botão **One-Click Flash Firmware**. O instalador descarrega e grava automaticamente o manifesto BSP:
   ```
   https://m5stack-doc.oss-cn-shenzhen.aliyuncs.com/1230/stackchan_bsp_manifest_2026.4.4.json
   ```
4. Após o flash, o dispositivo emite um ponto de acesso Wi-Fi para configuração de rede (*Improv Wi-Fi*).

---

## 2. Configuração Completa em ESPHome (YAML)

Se pretender compilar o seu próprio firmware personalizado através do ESPHome, utilize a configuração de referência abaixo:

```yaml
substitutions:
  name: "stackchan-m5"
  friendly_name: "Stack-chan Robot"

esphome:
  name: ${name}
  friendly_name: ${friendly_name}

esp32:
  board: esp32-s3-devkitc-1
  variant: esp32s3
  framework:
    type: esp-idf

psram:
  mode: quad
  speed: 80MHz

# Configuração UART para comunicação com os servos da base
uart:
  - id: servo_uart
    tx_pin: GPIO6
    rx_pin: GPIO7
    baud_rate: 1000000

# Controlador de Servos Série FTServo (SCS9009)
ftservo:
  - platform: scs9009
    uart_id: servo_uart
    address: 1
    id: x_servo
  - platform: scs9009
    uart_id: servo_uart
    address: 2
    id: y_servo

# Entidades de Controlo Numérico no Home Assistant para Ângulo e Velocidade
number:
  - platform: ftservo
    ftservo_id: x_servo
    angle:
      id: servo_x_angle
      name: "Ângulo Cabeça Eixo X (Horizontal)"
      min_value: -164
      max_value: 164
      use_raw_angle: false
      angle_offset: 164
      step: 5
    speed:
      id: servo_x_speed
      name: "Velocidade Eixo X"
      min_value: 100
      max_value: 1500
      step: 100

  - platform: ftservo
    ftservo_id: y_servo
    angle:
      id: servo_y_angle
      name: "Ângulo Cabeça Eixo Y (Vertical)"
      min_value: 0
      max_value: 90
      use_raw_angle: false
      angle_offset: 239
      step: 5
    speed:
      id: servo_y_speed
      name: "Velocidade Eixo Y"
      min_value: 100
      max_value: 1500
      step: 100

# Barramento I2C para sensores da base e bateria
i2c:
  sda: GPIO12
  scl: GPIO11
  scan: true
  id: bus_a

# Sensor de Telemetria de Bateria e Consumo
sensor:
  - platform: ina226
    i2c_id: bus_a
    address: 0x41
    shunt_resistance: 0.01 ohm
    current:
      name: "Corrente de Bateria"
    power:
      name: "Potência Consumida"
    bus_voltage:
      name: "Tensão da Bateria"
    max_current: 2.0A
```

---

## 3. Automações no Home Assistant

Com as entidades expostas no Home Assistant, é possível automatizar comportamentos como:
* **Olhar para a Porta**: Quando um sensor de presença da porta deteta movimento, rodar a cabeça para a entrada (`number.set_value` no `servo_x_angle`).
* **Expressão de Felicidade**: Fazer um aceno vertical da cabeça quando uma tarefa é concluída com sucesso.
* **Alerta de Alarme**: Rodar continuamente no eixo horizontal em modo patrulha com iluminação vermelha nos LEDs RGB.
