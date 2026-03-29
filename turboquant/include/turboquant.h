// turboquant.h — Memory-optimized quant engine for Intel N150
// Compiled: gcc -O3 -march=native -mtune=intel -ffast-math -fopenmp -std=c11
#ifndef TURBOQUANT_H
#define TURBOQUANT_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <pthread.h>

// ---- Core Data Structures ----

typedef struct {
    double *prices;
    int length;
    size_t allocated_size;
} PriceSeries;

typedef struct {
    double *values;
    int length;
    double period;
} Indicator;

typedef struct {
    double macd_line;
    double signal_line;
    double histogram;
} MACDResult;

// ---- Technical Indicators ----

Indicator*   calculate_sma(PriceSeries *series, int period);
Indicator*   calculate_ema(PriceSeries *series, int period);
MACDResult*  calculate_macd(PriceSeries *series, int fast, int slow, int signal);
double       calculate_rsi(PriceSeries *series, int period);
double       calculate_bollinger_upper(PriceSeries *series, int period, double std_dev);
double       calculate_bollinger_lower(PriceSeries *series, int period, double std_dev);

// ---- Portfolio Optimization ----

double optimize_portfolio(double **returns_matrix, int assets, int days);

// ---- Backtesting ----

typedef struct {
    PriceSeries *data;
    double (*strategy)(PriceSeries*, void*);
    void *params;
    double result;
    double sharpe_ratio;
    double max_drawdown;
} BacktestParams;

void* backtest_worker(void *arg);
int   run_backtest(BacktestParams *params);

// ---- Memory Helpers ----

PriceSeries* price_series_alloc(int length);
void         price_series_free(PriceSeries *series);
Indicator*   indicator_alloc(int length);
void         indicator_free(Indicator *ind);

#endif // TURBOQUANT_H
