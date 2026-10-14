"""
Automated tests for Stack-chan Scrapling API using FastAPI TestClient.
"""

from fastapi.testclient import TestClient
from server.app import app

client = TestClient(app)


def test_health_check():
    response = client.get("/health")
    assert response.status_code == 200
    data = response.json()
    assert data["status"] == "online"
    assert data["engine"] == "scrapling"


def test_search_endpoint():
    response = client.post(
        "/api/search",
        json={"query": "M5Stack ESP32", "limit": 2, "fetch_content": True, "max_chars": 500},
    )
    assert response.status_code == 200
    data = response.json()
    assert data["success"] is True
    assert data["query"] == "M5Stack ESP32"
    assert "results" in data
    assert len(data["results"]) > 0
    first = data["results"][0]
    assert "title" in first
    assert "url" in first
    assert "content" in first


def test_scrape_endpoint():
    response = client.post(
        "/api/scrape",
        json={"url": "https://httpbin.org/html", "timeout": 5, "max_chars": 500},
    )
    assert response.status_code == 200
    data = response.json()
    assert data["success"] is True
    assert "Herman Melville" in data["content"] or "Moby-Dick" in data["content"]


def test_v1_models_proxy():
    response = client.get("/v1/models")
    assert response.status_code == 200
    data = response.json()
    assert "data" in data
    ids = [m["id"] for m in data["data"]]
    assert "deepseek-v4.1-flash" in ids


def test_v1_chat_completions_proxy():
    response = client.post(
        "/v1/chat/completions",
        json={
            "model": "deepseek-v4.1-flash",
            "messages": [
                {"role": "user", "content": "Diz apenas 'OK'"}
            ],
            "max_tokens": 10
        }
    )
    assert response.status_code == 200
    data = response.json()
    assert "choices" in data
    assert len(data["choices"]) > 0


if __name__ == "__main__":
    import pytest
    import sys
    sys.exit(pytest.main(["-v", __file__]))
