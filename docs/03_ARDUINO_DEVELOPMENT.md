# 03 — Desenvolvimento em C++ com Arduino e PlatformIO

Para quem pretende programar comportamentos personalizados no **Stack-chan**, a M5Stack disponibiliza a biblioteca oficial de suporte à placa (**BSP**) denominada `M5StackChan`, construída sobre o ecossistema `M5Unified` e `M5GFX`.

---

## 1. Configuração do Ambiente de Desenvolvimento

### No Arduino IDE (2.x)

1. **URL Adicional do Gestor de Placas**:
   * Aceda a `File` -> `Preferences` -> `Additional Boards Manager URLs` e adicione:
     ```
     https://m5stack.oss-cn-shenzhen.aliyuncs.com/resource/arduino/package_m5stack_index.json
     ```
2. **Instalação da Placa**:
   * Abra o **Boards Manager** (`Tools` -> `Board` -> `Boards Manager...`), pesquise por `M5Stack` e instale a versão **$\ge$ 3.3.7**.
   * Em `Tools` -> `Board`, selecione: **`M5CoreS3`**.
3. **Instalação das Bibliotecas**:
   * Abra o **Library Manager** (`Sketch` -> `Include Library` -> `Manage Libraries...`).
   * Instale:
     * **`M5StackChan`** (Versão $\ge$ 1.0.0).
     * **`M5Unified`** (Instalar todas as dependências sugeridas).
     * **`M5GFX`** (Versão $\ge$ 0.2.27, necessária para suporte ao driver LCD ILI9342E).
     * **`IRremote`** (caso vá utilizar comandos por infravermelhos).

### No PlatformIO (`platformio.ini`)

```ini
[env:m5cores3]
platform = espressif32
board = esp32-s3-devkitc-1
framework = arduino
monitor_speed = 115200
board_build.mcu = esp32s3
board_build.f_cpu = 240000000L
board_build.flash_mode = qio
board_build.psram_type = qio
build_flags = 
    -DBOARD_HAS_PSRAM
    -mfix-esp32-psram-cache-issue
lib_deps =
    m5stack/M5Unified@^0.2.0
    m5stack/M5GFX@^0.2.27
    https://github.com/m5stack/StackChan-BSP.git
    z3t0/IRremote@^4.4.1
```

---

## 2. Como Colocar em Modo de Download (Flashing)

1. Ligue o robô ao computador através de um cabo USB Type-C (preferencialmente na porta inferior da base).
2. Prima e mantenha pressionado o botão **RST** (na parte inferior, junto ao microSD) durante cerca de **3 segundos**.
3. Assim que o **LED verde interior** acender, solte o botão.
4. O LED apaga-se e o ESP32-S3 fica em modo de bootloader pronto a compilar e carregar via porta COM.

---

## 3. Exemplos Práticos de Código

### Exemplo 1: Controlo de Movimento e Servos (Pan/Tilt)

Este código demonstra como mover a cabeça para ângulos específicos no eixo horizontal (X) e vertical (Y), além de efetuar rotações contínuas de 360°.

```cpp
#include <M5StackChan.h>

int state = 1; 
const int MAX_STATE = 7;

void setup() {
    // Inicialização geral do StackChan e periféricos
    M5StackChan.begin();
    M5StackChan.Motion.goHome();

    // Configuração do ecrã LCD
    M5StackChan.Display().setTextSize(2);
    M5StackChan.Display().setTextScroll(true);
    M5StackChan.Display().setTextColor(TFT_ORANGE);
    M5StackChan.Display().printf("> Toque no topo para iniciar\n");
    M5StackChan.Display().setTextColor(TFT_GREEN);
}

void loop() {
    M5StackChan.update();

    if (M5StackChan.TouchSensor.wasPressed()) {
        switch(state) {
            /* Unidade de ângulo: 10 unidades = 1 grau | Velocidade: 0 a 1000 */
            /* Limites: Eixo X (-1280 a 1280 = -128° a 128°), Eixo Y (0 a 900 = 0° a 90°) */
            case 1: 
                // Mover eixo X para 0° e eixo Y para 45°
                M5StackChan.Motion.move(0, 450);
                M5StackChan.Display().printf("> Olhar para a frente a 45 graus\n");
                break;
            case 2: 
                // Mover cabeça para a esquerda (90°)
                M5StackChan.Motion.moveX(900, 500);
                M5StackChan.Display().printf("> Rodar para a Esquerda\n");
                break;
            case 3: 
                // Mover cabeça para a direita (-90°)
                M5StackChan.Motion.moveX(-900, 500);
                M5StackChan.Display().printf("> Rodar para a Direita\n");
                break;
            case 4: 
                // Olhar para cima (máx recomendado: 85° a 90°)
                M5StackChan.Motion.moveY(850, 300);
                M5StackChan.Display().printf("> Olhar para Cima\n");
                break;
            case 5: 
                // Olhar para baixo (0°)
                M5StackChan.Motion.moveY(0, 300);
                M5StackChan.Display().printf("> Olhar para Baixo\n");
                break;
            case 6: 
                // Rotação horizontal contínua de 360° no sentido horário
                M5StackChan.Display().printf("> Rotacao Continua 360 Horaria\n");
                M5StackChan.Motion.rotateX(-800);
                delay(2000);
                M5StackChan.Motion.stop();
                break;
            case 7: 
                // Rotação horizontal contínua no sentido anti-horário
                M5StackChan.Display().printf("> Rotacao Continua 360 Anti-Horaria\n");
                M5StackChan.Motion.rotateX(800);
                delay(2000);
                M5StackChan.Motion.stop();
                break;
            default:
                M5StackChan.Motion.goHome();
                M5StackChan.Display().printf("> Posicao Home\n");
                break;
        }
        
        state++;
        if (state > MAX_STATE) {
            state = 1;
            M5StackChan.Display().setTextColor(TFT_ORANGE);
            M5StackChan.Display().printf("> Ciclo concluido. Toque para recomecar\n");
            M5StackChan.Display().setTextColor(TFT_GREEN);
        }
    }
    delay(10);
}
```

