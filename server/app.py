"""
FastAPI Server for Stack-chan Real-Time Online Search
Wraps Scrapling into high-performance, asynchronous JSON endpoints for the ESP32-S3.
"""

from fastapi import FastAPI, HTTPException, Request
from fastapi.responses import StreamingResponse, JSONResponse
from fastapi.middleware.cors import CORSMiddleware
from pydantic import BaseModel, Field
from typing import Optional, Any, Dict
import httpx
import os
import json

from server.search_engine import search_web_scrapling, fetch_page_markdown

OPENCODE_BASE_URL = os.environ.get("OPENCODE_BASE_URL", "https://opencode.ai/zen/go/v1")
DEFAULT_OPENCODE_KEY = os.environ.get("OPENCODE_API_KEY", "oc_sk_71714a3a9253_iMJa_fgl1reysS9F9UxkX_Q5lw4lXNuZ")

app = FastAPI(
    title="Stack-chan Scrapling & OpenCode Gateway",
    description="Microserviço autónomo de pesquisa Scrapling e gateway OpenCode Go (DeepSeek Flash) para o Stack-chan M5",
    version="1.1.0",
)

# Enable CORS for local and cloud testing
app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)


class SearchRequest(BaseModel):
    query: str = Field(..., description="Termo de pesquisa ou pergunta em linguagem natural")
    limit: int = Field(3, ge=1, le=10, description="Número máximo de páginas a retornar")
    fetch_content: bool = Field(True, description="Se True, extrai o texto em Markdown das páginas")
    max_chars: int = Field(1500, ge=100, le=5000, description="Limite de caracteres por página")


class ScrapeRequest(BaseModel):
    url: str = Field(..., description="URL direta a extrair")
    timeout: int = Field(5, ge=1, le=15, description="Timeout em segundos")
    max_chars: int = Field(3000, ge=100, le=10000, description="Limite de caracteres extraídos")


@app.get("/")
@app.get("/health")
def health_check():
    return {
        "status": "online",
        "service": "stackchan-scrapling-search",
        "version": "1.0.0",
        "engine": "scrapling",
    }


@app.post("/api/search")
def search_endpoint(req: SearchRequest):
    if not req.query.strip():
        raise HTTPException(status_code=400, detail="A query não pode estar vazia.")
    
    result = search_web_scrapling(
        query=req.query,
        limit=req.limit,
        fetch_full_content=req.fetch_content,
        max_content_chars=req.max_chars,
    )
    return {
        "success": True if not result.get("error") else False,
        **result,
    }


@app.post("/api/scrape")
def scrape_endpoint(req: ScrapeRequest):
    if not req.url.startswith("http"):
        raise HTTPException(status_code=400, detail="URL inválida. Deve iniciar com http:// ou https://")
    
    result = fetch_page_markdown(
        url=req.url,
        timeout=req.timeout,
        max_chars=req.max_chars,
    )
    return result


@app.get("/v1/models")
async def list_models(request: Request):
    """Encaminha a listagem de modelos para o OpenCode Go com autenticação."""
    auth_header = request.headers.get("authorization", f"Bearer {DEFAULT_OPENCODE_KEY}")
    headers = {
        "Authorization": auth_header,
        "x-opencode-session": "stackchan-session-001",
    }
    async with httpx.AsyncClient(timeout=10.0) as client:
        res = await client.get(f"{OPENCODE_BASE_URL}/models", headers=headers)
        return JSONResponse(status_code=res.status_code, content=res.json())


@app.post("/v1/chat/completions")
async def chat_completions_proxy(request: Request):
    """
    Gateway OpenAI-compatível para o OpenCode Go (DeepSeek-v4.1-flash).
    Injeta automaticamente o cabeçalho 'x-opencode-session' e suporta streaming.
    """
    try:
        body = await request.json()
    except Exception:
        raise HTTPException(status_code=400, detail="Corpo do pedido JSON inválido")

    # Garante modelo padrão DeepSeek Flash se não for especificado
    if not body.get("model"):
        body["model"] = "deepseek-v4.1-flash"

    auth_header = request.headers.get("authorization", f"Bearer {DEFAULT_OPENCODE_KEY}")
    headers = {
        "Authorization": auth_header,
        "Content-Type": "application/json",
        "x-opencode-session": "stackchan-session-001",
    }

    is_stream = body.get("stream", False)

    if is_stream:
        async def stream_generator():
            async with httpx.AsyncClient(timeout=60.0) as client:
                async with client.stream(
                    "POST",
                    f"{OPENCODE_BASE_URL}/chat/completions",
                    headers=headers,
                    json=body,
                ) as response:
                    async for chunk in response.aiter_raw():
                        yield chunk

        return StreamingResponse(stream_generator(), media_type="text/event-stream")
    else:
        async with httpx.AsyncClient(timeout=60.0) as client:
            res = await client.post(
                f"{OPENCODE_BASE_URL}/chat/completions",
                headers=headers,
                json=body,
            )
            return JSONResponse(status_code=res.status_code, content=res.json())


if __name__ == "__main__":
    import uvicorn
    uvicorn.run("server.app:app", host="0.0.0.0", port=8000, reload=True)

