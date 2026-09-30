#include "bench.h"
#include "sort.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void generateInput(Record *arr, size_t n, const char *shape) {
    srand(42);
    if (strcmp(shape, "무작위") == 0) {
        for (size_t i = 0; i < n; i++) {
            arr[i].key = rand() % (int)(n * 2);
            arr[i].tag = (int)i;
        }
    } else if (strcmp(shape, "정렬됨") == 0) {
        for (size_t i = 0; i < n; i++) {
            arr[i].key = (int)i;
            arr[i].tag = (int)i;
        }
    } else if (strcmp(shape, "역순") == 0) {
        for (size_t i = 0; i < n; i++) {
            arr[i].key = (int)(n - i);
            arr[i].tag = (int)i;
        }
    } else if (strcmp(shape, "중복많음") == 0) {
        for (size_t i = 0; i < n; i++) {
            arr[i].key = rand() % 10;
            arr[i].tag = (int)i;
        }
    }
}

int main(int argc, char **argv) {
    int csvMode = (argc > 1 && strcmp(argv[1], "--csv") == 0);

    const char *shapes[] = {"무작위", "정렬됨", "역순", "중복많음"};
    size_t shapesCount = 4;
    size_t testN = 10000;

    if (csvMode) {
        printf("Algorithm,Shape,N,TimeMs,Compares,Moves,ExtraBytes,MaxDepth,StableReal\n");
    } else {
        printf("===========================================================================================\n");
        printf("                       정렬 알고리즘 성능 및 안정성 비교 결과 (N=%zu)\n", testN);
        printf("===========================================================================================\n");
        printf("%-15s %-10s %-10s %-12s %-12s %-10s %-10s %-8s\n",
               "알고리즘", "입력모양", "시간(ms)", "비교횟수", "이동횟수", "메모리(B)", "재귀깊이", "안정성실측");
        printf("-------------------------------------------------------------------------------------------\n");
    }

    for (size_t s = 0; s < shapesCount; s++) {
        Record *data = (Record *)malloc(testN * sizeof(Record));
        generateInput(data, testN, shapes[s]);

        for (size_t a = 0; a < SORT_ALGORITHMS_COUNT; a++) {
            BenchResult res;
            benchRun(&SORT_ALGORITHMS[a], shapes[s], data, testN, &res);

            if (csvMode) {
                printf("%s,%s,%zu,%.3f,%zu,%zu,%zu,%zu,%s\n",
                       res.algoName, res.inputShape, res.n, res.timeMs,
                       res.stats.compares, res.stats.moves, res.stats.extraBytes,
                       res.stats.maxDepth, res.isStableReal ? "O" : "X");
            } else {
                printf("%-15s %-10s %-10.3f %-12zu %-12zu %-10zu %-10zu %-8s\n",
                       res.algoName, res.inputShape, res.timeMs,
                       res.stats.compares, res.stats.moves, res.stats.extraBytes,
                       res.stats.maxDepth, res.isStableReal ? "안정" : "불안정");
            }
        }
        free(data);
        if (!csvMode) printf("-------------------------------------------------------------------------------------------\n");
    }

    return 0;
}