---

### Exemplo 2: Gestos do Sensor de Toque Superior

O painel capacitivo Si12T no topo da cabeça deteta cliques simples e deslizamentos direcionais (*swipes*):

```cpp
#include <M5StackChan.h>

void setup() {
    M5StackChan.begin();

    M5StackChan.Display().setTextSize(2);
    M5StackChan.Display().setTextScroll(true);
    M5StackChan.Display().setTextColor(TFT_CYAN);
    M5StackChan.Display().printf("Sensor Tátil Ativo!\n");
}

void loop() {
    M5StackChan.update();
    auto& ts = M5StackChan.TouchSensor;

    if (ts.wasClicked()) {
        M5StackChan.Display().printf("[Gesto] Clique no topo detetado!\n");
    }

    if (ts.wasSwipedForward()) {
        M5StackChan.Display().printf("[Gesto] Swipe para a Frente (Carinho)!\n");
    }

    if (ts.wasSwipedBackward()) {
        M5StackChan.Display().printf("[Gesto] Swipe para Tras!\n");
    }

    delay(50);
}
```

---

### Exemplo 3: Calibração de Servos com Interface Tátil

Permite memorizar a posição atual como o zero mecânico (*Home*) através do toque no ecrã:

```cpp
#include <M5StackChan.h>

namespace {
constexpr uint16_t kBgColor       = TFT_BLACK;
constexpr uint16_t kBorderColor   = TFT_WHITE;
constexpr uint16_t kTopBtnColor    = 0x39C7;
constexpr uint16_t kBottomBtnColor = 0x03EF;
constexpr uint16_t kTextColor     = TFT_WHITE;

enum class ButtonZone { None, SetHome, GoHome };
ButtonZone pressed_zone = ButtonZone::None;

ButtonZone getButtonZone(const int16_t y, const int16_t height) {
    return y < (height / 2) ? ButtonZone::SetHome : ButtonZone::GoHome;
}

void drawButton(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color, const char* l1, const char* l2) {
    auto& d = M5StackChan.Display();
    d.fillRect(x, y, w, h, color);
    d.drawRect(x, y, w, h, kBorderColor);
    d.setTextDatum(middle_center);
    d.setTextColor(kTextColor, color);
    d.setTextSize(2);
    d.drawString(l1, x + w / 2, y + h / 2 - 12);
    d.drawString(l2, x + w / 2, y + h / 2 + 12);
}

void drawUi(ButtonZone active) {
    auto& d = M5StackChan.Display();
    const int16_t w = d.width(), h = d.height();
    const int16_t half_h = (h - 8) / 2;

    d.startWrite();
    d.fillScreen(kBgColor);
    drawButton(8, 8, w - 16, half_h - 12, 
               active == ButtonZone::SetHome ? 0x2204 : kTopBtnColor, 
               "Gravar Posicao", "Como Origem (Home)");
    drawButton(8, half_h + 12, w - 16, h - (half_h + 20), 
               active == ButtonZone::GoHome ? 0x01E8 : kBottomBtnColor, 
               "Mover Para", "Origem (Home)");
    d.endWrite();
}
} // namespace

void setup() {
    M5StackChan.begin();
    M5StackChan.Display().setTextScroll(false);
    drawUi(ButtonZone::None);
}

void loop() {
    M5StackChan.update();
    auto& d = M5StackChan.Display();
    int16_t tx = 0, ty = 0;
    bool touching = d.getTouch(&tx, &ty);

    if (touching) {
        ButtonZone cur = getButtonZone(ty, d.height());
        if (cur != pressed_zone) {
            pressed_zone = cur;
            drawUi(pressed_zone);
        }
    } else if (pressed_zone != ButtonZone::None) {
        ButtonZone released = pressed_zone;
        pressed_zone = ButtonZone::None;
        drawUi(ButtonZone::None);

        if (released == ButtonZone::SetHome) {
            M5StackChan.Motion.setCurrentPostionAsHome();
        } else if (released == ButtonZone::GoHome) {
            M5StackChan.Motion.goHome();
        }
    }
    delay(20);
}
```

---

### Exemplo 4: Emissão e Receção de Infravermelhos (Protocolo NEC)

```cpp
#include <IRremote.hpp>
#include <M5StackChan.h>

#define IR_SEND_PIN    5   // Pino GPIO do transmissor IR
#define IR_RECEIVE_PIN 10  // Pino GPIO do recetor IR

uint16_t address = 0x0001;
uint8_t  command = 0x10;

void setup() {
    M5StackChan.begin();
    Serial.begin(115200);

    IrReceiver.begin(IR_RECEIVE_PIN);
    IrSender.begin(DISABLE_LED_FEEDBACK);
    IrSender.setSendPin(IR_SEND_PIN);

    M5StackChan.Display().clear();
    M5StackChan.Display().printf("IR NEC Teste Ativo\n");
}

void loop() {
    // 1. Emitir comando por IR
    IrSender.sendNEC(address, command, 0);
    M5StackChan.Display().printf("Enviado NEC: Cmd=0x%02X\n", command);
    IrReceiver.restartAfterSend();

    delay(100);

    // 2. Tentar receber sinal refletido ou externo
    if (IrReceiver.decode()) {
        M5StackChan.Display().printf("Recebido: Cmd=0x%02X\n", IrReceiver.decodedIRData.command);
        IrReceiver.resume();
    }

    delay(3000);
}
```
