#include "sortctx.h"
#include <stdlib.h>

void insertionSortRange(SortCtx *ctx, size_t lo, size_t hi) {
    if (hi <= lo + 1) return;
    for (size_t i = lo + 1; i < hi; i++) {
        sortMove(ctx, ctx->tmp, sortElemAt(ctx, i));
        size_t j = i;
        while (j > lo && sortCompareTmp(ctx, j - 1) < 0) {
            sortMove(ctx, sortElemAt(ctx, j), sortElemAt(ctx, j - 1));
            j--;
        }
        sortMove(ctx, sortElemAt(ctx, j), ctx->tmp);
    }
}

void insertionSort(void *base, size_t n, size_t size, SortCompare cmp, SortStats *stats) {
    if (n <= 1) return;
    char *tmp = (char *)malloc(size);
    if (!tmp) return;

    SortCtx ctx = {
        .base = (char *)base,
        .size = size,
        .cmp = cmp,
        .stats = stats,
        .tmp = tmp,
        .currentDepth = 1
    };

    if (stats) {
        stats->extraBytes = size;
        stats->maxDepth = 1;
    }

    insertionSortRange(&ctx, 0, n);
    free(tmp);
}