# Task Contract: Stack-chan Real-Time Online Search with Scrapling (Autonomous)

## Goal
Dotar o Stack-chan M5 (ESP32-S3) da capacidade autónoma de efetuar pesquisas na web em tempo real através do Scrapling com latência mínima, operando via Wi-Fi sem qualquer dependência do PC.

## Non-Goals
- Não executar Python/Scrapling dentro do ESP32-S3 (impossibilidade técnica do SoC).
- Não exigir que o PC permaneça ligado após o deploy inicial e upload do firmware.
- Não embutir o LLM no motor de busca (o motor fornece dados brutos de pesquisa via Scrapling para consumo do Stack-chan).

## Verifiable Acceptance Criteria
1. **Serviço Scrapling (Cloud / Local Server)**:
   - FastAPI endpoint `POST /api/search` responde a consultas em menos de 1.5s com resultados limpos em Markdown.
   - Endpoint `POST /api/scrape` extrai conteúdo com sucesso usando `scrapling`.
   - Execução de teste automatizado `pytest` no servidor com 100% de sucesso.
   - Ficheiros de deploy para a nuvem gratuita (Render / HuggingFace Spaces) validados com `Dockerfile` funcional.
2. **Firmware Stack-chan (ESP32-S3)**:
   - Módulo cliente HTTPS em C++ (`scrapling_client.h`/`.cpp`) com parsing JSON (`ArduinoJson`).
   - Módulo orquestrador de IA/LLM com Tool Calling integrado.
   - Sketch completo para PlatformIO / Arduino IDE pronto para compilação e upload via `COM3`.

## Exact Test Oracle Commands
- `pytest server/tests/ -v` ou script de teste do endpoint Scrapling:
  `python server/test_search.py`
