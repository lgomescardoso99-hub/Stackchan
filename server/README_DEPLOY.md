# Guia de Deploy Gratuito do Motor Scrapling na Cloud

Para que o **Stack-chan funcione de forma 100% autónoma** (apenas com o seu Wi-Fi, sem necessidade de ter o computador ligado), o micro-serviço Scrapling pode ser alojado gratuitamente em qualquer uma das seguintes plataformas:

---

## Opção 1: Render (Recomendado — 100% Gratuito)

1. Crie uma conta gratuita em [render.com](https://render.com).
2. Conecte o seu repositório GitHub (onde guardou esta pasta do projeto).
3. Clique em **New** -> **Web Service**.
4. Selecione o repositório do projeto.
5. Em **Runtime**, selecione **Docker**.
6. Defina:
   * **Root Directory**: `server`
   * **Docker Command**: Deixe em branco (o Dockerfile já inclui o comando).
   * **Instance Type**: **Free** ($0/mês).
7. Clique em **Create Web Service**.
8. Em cerca de 2 minutos, o Render fornecerá a sua URL pública HTTPS:
   ```
   https://stackchan-scrapling-search-xxxx.onrender.com
   ```
9. Teste no browser acedendo a: `https://sua-url.onrender.com/health` (deve retornar `{"status": "online"}`).

---

## Opção 2: Hugging Face Spaces (Docker Free)

1. Aceda a [huggingface.co/spaces](https://huggingface.co/spaces) e clique em **Create new Space**.
2. Escolha um nome (ex.: `stackchan-scrapling-search`).
3. Selecione a licença e escolha **Space SDK: Docker** (Blank).
4. Em Hardware, mantenha **Free (CPU basic)**.
5. Copie os ficheiros da pasta `server/` para a raiz do Space.
6. O Hugging Face construirá o container e gerará a URL da API HTTPS:
   ```
   https://<seu-utilizador>-stackchan-scrapling-search.hf.space
   ```

---

## Endpoints Disponíveis para o Stack-chan

* **`POST /api/search`**:
  * Payload:
    ```json
    {
      "query": "Quem ganhou o jogo de futebol ontem à noite?",
      "limit": 2,
      "fetch_content": true,
      "max_chars": 1500
    }
    ```
  * Retorno:
    ```json
    {
      "success": true,
      "query": "...",
      "results_count": 2,
      "results": [
        {
          "title": "...",
          "url": "https://...",
          "snippet": "...",
          "content": "# Resumo dos resultados em Markdown..."
        }
      ],
      "total_time_ms": 940
    }
    ```
* **`POST /api/scrape`**:
  * Extração direta de texto de qualquer página web.
