#pragma once

#include <Arduino.h>
#include "scrapling_client.h"

struct LLMResponse {
    String emotion; // "HAPPY", "THINKING", "SURPRISED", "NEUTRAL"
    String text;    // Resposta falada/impressa
    bool success;
};

class LLMOrchestrator {
public:
    LLMOrchestrator();

    /**
     * Processa a pergunta do utilizador, efetuando pesquisa via Scrapling se necessário,
     * e sintetiza a resposta final através do LLM.
     */
    LLMResponse ask(const String& userQuery, bool requireWebSearch = true);

private:
    ScraplingClient _scrapling;
};
