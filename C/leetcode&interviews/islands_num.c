#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include <time.h>
#include <stdbool.h>

bool expand(char** grid, int gridSize, int gridColSize, bool** expanded, int i, int j){
    //bool tmp = expanded[i][j];
    if(grid[i][j] == '0' || expanded[i][j] == true) return false;

    int stackI[gridSize * gridColSize];
    int stackJ[gridSize * gridColSize];
    int itrI = 0;
    int itrJ = 0;
    int curI = i;
    int curJ = j;
    stackI[itrI++] = curI;
    stackJ[itrJ++] = curJ;
    while(itrI > 0){
        curI = stackI[--itrI];
        curJ = stackJ[--itrJ];
        if(expanded[curI][curJ]) continue;
        //printf("%d %d \n", curI, curJ);
        expanded[curI][curJ] = 1;
        if(curI - 1 >= 0 &&
           expanded[curI - 1][curJ] == 0 &&
           grid[curI - 1][curJ] == '1') {
            stackI[itrI++] = curI - 1;
            stackJ[itrJ++] = curJ;
        }
        if(curI + 1 < gridSize &&
           expanded[curI + 1][curJ] == false &&
           grid[curI + 1][curJ] == '1') {
            stackI[itrI++] = curI + 1;
            stackJ[itrJ++] = curJ;
        }
        if(curJ - 1 >= 0 &&
           expanded[curI][curJ - 1] == false &&
           grid[curI][curJ - 1] == '1') {
            stackI[itrI++] = curI;
            stackJ[itrJ++] = curJ - 1;
        }
        if(curJ + 1 < gridColSize &&
           expanded[curI][curJ + 1] == false &&
           grid[curI][curJ + 1] == '1') {
            stackI[itrI++] = curI;
            stackJ[itrJ++] = curJ + 1;
        }
    }
    return true;
}


int numIslands(char** grid, int gridSize, int* gridColSize) {
    int cols = gridColSize[0];

    bool** expanded = (bool**)malloc(gridSize * sizeof(bool*));
    for(int i = 0; i < gridSize; i++) {
        expanded[i] = (bool*)malloc(cols * sizeof(bool));
        for(int j = 0; j < cols; j++) {
            expanded[i][j] = false;
        }
    }

    int iCount = 0;
    for(int i = 0; i < gridSize; i++){
        for(int j = 0; j < cols; j++){
            iCount += expand(grid, gridSize, cols, expanded, i, j);
        }
    }
    return iCount;
}

int main() {
    int gridSize = 4;
    int gridColSize[] = {5, 5, 5, 5};  // Each row has 5 columns

    // Allocate memory for the grid
    char** grid = (char**)malloc(gridSize * sizeof(char*));
    for (int i = 0; i < gridSize; i++) {
        grid[i] = (char*)malloc(gridColSize[i] * sizeof(char));
    }

    // Initialize the grid with the values from Example 1
    char* rows[] = {
            "11110",
            "11010",
            "11000",
            "00000"
    };

    for (int i = 0; i < gridSize; i++) {
        for (int j = 0; j < gridColSize[i]; j++) {
            grid[i][j] = rows[i][j];
        }
    }

    // Call the numIslands function
    int result = numIslands(grid, gridSize, gridColSize);

    printf("Number of islands: %d\n", result);

    // Free allocated memory
    for (int i = 0; i < gridSize; i++) {
        free(grid[i]);
    }
    free(grid);

    return 0;
}