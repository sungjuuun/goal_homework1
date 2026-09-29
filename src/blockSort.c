#include "sortctx.h"
#include <stdlib.h>
#include <assert.h>

#define BLOCK_SIZE 32

static void reverseRange(SortCtx *ctx, size_t lo, size_t hi) {
    while (lo + 1 < hi) {
        sortSwap(ctx, lo, hi - 1);
        lo++;
        hi--;
    }
}

static void rotateRange(SortCtx *ctx, size_t lo, size_t mid, size_t hi) {
    if (lo >= mid || mid >= hi) return;
    reverseRange(ctx, lo, mid);
    reverseRange(ctx, mid, hi);
    reverseRange(ctx, lo, hi);
}

static size_t lowerBound(SortCtx *ctx, size_t lo, size_t hi, size_t valIdx) {
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (sortCompareAt(ctx, mid, valIdx) < 0) lo = mid + 1;
        else hi = mid;
    }
    return lo;
}

static size_t upperBound(SortCtx *ctx, size_t lo, size_t hi, size_t valIdx) {
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (sortCompareAt(ctx, valIdx, mid) >= 0) lo = mid + 1;
        else hi = mid;
    }
    return lo;
}

static void mergeInPlace(SortCtx *ctx, size_t lo, size_t mid, size_t hi, size_t depth) {
    sortUpdateDepth(ctx, depth);
    if (lo >= mid || mid >= hi) return;
    if (sortCompareAt(ctx, mid - 1, mid) <= 0) return;

    size_t len1 = mid - lo;
    size_t len2 = hi - mid;

    if (len1 >= len2) {
        size_t mid1 = lo + len1 / 2;
        size_t mid2 = lowerBound(ctx, mid, hi, mid1);
        rotateRange(ctx, mid1, mid, mid2);
        size_t newMid = mid1 + (mid2 - mid);
        assert(newMid > lo || mid2 > mid);
        mergeInPlace(ctx, lo, mid1, newMid, depth + 1);
        mergeInPlace(ctx, newMid, mid2, hi, depth + 1);
    } else {
        size_t mid2 = mid + len2 / 2;
        size_t mid1 = upperBound(ctx, lo, mid, mid2);
        rotateRange(ctx, mid1, mid, mid2);
        size_t newMid = mid1 + (mid2 - mid);
        mergeInPlace(ctx, lo, mid1, newMid, depth + 1);
        mergeInPlace(ctx, newMid, mid2 + 1, hi, depth + 1);
    }
}

void blockSort(void *base, size_t n, size_t size, SortCompare cmp, SortStats *stats) {
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

    for (size_t i = 0; i < n; i += BLOCK_SIZE) {
        size_t hi = (i + BLOCK_SIZE < n) ? i + BLOCK_SIZE : n;
        insertionSortRange(&ctx, i, hi);
    }

    for (size_t block = BLOCK_SIZE; block < n; block *= 2) {
        for (size_t i = 0; i < n; i += 2 * block) {
            size_t mid = (i + block < n) ? i + block : n;
            size_t hi = (i + 2 * block < n) ? i + 2 * block : n;
            mergeInPlace(&ctx, i, mid, hi, 1);
        }
    }

    free(tmp);
}