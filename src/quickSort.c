#include "sortctx.h"
#include <stdlib.h>

static size_t medianOfThree(SortCtx *ctx, size_t a, size_t b, size_t c) {
    if (sortCompareAt(ctx, a, b) > 0) {
        if (sortCompareAt(ctx, b, c) > 0) return b;
        return (sortCompareAt(ctx, a, c) > 0) ? c : a;
    } else {
        if (sortCompareAt(ctx, a, c) > 0) return a;
        return (sortCompareAt(ctx, b, c) > 0) ? c : b;
    }
}

static void quickSortRec(SortCtx *ctx, size_t lo, size_t hi, size_t depth) {
    sortUpdateDepth(ctx, depth);
    while (hi > lo + 1) {
        if (hi - lo <= 16) {
            insertionSortRange(ctx, lo, hi);
            return;
        }

        size_t mid = lo + (hi - lo) / 2;
        size_t pivotIdx = medianOfThree(ctx, lo, mid, hi - 1);
        sortSwap(ctx, pivotIdx, hi - 1);

        size_t i = lo;
        size_t j = hi - 1;

        while (1) {
            while (i < j && sortCompareAt(ctx, i, hi - 1) < 0) i++;
            while (j > i && sortCompareAt(ctx, j - 1, hi - 1) >= 0) j--;
            if (i >= j) break;
            sortSwap(ctx, i, j - 1);
            i++;
            j--;
        }
        sortSwap(ctx, i, hi - 1);

        /* Tail Call Optimization: 작은 구간을 먼저 재귀 호출 */
        if (i - lo < hi - (i + 1)) {
            quickSortRec(ctx, lo, i, depth + 1);
            lo = i + 1;
        } else {
            quickSortRec(ctx, i + 1, hi, depth + 1);
            hi = i;
        }
    }
}

void quickSort(void *base, size_t n, size_t size, SortCompare cmp, SortStats *stats) {
    if (n <= 1) return;
    char *tmp = (char *)malloc(size);
    if (!tmp) return;

    SortCtx ctx = {
        .base = (char *)base,
        .size = size,
        .cmp = cmp,
        .stats = stats,
        .tmp = tmp,
        .currentDepth = 0
    };

    if (stats) {
        stats->extraBytes = size;
        stats->maxDepth = 0;
    }

    quickSortRec(&ctx, 0, n, 1);
    free(tmp);
}