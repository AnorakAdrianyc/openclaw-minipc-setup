FROM gcc:13-bookworm AS builder

WORKDIR /build

COPY include/ ./include/
COPY src/ ./src/

# Compile with N150-optimized flags
RUN gcc -O3 -march=native -mtune=intel -ffast-math -fopenmp \
    -std=c11 \
    -I./include \
    src/turboquant_server.c \
    -o turboquant_server \
    -lm -lpthread

FROM debian:bookworm-slim
WORKDIR /app
COPY --from=builder /build/turboquant_server .
COPY include/ ./include/

EXPOSE 5050
CMD ["./turboquant_server", "--port", "5050"]
