"""
Scrapling Search Engine for Stack-chan
Provides ultra-fast real-time web search and page markdown extraction using Scrapling.
"""

import time
import re
from typing import List, Dict, Any, Optional
from concurrent.futures import ThreadPoolExecutor, as_completed
from scrapling.fetchers import Fetcher


def clean_markdown_text(text: str, max_length: int = 2000) -> str:
    """Cleans up markdown content, stripping excessive newlines, navigation junk and truncating."""
    if not text:
        return ""
    # Collapse multiple blank lines
    text = re.sub(r"\n{3,}", "\n\n", text)
    # Remove image links to save bandwidth and token space
    text = re.sub(r"!\[.*?\]\(.*?\)", "", text)
    # Strip leading/trailing whitespaces
    text = text.strip()
    if len(text) > max_length:
        text = text[:max_length] + "\n... [conteúdo truncado para brevidade]"
    return text


def fetch_page_markdown(url: str, timeout: int = 4, max_chars: int = 2000) -> Dict[str, Any]:
    """Fetches a single URL using Scrapling and returns its title and markdown content."""
    try:
        response = Fetcher.get(url, timeout=timeout)
        if response.status == 200:
            md = response.markdown()
            clean_md = clean_markdown_text(md, max_length=max_chars)
            # Try to get title
            title_tag = response.css("title")
            title = title_tag[0].text.strip() if title_tag else url
            return {
                "url": url,
                "status": 200,
                "title": title,
                "content": clean_md,
                "success": True,
            }
        else:
            return {
                "url": url,
                "status": response.status,
                "title": url,
                "content": "",
                "success": False,
            }
    except Exception as e:
        return {
            "url": url,
            "status": 500,
            "title": url,
            "content": f"[Erro ao extrair página: {str(e)}]",
            "success": False,
        }


def search_web_scrapling(
    query: str,
    limit: int = 3,
    fetch_full_content: bool = True,
    max_content_chars: int = 1500,
) -> Dict[str, Any]:
    """
    Performs real-time search using DuckDuckGo Lite via Scrapling Fetcher,
    then fetches top page contents in parallel for instantaneous contextual answering.
    """
    start_time = time.time()
    search_url = "https://lite.duckduckgo.com/lite/"

    try:
        # 1. Fetch search result links
        search_res = Fetcher.post(search_url, data={"q": query}, timeout=5)
        if search_res.status != 200:
            return {
                "query": query,
                "results": [],
                "error": f"Search engine returned status {search_res.status}",
                "total_time_ms": int((time.time() - start_time) * 1000),
            }

        # Extract links and titles
        links = search_res.css("a.result-link")
        snippets = search_res.css("td.result-snippet")

        raw_results = []
        for i, link in enumerate(links[:limit]):
            href = link.attrib.get("href", "")
            title = link.text.strip()
            snippet_text = snippets[i].text.strip() if i < len(snippets) else ""
            if href and href.startswith("http"):
                raw_results.append({
                    "title": title,
                    "url": href,
                    "snippet": snippet_text,
                })

        # 2. Optionally fetch full page markdown in parallel with Scrapling
        final_results = []
        if fetch_full_content and raw_results:
            with ThreadPoolExecutor(max_workers=min(limit, 4)) as executor:
                future_to_item = {
                    executor.submit(
                        fetch_page_markdown,
                        item["url"],
                        timeout=4,
                        max_chars=max_content_chars,
                    ): item
                    for item in raw_results
                }
                for future in as_completed(future_to_item):
                    item = future_to_item[future]
                    try:
                        page_data = future.result()
                        final_results.append({
                            "title": page_data["title"] or item["title"],
                            "url": item["url"],
                            "snippet": item["snippet"],
                            "content": page_data["content"] or item["snippet"],
                        })
                    except Exception:
                        final_results.append({
                            "title": item["title"],
                            "url": item["url"],
                            "snippet": item["snippet"],
                            "content": item["snippet"],
                        })
        else:
            final_results = raw_results

        elapsed_ms = int((time.time() - start_time) * 1000)
        return {
            "query": query,
            "results_count": len(final_results),
            "results": final_results,
            "total_time_ms": elapsed_ms,
        }

    except Exception as e:
        elapsed_ms = int((time.time() - start_time) * 1000)
        return {
            "query": query,
            "results": [],
            "error": str(e),
            "total_time_ms": elapsed_ms,
        }
