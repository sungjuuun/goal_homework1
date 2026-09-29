#include "bench.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int recordCompare(const void *a, const void *b) {
    const Record *ra = (const Record *)a;
    const Record *rb = (const Record *)b;
    if (ra->key < rb->key) return -1;
    if (ra->key > rb->key) return 1;
    return 0;
}

static int checkStability(const Record *arr, size_t n) {
    for (size_t i = 0; i < n - 1; i++) {
        if (arr[i].key == arr[i + 1].key) {
            if (arr[i].tag > arr[i + 1].tag) return 0;
        }
    }
    return 1;
}

void benchRun(const SortAlgorithm *algo, const char *shapeName, Record *data, size_t n, BenchResult *res) {
    Record *work = (Record *)malloc(n * sizeof(Record));
    memcpy(work, data, n * sizeof(Record));

    SortStats stats = {0};

    clock_t start = clock();
    algo->sort(work, n, sizeof(Record), recordCompare, &stats);
    clock_t end = clock();

    res->algoName = algo->name;
    res->inputShape = shapeName;
    res->n = n;
    res->timeMs = ((double)(end - start) * 1000.0) / CLOCKS_PER_SEC;
    res->stats = stats;
    res->isStableReal = checkStability(work, n);

    free(work);
}