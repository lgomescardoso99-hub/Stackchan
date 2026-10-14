#pragma once

// =============================================================================
// Configurações de Rede Wi-Fi
// =============================================================================
#define WIFI_SSID       "O_TEU_WIFI_2.4GHZ"
#define WIFI_PASSWORD   "A_TUA_PASSWORD"

// =============================================================================
// Configuração do Endpoint Cloud Autónomo do Scrapling
// Pode ser a tua URL do Render, HuggingFace, ou o IP local durante testes.
// Exemplo Cloud: "https://stackchan-scrapling-search.onrender.com/api/search"
// =============================================================================
#define SCRAPLING_API_URL "https://stackchan-scrapling-search.onrender.com/api/search"

// =============================================================================
// Configuração do Fornecedor de IA / LLM (OpenCode Go - DeepSeek Flash)
// =============================================================================
#define LLM_API_URL     "https://opencode.ai/zen/go/v1/chat/completions"
#define LLM_API_KEY     "oc_sk_71714a3a9253_iMJa_fgl1reysS9F9UxkX_Q5lw4lXNuZ"
#define LLM_MODEL_NAME  "deepseek-v4.1-flash"

// =============================================================================
// Parâmetros de Pesquisa e Timeout
// =============================================================================
#define MAX_SEARCH_RESULTS  2
#define MAX_CHARS_PER_PAGE  800
#define HTTP_TIMEOUT_MS     6000
