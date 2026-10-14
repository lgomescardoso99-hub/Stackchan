#include <M5StackChan.h>
#include <WiFi.h>
#include "config.h"
#include "avatar_display.h"
#include "llm_orchestrator.h"

AvatarDisplay avatar;
LLMOrchestrator orchestrator;

unsigned long lastTouchTime = 0;
int queryDemoIndex = 0;

// Lista de perguntas de teste em tempo real para acionar por toque
const char* demoQueries[] = {
    "Qual é a previsão do tempo para Lisboa hoje?",
    "Quais são as principais notícias de tecnologia de hoje?",
    "Quem ganhou o último jogo do Benfica ou Sporting?",
    "Qual é o preço atual do Bitcoin hoje?"
};
const int totalDemoQueries = 4;

void connectWiFi() {
    avatar.showMessage("A ligar ao Wi-Fi...");
    Serial.printf("[Wi-Fi] A ligar a %s...\n", WIFI_SSID);

    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 25) {
        delay(400);
        Serial.print(".");
        attempts++;
    }

    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\n[Wi-Fi] Conectado com sucesso!");
        Serial.printf("[Wi-Fi] Endereço IP: %s\n", WiFi.localIP().toString().c_str());
        avatar.setEmotion(FaceEmotion::HAPPY);
        avatar.showMessage("Online! Toque no topo para pesquisar.");
        // Pequena dança de saudação
        M5StackChan.Motion.move(200, 450);
        delay(300);
        M5StackChan.Motion.move(-200, 450);
        delay(300);
        M5StackChan.Motion.goHome();
    } else {
        Serial.println("\n[Wi-Fi] Falha ao ligar ao Wi-Fi. Verifique config.h");
        avatar.setEmotion(FaceEmotion::SURPRISED);
        avatar.showMessage("Erro Wi-Fi! Verifique config.h");
    }
}

void setup() {
    Serial.begin(115200);
    delay(500);

    // Inicialização do hardware do Stack-chan
    M5StackChan.begin();
    M5StackChan.Motion.goHome();

    // Inicialização do display facial
    avatar.init();
    avatar.setEmotion(FaceEmotion::NEUTRAL);

    // Ligação Wi-Fi autónoma
    connectWiFi();
}

void handleUserQuery(const String& query) {
    Serial.printf("\n=========================================\n");
    Serial.printf("[Stack-chan] Nova Questão: %s\n", query.c_str());
    Serial.printf("=========================================\n");

    // 1. Reação imediata de pensamento e movimento de cabeça
    avatar.setEmotion(FaceEmotion::THINKING);
    avatar.showMessage("A pesquisar no Scrapling...");
    M5StackChan.Motion.move(250, 600); // Vira ligeiramente para cima e para a direita

    // 2. Executa a pesquisa web em tempo real e síntese com LLM
    LLMResponse resp = orchestrator.ask(query, true);

    // 3. Atualiza expressão facial e texto de resposta no ecrã
    avatar.setEmotion(resp.emotion);
    avatar.showMessage(resp.text);

    // 4. Expressão física: acenar afirmativamente com a cabeça
    M5StackChan.Motion.move(0, 400);
    delay(250);
    M5StackChan.Motion.move(0, 650);
    delay(250);
    M5StackChan.Motion.goHome();
}

void loop() {
    M5StackChan.update();
    avatar.updateAnimation();

    // Detetar toque no sensor tátil superior ou no ecrã
    bool touched = false;
    if (M5StackChan.TouchSensor.wasClicked() || M5StackChan.TouchSensor.wasPressed()) {
        touched = true;
    }

    int16_t tx = 0, ty = 0;
    if (M5StackChan.Display().getTouch(&tx, &ty)) {
        touched = true;
    }

    if (touched && (millis() - lastTouchTime > 1500)) {
        lastTouchTime = millis();
        // Seleciona a próxima pergunta de teste em tempo real
        String currentQuery = demoQueries[queryDemoIndex];
        queryDemoIndex = (queryDemoIndex + 1) % totalDemoQueries;

        handleUserQuery(currentQuery);
    }

    // Também permite receber perguntas enviadas pelo Serial Monitor para testes
    if (Serial.available() > 0) {
        String serialQuery = Serial.readStringUntil('\n');
        serialQuery.trim();
        if (serialQuery.length() > 0) {
            handleUserQuery(serialQuery);
        }
    }

    delay(20);
}
