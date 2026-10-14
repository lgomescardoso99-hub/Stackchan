# Firmware Autónomo do Stack-chan com Pesquisa Web em Tempo Real (Scrapling)

Este firmware permite ao **Stack-chan M5** pesquisar informações online em tempo real na Internet sem qualquer dependência do PC, utilizando o micro-serviço Scrapling e um motor de IA LLM ultrarrápido (Groq / OpenAI / Gemini).

---

## 1. Como Funciona

1. **Ligação Wi-Fi**: Ao ligar à corrente (ou na bateria interna de 550mAh), o Stack-chan liga-se automaticamente à rede Wi-Fi configurada.
2. **Acionamento**: O utilizador toca no sensor capacitivo superior ou no ecrã LCD (ou envia uma questão pelo Serial).
3. **Pesquisa Online em Tempo Real**:
   * O Stack-chan faz um pedido HTTPS direto ao teu **Endpoint Cloud do Scrapling**.
   * O Scrapling pesquisa na web e extrai o conteúdo limpo das páginas em formato Markdown.
4. **Síntese e Animação**:
   * O Stack-chan envia os dados extraídos para o LLM.
   * O robô adota a expressão facial correspondente (`[HAPPY]`, `[THINKING]`, `[SURPRISED]`), acena com a cabeça e mostra a resposta final no ecrã de 2.0 polegadas.

---

## 2. Configuração Antes de Gravar

Abre o ficheiro [`firmware/src/config.h`](src/config.h) e edita:

```cpp
// 1. Rede Wi-Fi 2.4 GHz
#define WIFI_SSID       "O_TEU_WIFI"
#define WIFI_PASSWORD   "A_TUA_PASSWORD"

// 2. URL do teu serviço Scrapling Cloud (Render / Hugging Face ou IP local)
#define SCRAPLING_API_URL "https://teu-servico.onrender.com/api/search"

// 3. Chave de API de LLM (ex.: Groq gratuita em console.groq.com ou OpenAI)
#define LLM_API_KEY     "gsk_tua_chave_groq_aqui"
```

---

## 3. Como Gravar no Stack-chan (COM3)

O teu robô está ligado na porta **`COM3`**. Podes compilar e carregar o firmware de duas formas:

### Opção A: Via PlatformIO (Recomendado)
No terminal da pasta `firmware`:
```bash
pio run --target upload
```

### Opção B: Via Arduino IDE
1. Abre o Arduino IDE.
2. Abre a pasta `firmware/src`.
3. Em **Tools** -> **Board**, seleciona: **`M5CoreS3`**.
4. Em **Tools** -> **Port**, seleciona: **`COM3`**.
5. Certifica-te de que as bibliotecas `M5StackChan`, `M5Unified`, `M5GFX` e `ArduinoJson` estão instaladas.
6. Clica em **Upload** (seta para a direita).

> [!TIP]
> Se o upload não iniciar automaticamente, força o **Modo de Download**: mantém premido o botão **RST** (junto ao microSD na parte inferior) durante **3 segundos** até o **LED verde interior acender**, e solta.

---

## 4. Teste 100% Autónomo (Sem Computador)

1. Após a gravação bem-sucedida, desliga o cabo USB do computador.
2. Liga o Stack-chan a qualquer carregador normal de smartphone ou liga-o pela bateria.
3. Observa o ecrã: o robô exibirá `"Online! Toque no topo para pesquisar."` e fará uma saudação com a cabeça.
4. Toca no sensor superior: o Stack-chan entrará em modo `[THINKING]`, efetuará a pesquisa em tempo real com o Scrapling na cloud, acenará afirmativamente com a cabeça e apresentará a resposta atualizada com a respetiva expressão facial.
