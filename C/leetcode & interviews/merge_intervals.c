#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Comparison function
int compareIntervals(const void* a, const void* b) {
    int *intr1 = *(int**)a;
    int *intr2 = *(int**)b;
    return intr1[0] - intr2[0];
}


/**
 * Return an array of arrays of size *returnSize.
 * The sizes of the arrays are returned as *returnColumnSizes array.
 * Note: Both returned array and *columnSizes array must be malloced, assume caller calls free().
 */
int** merge(int** intervals, int intervalsSize, int* intervalsColSize, int* returnSize, int** returnColumnSizes) {

    qsort(intervals, intervalsSize, sizeof(int*), compareIntervals);

    int** res = (int**) malloc(sizeof(int*) * intervalsSize);

    intervalsColSize = (int*) malloc (sizeof(int) * intervalsSize);
    *returnColumnSizes = (int*) malloc(sizeof(int) * intervalsSize);

    for(int i = 0; i < intervalsSize; i++){
        intervalsColSize[i] = 2;
        (*returnColumnSizes)[i] = 2;
    }

    int intervalsItr = 0;

    (*returnSize) = 0;
    while(intervalsItr < intervalsSize){
        int start = intervals[intervalsItr][0];
        int curEnd= intervals[intervalsItr][1];
        while(intervalsItr < intervalsSize && intervals[intervalsItr][0] <= curEnd) {
            curEnd = intervals[intervalsItr][1] > curEnd ?
                     intervals[intervalsItr][1] : curEnd;
            intervalsItr++;
        }
        res[(*returnSize)] = (int*) malloc(sizeof(int)*2);
        res[(*returnSize)][0] = start;
        res[(*returnSize)][1] = curEnd;
        (*returnSize)++;
    }
    return res;
}

int main() {
    // Define the intervals
    int intervalsNum = 2;
    int intervalsArray[2][2] = {{4, 5}, {1, 4}, };
    int* intervals[intervalsNum];
    for (int i = 0; i < intervalsNum; i++) {
        intervals[i] = intervalsArray[i];
    }

    // Define intervalsSize
    int intervalsSize = intervalsNum;

    // Define intervalsColSize
    int intervalsColSize[2] = {2, 2};

    // Variables to store the result
    int returnSize = 0;
    int* returnColumnSizes;

    // Call the merge function (assuming it is implemented elsewhere)
    int** mergedIntervals = merge(intervals, intervalsSize, intervalsColSize, &returnSize, &returnColumnSizes);

    // Print the merged intervals
    printf("Merged Intervals:\n");
    for (int i = 0; i < returnSize; i++) {
        printf("[%d, %d]\n", mergedIntervals[i][0], mergedIntervals[i][1]);
        free(mergedIntervals[i]); // Free each merged interval
    }
    free(mergedIntervals); // Free the array of merged intervals
    free(returnColumnSizes); // Free the return column sizes array

    return 0;
}