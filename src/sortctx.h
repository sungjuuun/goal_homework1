#ifndef SORTCTX_H
#define SORTCTX_H

#include "sort.h"
#include <string.h>

typedef struct SortCtx {
    char *base;
    size_t size;
    SortCompare cmp;
    SortStats *stats;
    char *tmp;
    size_t currentDepth;
} SortCtx;

static inline void *sortElemAt(const SortCtx *ctx, size_t idx) {
    return ctx->base + idx * ctx->size;
}

static inline int sortCompareAt(SortCtx *ctx, size_t i, size_t j) {
    if (ctx->stats) ctx->stats->compares++;
    return ctx->cmp(sortElemAt(ctx, i), sortElemAt(ctx, j));
}

static inline int sortCompareElem(SortCtx *ctx, size_t i, const void *elem) {
    if (ctx->stats) ctx->stats->compares++;
    return ctx->cmp(sortElemAt(ctx, i), elem);
}

static inline int sortCompareTmp(SortCtx *ctx, size_t i) {
    if (ctx->stats) ctx->stats->compares++;
    return ctx->cmp(ctx->tmp, sortElemAt(ctx, i));
}

static inline void sortMove(SortCtx *ctx, void *dst, const void *src) {
    if (ctx->stats) ctx->stats->moves++;
    memcpy(dst, src, ctx->size);
}

static inline void sortSwap(SortCtx *ctx, size_t i, size_t j) {
    if (i == j) return;
    void *a = sortElemAt(ctx, i);
    void *b = sortElemAt(ctx, j);
    memcpy(ctx->tmp, a, ctx->size);
    memcpy(a, b, ctx->size);
    memcpy(b, ctx->tmp, ctx->size);
    if (ctx->stats) ctx->stats->moves += 3;
}

static inline void sortUpdateDepth(SortCtx *ctx, size_t depth) {
    if (ctx->stats && depth > ctx->stats->maxDepth) {
        ctx->stats->maxDepth = depth;
    }
}

void insertionSortRange(SortCtx *ctx, size_t lo, size_t hi);

#endif