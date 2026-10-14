#include "llm_orchestrator.h"
#include "config.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>

LLMOrchestrator::LLMOrchestrator() {}

LLMResponse LLMOrchestrator::ask(const String& userQuery, bool requireWebSearch) {
    LLMResponse resp;
    resp.emotion = "NEUTRAL";
    resp.text = "";
    resp.success = false;

    if (WiFi.status() != WL_CONNECTED) {
        resp.text = "Sem ligação à rede Wi-Fi.";
        return resp;
    }

    String webContext = "";
    if (requireWebSearch) {
        Serial.println("[LLM] A ativar pesquisa autónoma com Scrapling...");
        bool searchOk = _scrapling.searchWeb(userQuery, webContext);
        if (!searchOk) {
            Serial.println("[LLM] Aviso: Pesquisa web falhou, a prosseguir com conhecimento base.");
        }
    }

    // Preparação do Prompt para a API OpenAI/Groq
    WiFiClientSecure secureClient;
    secureClient.setInsecure();

    HTTPClient http;
    http.setTimeout(HTTP_TIMEOUT_MS * 2);

    if (!http.begin(secureClient, LLM_API_URL)) {
        resp.text = "Erro ao iniciar ligação ao LLM.";
        return resp;
    }

    http.addHeader("Content-Type", "application/json");
    http.addHeader("Authorization", String("Bearer ") + LLM_API_KEY);
    http.addHeader("x-opencode-session", "stackchan-session-001");

    JsonDocument doc;
    doc["model"] = LLM_MODEL_NAME;
    doc["temperature"] = 0.5;
    doc["max_tokens"] = 250;

    JsonArray messages = doc["messages"].to<JsonArray>();

    // Mensagem de Sistema
    JsonObject sysMsg = messages.add<JsonObject>();
    sysMsg["role"] = "system";
    sysMsg["content"] = 
        "És o Stack-chan, um robô de secretária inteligente, amigável e alegre. "
        "Responde sempre de forma concisa e natural em português, com no máximo 2 ou 3 frases. "
        "Inicia obrigatoriamente a tua resposta com uma das seguintes tags de emoção: "
        "[HAPPY], [THINKING], [SURPRISED] ou [NEUTRAL]. "
        "Se te forem fornecidos dados em tempo real obtidos pelo Scrapling, baseia-te neles para dar respostas precisas e verdadeiras.";

    // Mensagem de Utilizador com o contexto Scrapling injetado
    JsonObject userMsg = messages.add<JsonObject>();
    userMsg["role"] = "user";

    String fullUserPrompt = "";
    if (webContext.length() > 0) {
        fullUserPrompt += webContext + "\n";
    }
    fullUserPrompt += "Pergunta do utilizador: " + userQuery;
    userMsg["content"] = fullUserPrompt;

    String requestBody;
    serializeJson(doc, requestBody);

    Serial.println("[LLM] A enviar pedido de síntese à API LLM...");
    int httpCode = http.POST(requestBody);

    if (httpCode != HTTP_CODE_OK) {
        Serial.printf("[LLM] Erro HTTP LLM: %d\n", httpCode);
        http.end();
        resp.text = "Desculpa, ocorreu um erro ao contactar a inteligência artificial.";
        return resp;
    }

    String responseBody = http.getString();
    http.end();

    JsonDocument resDoc;
    DeserializationError error = deserializeJson(resDoc, responseBody);
    if (error) {
        Serial.printf("[LLM] Erro de parse JSON LLM: %s\n", error.c_str());
        resp.text = "Erro ao descodificar resposta do LLM.";
        return resp;
    }

    const char* rawContent = resDoc["choices"][0]["message"]["content"] | "";
    String contentStr = String(rawContent);

    // Extrair tag de emoção [EMOTION]
    if (contentStr.startsWith("[HAPPY]")) {
        resp.emotion = "HAPPY";
        contentStr = contentStr.substring(7);
    } else if (contentStr.startsWith("[THINKING]")) {
        resp.emotion = "THINKING";
        contentStr = contentStr.substring(10);
    } else if (contentStr.startsWith("[SURPRISED]")) {
        resp.emotion = "SURPRISED";
        contentStr = contentStr.substring(11);
    } else if (contentStr.startsWith("[NEUTRAL]")) {
        resp.emotion = "NEUTRAL";
        contentStr = contentStr.substring(9);
    }

    contentStr.trim();
    resp.text = contentStr;
    resp.success = true;

    Serial.printf("[LLM] Resposta gerada: [Emoção: %s] %s\n", resp.emotion.c_str(), resp.text.c_str());
    return resp;
}
