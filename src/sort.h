#ifndef SORT_H
#define SORT_H

#include <stddef.h>

typedef struct SortStats {
    size_t compares;    /* 비교 횟수 */
    size_t moves;       /* 이동(복사/교환) 횟수 */
    size_t extraBytes;  /* 추가 할당 메모리 (바이트) */
    size_t maxDepth;    /* 최대 재귀 호출 깊이 */
} SortStats;

typedef int (*SortCompare)(const void *a, const void *b);

typedef struct SortAlgorithm {
    const char *name;
    const char *timeComplexity;
    const char *spaceComplexity;
    int stable;         /* 1: 안정 정렬, 0: 불안정 정렬 */
    void (*sort)(void *base, size_t n, size_t size, SortCompare cmp, SortStats *stats);
} SortAlgorithm;

extern const SortAlgorithm SORT_ALGORITHMS[];
extern const size_t SORT_ALGORITHMS_COUNT;

/* 정렬 함수 전역 선언 */
void insertionSort(void *base, size_t n, size_t size, SortCompare cmp, SortStats *stats);
void quickSort(void *base, size_t n, size_t size, SortCompare cmp, SortStats *stats);
void blockSort(void *base, size_t n, size_t size, SortCompare cmp, SortStats *stats);

#endif
