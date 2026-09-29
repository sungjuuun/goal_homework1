#include "../src/sort.h"
#include "../src/bench.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

void runUnitTest() {
    printf("유닛 테스트 실행 중...\n");
    size_t sizes[] = {0, 1, 2, 31, 32, 33, 100, 500};
    size_t numSizes = sizeof(sizes) / sizeof(sizes[0]);

    for (size_t a = 0; a < SORT_ALGORITHMS_COUNT; a++) {
        for (size_t s = 0; s < numSizes; s++) {
            size_t n = sizes[s];
            Record *arr = (Record *)malloc(n * sizeof(Record));
            for (size_t i = 0; i < n; i++) {
                arr[i].key = rand() % 100;
                arr[i].tag = (int)i;
            }

            SortStats stats = {0};
            SORT_ALGORITHMS[a].sort(arr, n, sizeof(Record), recordCompare, &stats);

            for (size_t i = 0; i < n > 0 ? n - 1 : 0; i++) {
                assert(arr[i].key <= arr[i + 1].key);
            }
            free(arr);
        }
        printf("  [PASS] %s 정렬 정상 동작 검증 완료\n", SORT_ALGORITHMS[a].name);
    }
    printf("모든 유닛 테스트 통과!\n");
}

int main() {
    runUnitTest();
    return 0;
}