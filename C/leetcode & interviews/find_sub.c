#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <math.h>
/**
 * Note: The returned array must be malloced, assume caller calls free().
 */

int StringCmp(char* a, char* b) {
    int len = strlen(b);
    int idx = 0;
    while(a[idx] == b[idx] && idx < len) idx++;
    return idx;

}

int indexOfWordIsSubstring(char* string, char** words, int wordsSize, bool* used){
    int len = strlen(words[0]);
    for(int i = 0; i < wordsSize; i++){
        if(!used[i] && StringCmp(string, words[i]) == len) return i;
    }
    return -1;

}

int EndIndexOfSubstringInWindow(char *s, int winSize, char** words, int wordsSize){
    bool used[wordsSize];
    for(int i = 0; i < wordsSize; i++) used[i] = false;
    int wordLen = strlen(words[0]);
    int i = 0;
    while(i < winSize){
        int idx = indexOfWordIsSubstring(&s[i], words, wordsSize, used);
        if(idx == -1 || used[idx] == true) return i;
        used[idx] = true;
        i += wordLen;
    }
    return i;
}

int* findSubstring(char* s, char** words, int wordsSize, int* returnSize) {


    *returnSize = 0;
    int len = strlen(s);
    int st = 0;
    int wordLen = strlen(words[0]);
    int a = ceil((double)len / wordsSize);
    int* indices = (int*) malloc(sizeof(int) * ceil((double)len / wordsSize));

    int winSize = wordsSize * wordLen;

    while(st < len){
        int endIndex = EndIndexOfSubstringInWindow(&s[st], winSize, words, wordsSize);
        if(endIndex == wordLen * wordsSize){
            indices[(*returnSize)++] = st;
            //st += wordLen;
        } //else st += endIndex > 0 ? endIndex : wordLen;
        st += 1;
    }

    return indices;

}


int main() {
    clock_t start, end;
    double cpu_time_used;

    // Start timing
    start = clock();
    char* in = "aaa";
    const char* words[] = {"a","a"};
    int wordsSize = 2;
    int retSize = -1;
    int* res = findSubstring(in, words, wordsSize, &retSize);
    // End timing
    end = clock();

    // Calculate the elapsed time
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    printf("Function took %f seconds to execute\n", cpu_time_used);

    for(int i = 0; i < retSize; i++){
        printf(" %d ", res[i]);
    }

    return 0;
}