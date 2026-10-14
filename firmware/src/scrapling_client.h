#pragma once

#include <Arduino.h>

class ScraplingClient {
public:
    ScraplingClient();
    
    /**
     * Envia uma query de pesquisa para o Endpoint Cloud do Scrapling
     * @param query Termo ou questão a pesquisar
     * @param outContext String de saída com os resumos e conteúdos das páginas em Markdown
     * @return true se obteve resultados válidos, false em caso de erro
     */
    bool searchWeb(const String& query, String& outContext);

private:
    unsigned long _lastLatencyMs = 0;
};
