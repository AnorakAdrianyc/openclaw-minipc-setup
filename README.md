# openclaw-minipc-setup

> Hardened OpenClaw + full local AI stack on a Morefine M9 Mini PC (Intel N150) — one `docker compose up` on Windows.

**Hardware:** Intel N150 · 16 GB RAM · 2 GB GPU  
**OS:** Windows 11 + Docker Desktop (WSL2)  
**Author:** [AnorakAdrianyc](https://github.com/AnorakAdrianyc)

---

## What's Inside

| File | Purpose |
|------|---------|
| `docker-compose.yml` | Spins up 6 services in one command |
| `openclaw.json` | Hardened agent config — identity, security, TurboQuant, forked repo registry |
| `.env.example` | API key template — copy to `.env` |
| `turboquant/` | C quant engine, N150-optimized, compiled with `-O3 -march=native` |

---

## Services

| Service | Port | Role |
|---------|------|------|
| [OpenClaw](https://github.com/openclaw/openclaw) | 3000 | AI agent gateway — Telegram / WhatsApp / Discord / Web |
| [Ollama](https://github.com/ollama/ollama) | 11434 | Local LLM inference — Llama 4, Qwen, Mistral, DeepSeek, Gemma |
| [Open WebUI](https://github.com/open-webui/open-webui) | 8080 | ChatGPT-style UI with RAG + MCP support |
| [LocalAI](https://github.com/mudler/LocalAI) | 8081 | CPU-only OpenAI-compatible API — LLMs, Whisper, Stable Diffusion |
| [AnythingLLM](https://github.com/Mintplex-Labs/anything-llm) | 3001 | Private RAG workspace — connect docs, GitHub, YouTube, Notion |
| TurboQuant | 5050 | C quant engine — MACD, RSI, backtesting, portfolio optimization |

---

## Quick Start (Windows)

### 1. Prerequisites
- [Docker Desktop](https://www.docker.com/products/docker-desktop/) — install with WSL2 backend
- After install: **Settings → Resources → Memory → 12 GB** *(critical)*
- [Git for Windows](https://git-scm.com/download/win)

### 2. Clone & Configure

```powershell
git clone https://github.com/AnorakAdrianyc/openclaw-minipc-setup.git
cd openclaw-minipc-setup
copy .env.example .env
notepad .env
```

Minimum keys needed in `.env`:
```
ANTHROPIC_API_KEY=your_key   # leave blank to use Ollama locally for free
WEBUI_SECRET_KEY=random_str
JWT_SECRET=random_str
```

### 3. Start

```powershell
docker compose up -d
docker compose ps        # verify all 6 services are Up
```

First boot: ~3–5 min while images download.

### 4. Pull Models

```powershell
docker exec ollama ollama pull llama3.2:3b          # fast, fits N150 easily
docker exec ollama ollama pull nomic-embed-text      # RAG embeddings
docker exec ollama ollama pull qwen2.5:3b            # great for code + math
docker exec ollama ollama pull mistral:7b-instruct-q4_K_M  # smarter, still fits
```

### 5. Open in Browser

| URL | Service |
|-----|---------|
| http://localhost:3000 | OpenClaw Dashboard |
| http://localhost:8080 | Open WebUI |
| http://localhost:3001 | AnythingLLM |
| http://localhost:8081 | LocalAI API |
| http://localhost:5050 | TurboQuant API |

---

## N150 RAM Budget

| Model | RAM Needed | Status |
|-------|-----------|--------|
| 3B Q4 | ~2.5 GB | ✅ Comfortable |
| 7B Q4_K_M | ~5 GB | ✅ Fine |
| 13B Q4 | ~9 GB | ⚠️ Tight |
| 70B (AirLLM layer-split) | ~2 GB active | ✅ Slow but works |

---

## Forked Repos — Priority Order

### Tier 1 — Use Immediately
| Repo | Purpose |
|------|---------|
| [agenticSeek](https://github.com/AnorakAdrianyc/agenticSeek) | Zero-cost local fallback agent (no API needed) |
| [OpenShell](https://github.com/AnorakAdrianyc/OpenShell) | Hardened sandboxed execution runtime |
| [antigravity-awesome-skills](https://github.com/AnorakAdrianyc/antigravity-awesome-skills) | 1000+ pre-built OpenClaw skills |
| [how-to-build-a-coding-agent](https://github.com/AnorakAdrianyc/how-to-build-a-coding-agent) | Agent internals reference workshop |
| [leaked-system-prompts](https://github.com/AnorakAdrianyc/leaked-system-prompts) | System prompt hardening reference |
| [mise](https://github.com/AnorakAdrianyc/mise) | Dev tools + env var manager for WSL2 |

### Tier 2 — Stack Extensions
| Repo | Purpose |
|------|---------|
| [airllm](https://github.com/AnorakAdrianyc/airllm) | 70B inference with only 2 GB active RAM |
| [BitNet](https://github.com/AnorakAdrianyc/BitNet) | 1-bit LLM — ultra-low memory for N150 |
| [MiroThinker](https://github.com/AnorakAdrianyc/MiroThinker) | Deep research sub-agent skill |
| [Scrapling](https://github.com/AnorakAdrianyc/Scrapling) | Adaptive web extraction hook |
| [NoteLLM](https://github.com/AnorakAdrianyc/NoteLLM) | LLM-backed note memory (Obsidian) |
| [WolframLanguageForJupyter](https://github.com/AnorakAdrianyc/WolframLanguageForJupyter) | Math verification via MCP |
| [nvm](https://github.com/AnorakAdrianyc/nvm) | Node.js version pinning for OpenClaw |

### Tier 3 — AI Models Stack (all in docker-compose)
[ollama](https://github.com/ollama/ollama) · [open-webui](https://github.com/open-webui/open-webui) · [LocalAI](https://github.com/mudler/LocalAI) · [AnythingLLM](https://github.com/Mintplex-Labs/anything-llm) · [llama.cpp](https://github.com/ggerganov/llama.cpp) · [HF Transformers](https://github.com/huggingface/transformers) · [vLLM](https://github.com/vllm-project/vllm)

---

## Free Cloud Options

| Platform | Use For |
|---------|---------|
| Google Colab (free T4) | vLLM, HF Transformers, fine-tuning |
| Hugging Face Spaces | Any Hub model, free inference tier |
| Railway / Render free | Host AnythingLLM or Open WebUI |
| $5/mo VPS | 7B GGUF via `localai:latest-aio-cpu` |

---

## TurboQuant C Engine

```powershell
docker compose build turboquant
curl http://localhost:5050/health
```

Compiled with: `-O3 -march=native -mtune=intel -ffast-math -fopenmp -std=c11`  
Functions: `calculate_sma/ema/macd/rsi/bollinger` · `optimize_portfolio` · `backtest_worker`

---

## Security Checklist

- [ ] `.env` created — never committed to Git (`.gitignore` covers it)
- [ ] Docker Desktop memory capped at 12 GB
- [ ] `WEBUI_SECRET_KEY` and `JWT_SECRET` are unique random strings
- [ ] `sshSandboxing: true` in `openclaw.json`
- [ ] Forbidden commands: `rm -rf`, `sudo`, `mkfs`
- [ ] Audit log enabled in `openclaw.json`

---

## Common Commands

```powershell
docker compose up -d                          # start all
docker compose down                           # stop all
docker compose restart openclaw               # restart one service
docker stats                                  # resource usage
docker compose pull && docker compose up -d   # update all images
docker exec -it openclaw bash                 # shell into container
docker exec ollama ollama list                # list installed models
```

---

## Full Setup Guide

See [SETUP.md](./SETUP.md) for the complete step-by-step walkthrough including AnythingLLM RAG, OpenClaw channel setup, troubleshooting, and all source references.

---

## Sources

[openclaw/openclaw](https://github.com/openclaw/openclaw) · [ollama/ollama](https://github.com/ollama/ollama) · [open-webui/open-webui](https://github.com/open-webui/open-webui) · [mudler/LocalAI](https://github.com/mudler/LocalAI) · [Mintplex-Labs/anything-llm](https://github.com/Mintplex-Labs/anything-llm) · [ggerganov/llama.cpp](https://github.com/ggerganov/llama.cpp) · [Perplexity Search](https://www.perplexity.ai/search/opensource-github-where-all-th-jwFVXzlBRdmXXqwa18pebw#2) · [GEEKOM Mini PC Guide](https://www.geekompc.com/what-is-openclaw-mini-pc-setup/)
