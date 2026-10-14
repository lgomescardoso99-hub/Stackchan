#include "scrapling_client.h"
#include "config.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>

ScraplingClient::ScraplingClient() {}

bool ScraplingClient::searchWeb(const String& query, String& outContext) {
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("[Scrapling] Erro: Wi-Fi desconectado.");
        outContext = "Erro: Sem ligação Wi-Fi para pesquisa web.";
        return false;
    }

    Serial.printf("[Scrapling] A pesquisar online via Scrapling Cloud: '%s'\n", query.c_str());
    unsigned long startTime = millis();

    WiFiClientSecure secureClient;
    secureClient.setInsecure(); // Permite certificados HTTPS públicos na cloud sem CA embutido

    HTTPClient http;
    http.setTimeout(HTTP_TIMEOUT_MS);

    if (!http.begin(secureClient, SCRAPLING_API_URL)) {
        Serial.println("[Scrapling] Erro ao inicializar conexão HTTP.");
        return false;
    }

    http.addHeader("Content-Type", "application/json");

    // Construção do Payload JSON para a API do Scrapling
    JsonDocument reqDoc;
    reqDoc["query"] = query;
    reqDoc["limit"] = MAX_SEARCH_RESULTS;
    reqDoc["fetch_content"] = true;
    reqDoc["max_chars"] = MAX_CHARS_PER_PAGE;

    String requestBody;
    serializeJson(reqDoc, requestBody);

    int httpCode = http.POST(requestBody);
    _lastLatencyMs = millis() - startTime;

    if (httpCode != HTTP_CODE_OK) {
        Serial.printf("[Scrapling] Falha HTTP: %d (em %lu ms)\n", httpCode, _lastLatencyMs);
        http.end();
        return false;
    }

    String responseBody = http.getString();
    http.end();

    // Descodificação da resposta JSON
    JsonDocument resDoc;
    DeserializationError error = deserializeJson(resDoc, responseBody);
    if (error) {
        Serial.printf("[Scrapling] Erro de parse JSON: %s\n", error.c_str());
        return false;
    }

    int count = resDoc["results_count"] | 0;
    Serial.printf("[Scrapling] Concluído em %lu ms. Resultados extraídos: %d\n", _lastLatencyMs, count);

    if (count == 0) {
        outContext = "Nenhum resultado relevante encontrado na pesquisa web.";
        return true;
    }

    outContext = "=== INFORMAÇÕES PESQUISADAS EM TEMPO REAL (SCRAPLING) ===\n";
    JsonArray results = resDoc["results"].as<JsonArray>();
    int idx = 1;
    for (JsonObject r : results) {
        const char* title = r["title"] | "Sem título";
        const char* url = r["url"] | "";
        const char* content = r["content"] | r["snippet"] | "";

        outContext += String("[Fonte ") + idx + String("] ") + title + "\n";
        outContext += String("URL: ") + url + "\n";
        outContext += String("Conteúdo:\n") + content + "\n\n";
        idx++;
    }
    outContext += "========================================================\n";

    return true;
}
