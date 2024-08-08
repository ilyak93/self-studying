#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include <time.h>

#define MAXBOARDSIZE 9

void totalNQueensAux(int n, bool* rows, bool* cols, bool* diag1, bool* diag2, int curCol, int* count) {
    if(curCol == n) {
        (*count)++;
        return;
    }
    for(int i = 0; i < n; i++){
        int diag1Idx = i - curCol + (n-1);
        int diag2Idx = i + curCol;
        if(rows[i] || diag1[diag1Idx] || diag2[diag2Idx]) continue;
        rows[i] = true;
        cols[curCol] = true;
        diag1[diag1Idx] = true;
        diag2[diag2Idx] = true;
        totalNQueensAux(n, rows, cols, diag1, diag2, curCol+1, count);
        rows[i] = false;
        cols[curCol] = false;
        diag1[diag1Idx] = false;
        diag2[diag2Idx] = false;

    }

}

int totalNQueens(int n) {
    bool rows[MAXBOARDSIZE] = {false};
    bool cols[MAXBOARDSIZE] = {false};
    bool diag1[2*MAXBOARDSIZE-1] = {false};
    bool diag2[2*MAXBOARDSIZE-1] = {false};
    int count = 0;
    totalNQueensAux(n, rows, cols,diag1, diag2, 0, &count);
    return count;

}



int main() {
    clock_t start, end;
    double cpu_time_used;

    // Start timing
    start = clock();

    int in[] = {};

    int res = totalNQueens(4);

    // End timing
    end = clock();

    // Calculate the elapsed time
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    printf("%d\n", res);

    printf("Function took %f seconds to execute\n", cpu_time_used);


    return 0;
}