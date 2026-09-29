#ifndef BENCH_H
#define BENCH_H

#include "sort.h"

typedef struct Record {
    int key;
    int tag;
} Record;

typedef struct BenchResult {
    const char *algoName;
    const char *inputShape;
    size_t n;
    double timeMs;
    SortStats stats;
    int isStableReal;
} BenchResult;

int recordCompare(const void *a, const void *b);
void benchRun(const SortAlgorithm *algo, const char *shapeName, Record *data, size_t n, BenchResult *res);

#endif