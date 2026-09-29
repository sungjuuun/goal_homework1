#include "sortctx.h"

const SortAlgorithm SORT_ALGORITHMS[] = {
    {"insertionSort", "O(n^2)",       "O(1)", 1, insertionSort},
    {"quickSort",     "O(n log n)",   "O(log n)", 0, quickSort},
    {"blockSort",     "O(n log^2 n)", "O(1)", 1, blockSort},
};

const size_t SORT_ALGORITHMS_COUNT = sizeof(SORT_ALGORITHMS) / sizeof(SORT_ALGORITHMS[0]);