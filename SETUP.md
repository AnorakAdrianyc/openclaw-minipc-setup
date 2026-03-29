# OpenClaw + AI Stack — Elite Mini M9 Setup Guide
**Hardware:** Morefine M9 Mini PC | Intel N150 (4C/3.6GHz) | 16GB RAM | 2GB GPU  
**OS:** Windows 11 + Docker Desktop (WSL2 backend)  
**GitHub:** [AnorakAdrianyc](https://github.com/AnorakAdrianyc)  
**Last updated:** 2026-03-29

---

## What This Repo Contains

| File | Purpose |
|------|---------|
| `docker-compose.yml` | Spins up the full AI stack in one command |
| `openclaw.json` | Hardened OpenClaw config (agent identity, security, TurboQuant, all forked repos registered) |
| `.env.example` | Template for your API keys — copy to `.env` |
| `turboquant/` | C-based quant finance engine with N150-optimized Dockerfile |

---

## Services Included

| Service | Port | What It Does | GitHub |
|---------|------|-------------|--------|
| **OpenClaw** | 3000 | AI agent gateway — chat via Telegram/WhatsApp/Discord/Slack/Web | [openclaw/openclaw](https://github.com/openclaw/openclaw) |
| **Ollama** | 11434 | Local LLM inference — Llama 4, Phi-4, Mistral, Qwen, DeepSeek, Gemma | [ollama/ollama](https://github.com/ollama/ollama) |
| **Open WebUI** | 8080 | ChatGPT-style UI for Ollama + RAG + MCP support | [open-webui/open-webui](https://github.com/open-webui/open-webui) |
| **LocalAI** | 8081 | OpenAI-compatible CPU API — LLMs, Whisper, Stable Diffusion | [mudler/LocalAI](https://github.com/mudler/LocalAI) |
| **AnythingLLM** | 3001 | Private AI workspace — RAG over docs, GitHub, YouTube, Notion | [Mintplex-Labs/anything-llm](https://github.com/Mintplex-Labs/anything-llm) |
| **TurboQuant** | 5050 | C quant engine — MACD, RSI, backtesting, portfolio optimization | Custom build |

---

## Step 1 — Prerequisites (Windows)

### 1.1 Docker Desktop
1. Download from [docker.com/products/docker-desktop](https://www.docker.com/products/docker-desktop/)
2. Install with **WSL2 backend** (default option)
3. After install: **Settings → Resources → Memory → set to 12 GB**  
   *(Critical for N150 with 16GB — leaves 4GB for Windows OS)*
4. Enable: Settings → General → "Use WSL 2 based engine" ✓

### 1.2 WSL2 (if not already set up)
```powershell
# Run in PowerShell as Administrator
wsl --install
wsl --set-default-version 2
```

### 1.3 Git for Windows
Download from [git-scm.com](https://git-scm.com/download/win) or:
```powershell
winget install Git.Git
```

---

## Step 2 — Clone This Repo

```powershell
# Open PowerShell or Windows Terminal
git clone https://github.com/AnorakAdrianyc/openclaw-minipc-setup.git
cd openclaw-minipc-setup
```

---

## Step 3 — Configure API Keys

```powershell
# Copy the example env file
copy .env.example .env

# Open in Notepad and fill in your keys
notepad .env
```

**Minimum required:**
- `ANTHROPIC_API_KEY` — get from [console.anthropic.com](https://console.anthropic.com)  
- `WEBUI_SECRET_KEY` — any random string (e.g. `openssl rand -hex 32` in WSL)
- `JWT_SECRET` — any random string

> **Zero-cost option:** Leave `ANTHROPIC_API_KEY` blank and OpenClaw will use Ollama local models only. No API bills.

---

## Step 4 — Start the Stack

```powershell
# From the project folder:
docker compose up -d

# Check all services are running:
docker compose ps

# Watch logs:
docker compose logs -f openclaw
docker compose logs -f ollama
```

**First boot takes 3–5 minutes** — Ollama and LocalAI download their base images.

---

## Step 5 — Pull Local AI Models (Ollama)

```powershell
# Recommended for N150 (16GB RAM):

# Best all-rounder — 3B, fast on N150
docker exec ollama ollama pull llama3.2:3b

# Lightweight embeddings for AnythingLLM RAG
docker exec ollama ollama pull nomic-embed-text

# Slightly larger but much smarter — 7B Q4 quantized
docker exec ollama ollama pull mistral:7b-instruct-q4_K_M

# Qwen 2.5 — great for code + math (your background)
docker exec ollama ollama pull qwen2.5:3b

# DeepSeek R1 distill — reasoning tasks, Q4 quantized
docker exec ollama ollama pull deepseek-r1:7b-qwen-distill-q4_K_M

# 1-bit model (ultra-low memory via your BitNet fork)
# docker exec ollama ollama pull bitnet-b1.58-2b
```

**N150 RAM budget for models:**

| Model | VRAM/RAM Needed | Fits on N150? |
|-------|----------------|---------------|
| 3B Q4 | ~2.5 GB | ✅ Comfortable |
| 7B Q4_K_M | ~5 GB | ✅ Fine |
| 13B Q4 | ~9 GB | ⚠️ Tight (close all other apps) |
| 70B (AirLLM layer-split) | ~2 GB active | ✅ Slow but possible |

---

## Step 6 — Access the Services

Open in your browser:

| URL | Service |
|-----|---------|
| [http://localhost:3000](http://localhost:3000) | OpenClaw Web Dashboard |
| [http://localhost:8080](http://localhost:8080) | Open WebUI (ChatGPT-style) |
| [http://localhost:3001](http://localhost:3001) | AnythingLLM Workspace |
| [http://localhost:8081](http://localhost:8081) | LocalAI API (CPU) |
| [http://localhost:11434](http://localhost:11434) | Ollama API |
| [http://localhost:5050](http://localhost:5050) | TurboQuant API |

---

## Step 7 — OpenClaw Agent Setup

The `openclaw.json` in this repo is your hardened config. It is auto-mounted into the container.

### Connect a Messaging Channel

On first launch at [http://localhost:3000](http://localhost:3000):
1. Click **Channels → Add Channel**
2. Choose **Telegram** (recommended — free, reliable on N150)
3. Scan QR or paste bot token from [@BotFather](https://t.me/BotFather)
4. Your agent is now reachable anywhere via Telegram

### Register Skills (antigravity-awesome-skills)

```powershell
# Clone your forked skills repo into the skills folder
git clone --depth=1 https://github.com/AnorakAdrianyc/antigravity-awesome-skills skills
docker compose restart openclaw
```

### Verify Config Loaded

```powershell
docker exec openclaw cat /root/.openclaw/openclaw.json | head -20
```

---

## Step 8 — AnythingLLM RAG Setup

1. Open [http://localhost:3001](http://localhost:3001)
2. Create workspace → **"Research"**
3. Upload documents (PDF, DOCX, TXT) or connect GitHub repo
4. Chat with your documents using local Ollama models — zero API cost

**Connect your GitHub repos:**
- Workspace → Connectors → GitHub → paste repo URL
- Recommended repos to index: `antigravity-awesome-skills`, `leaked-system-prompts`, `how-to-build-a-coding-agent`

---

## Your Forked Repos — Relevance to This Stack

### Tier 1 — Use Immediately

| Repo | How to Use |
|------|-----------|
| [agenticSeek](https://github.com/AnorakAdrianyc/agenticSeek) | Local Manus AI — zero-cost fallback agent when API is offline |
| [OpenShell](https://github.com/AnorakAdrianyc/OpenShell) | Hardened sandboxed execution for OpenClaw tools |
| [antigravity-awesome-skills](https://github.com/AnorakAdrianyc/antigravity-awesome-skills) | 1000+ pre-built skills → drop into `/skills` folder |
| [how-to-build-a-coding-agent](https://github.com/AnorakAdrianyc/how-to-build-a-coding-agent) | Reference workshop for customizing your agent internals |
| [leaked-system-prompts](https://github.com/AnorakAdrianyc/leaked-system-prompts) | System prompt hardening reference |
| [mise](https://github.com/AnorakAdrianyc/mise) | Dev tool + env var manager for your Windows/WSL2 workspace |

### Tier 2 — Stack Extensions

| Repo | How to Use |
|------|-----------|
| [airllm](https://github.com/AnorakAdrianyc/airllm) | Run 70B models with layer-splitting — only 2GB active in RAM |
| [BitNet](https://github.com/AnorakAdrianyc/BitNet) | 1-bit LLM inference — extreme memory efficiency for N150 |
| [MiroThinker](https://github.com/AnorakAdrianyc/MiroThinker) | Deep research sub-agent, register as skill in OpenClaw |
| [Scrapling](https://github.com/AnorakAdrianyc/Scrapling) | Adaptive web extraction hook for agent skills |
| [NoteLLM](https://github.com/AnorakAdrianyc/NoteLLM) | LLM-backed note memory, integrates with Obsidian via OpenClaw |
| [WolframLanguageForJupyter](https://github.com/AnorakAdrianyc/WolframLanguageForJupyter) | Math verification in Jupyter — connect to agent via MCP |
| [nvm](https://github.com/AnorakAdrianyc/nvm) | Node.js version pinning for OpenClaw Node.js runtime |

### Tier 3 — AI Models Stack (Perplexity Search Results)

| Repo | Role in Stack |
|------|-------------|
| [ollama/ollama](https://github.com/ollama/ollama) | **In docker-compose** — primary local inference |
| [open-webui/open-webui](https://github.com/open-webui/open-webui) | **In docker-compose** — ChatGPT UI with RAG + MCP |
| [mudler/LocalAI](https://github.com/mudler/LocalAI) | **In docker-compose** — CPU-only all-modality API |
| [Mintplex-Labs/anything-llm](https://github.com/Mintplex-Labs/anything-llm) | **In docker-compose** — private RAG workspace |
| [ggerganov/llama.cpp](https://github.com/ggerganov/llama.cpp) | C/C++ inference backend used inside Ollama |
| [huggingface/transformers](https://github.com/huggingface/transformers) | Python baseline for model experimentation in Jupyter |
| [vllm-project/vllm](https://github.com/vllm-project/vllm) | GPU cloud inference — use on Google Colab free T4 |

---

## Free Cloud Options (Zero Cost)

| Platform | What to Run | How |
|---------|------------|-----|
| Google Colab (free T4) | vLLM, HF Transformers, fine-tuning | `pip install vllm` → run model server |
| Hugging Face Spaces | Any Hub model inference | Deploy gradio app, free tier |
| Railway free tier | AnythingLLM or Open WebUI | One-click deploy template |
| Any $5/mo VPS | 7B GGUF via LocalAI CPU | `docker run -ti -p 8080:8080 localai/localai:latest-aio-cpu` |

---

## TurboQuant — C Quant Engine

Build and test locally:

```powershell
# Build the TurboQuant container
docker compose build turboquant

# Test endpoint
curl http://localhost:5050/health

# Example: calculate RSI via API
curl -X POST http://localhost:5050/rsi \
  -H "Content-Type: application/json" \
  -d '{"prices": [100,102,101,103,105,104,106], "period": 14}'
```

The C header file is at `turboquant/include/turboquant.h` — functions:
- `calculate_sma()`, `calculate_ema()`, `calculate_macd()`
- `calculate_rsi()`, `calculate_bollinger_upper/lower()`
- `optimize_portfolio()`, `backtest_worker()`

**Compile flags:** `-O3 -march=native -mtune=intel -ffast-math -fopenmp -std=c11`

---

## Hardening Checklist

- [ ] `.env` file created with real keys (`chmod 600 .env` in WSL)
- [ ] `.env` is in `.gitignore` (never push to GitHub)
- [ ] Docker Desktop memory set to 12 GB
- [ ] OpenClaw dashboard password set on first login
- [ ] `WEBUI_SECRET_KEY` and `JWT_SECRET` are unique random strings
- [ ] `sshSandboxing: true` confirmed in `openclaw.json`
- [ ] `filesystemWrite` restricted to safe directories only
- [ ] Forbidden commands list includes `rm -rf`, `sudo`, `mkfs`
- [ ] Audit log enabled in `openclaw.json`

---

## Useful Commands

```powershell
# Start everything
docker compose up -d

# Stop everything  
docker compose down

# Restart single service
docker compose restart openclaw

# View resource usage
docker stats

# Update all images
docker compose pull && docker compose up -d

# Shell into OpenClaw container
docker exec -it openclaw bash

# Shell into Ollama container
docker exec -it ollama bash

# Check Ollama models installed
docker exec ollama ollama list

# Pull a new model
docker exec ollama ollama pull phi4-mini
```

---

## Troubleshooting

**OpenClaw won't start:**
```powershell
docker compose logs openclaw
# Usually missing API key or port conflict on 3000
```

**Ollama runs out of memory:**
- Reduce Docker memory limit or pull a smaller model (3B instead of 7B)
- Set `OLLAMA_MAX_LOADED_MODELS=1` (already in compose file)

**Open WebUI can't reach Ollama:**
```powershell
docker exec open-webui curl http://ollama:11434/api/version
# If this fails, both containers may not be on the same Docker network
docker compose down && docker compose up -d
```

**N150 thermal throttling during heavy inference:**
- Check temps: `docker exec ollama cat /proc/cpuinfo | grep "cpu MHz"`
- Reduce `THREADS=2` in LocalAI environment (already set)
- Use quantized Q4 models, avoid Q8 on N150

**Port already in use:**
```powershell
# Find what's using port 3000
netstat -ano | findstr :3000
# Kill the PID or change the port in docker-compose.yml
```

---

## Sources

- [OpenClaw Official](https://github.com/openclaw/openclaw) — MIT license, 185k+ stars
- [Ollama](https://github.com/ollama/ollama) — Local model inference
- [Open WebUI](https://github.com/open-webui/open-webui) — ChatGPT-style frontend
- [LocalAI](https://github.com/mudler/LocalAI) — CPU-only OpenAI-compatible API
- [AnythingLLM](https://github.com/Mintplex-Labs/anything-llm) — Private RAG workspace, 53k stars
- [llama.cpp](https://github.com/ggerganov/llama.cpp) — C/C++ inference engine
- [Hugging Face Transformers](https://github.com/huggingface/transformers) — Python model library
- [vLLM](https://github.com/vllm-project/vllm) — GPU-optimized production inference
- [GPT4All](https://github.com/nomic-ai/gpt4all) — Privacy-first offline LLMs
- [Jan](https://github.com/janhq/jan) — Offline ChatGPT alternative
- [GEEKOM OpenClaw Mini PC Guide](https://www.geekompc.com/what-is-openclaw-mini-pc-setup/)
- [dev.to — Deploy OpenClaw on Ubuntu 24.04](https://dev.to/sst21/how-to-deploy-openclaw-ai-agent-on-ubuntu-2404-e87)
- [Perplexity Search — Open source AI models on local/cloud](https://www.perplexity.ai/search/opensource-github-where-all-th-jwFVXzlBRdmXXqwa18pebw#2)
