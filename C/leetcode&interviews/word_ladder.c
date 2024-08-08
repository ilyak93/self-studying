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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXWORDS 5000

bool wordSeq(char* word1, char* word2){
    int count = 0;
    int len1 = strlen(word1);
    int len2 = strlen(word2);
    int min = len1 < len2 ? len1 : len2;
    for(int i = 0; i < min; i++){
        count += (word1[i] == word2[i]);
    }
    if(count == min && (len2 == len1 + 1 || len1 == len2 + 1)) return true;
    if(count == min-1 && len1 == len2) return true;
    return false;
}

//pre building the graph
int ladderLengthExceedTime(char* beginWord, char* endWord, char** wordList, int wordListSize) {

    int** wordGraph = (int**) malloc(sizeof(int*) * wordListSize);
    int counts[wordListSize];
    for(int i = 0; i < wordListSize; i++){
        int count = 0;
        int* cur = (int*) malloc(sizeof(int) * wordListSize);
        for(int j = 0; j < wordListSize; j++) {
            if(i == j) continue;
            if(wordSeq(wordList[i], wordList[j])){
                cur[count++] = j;
            }
        }
        counts[i] = count;
        wordGraph[i] = (int *) malloc(sizeof(int) * count);
        for(int j = 0; j < count; j++){
            wordGraph[i][j] = cur[j];
        }
        free(cur);
    }

    int stack[MAXWORDS];
    int stackItr = 0;
    for(int i = 0; i < wordListSize; i++){
        if(wordSeq(beginWord, wordList[i]) == 1){
            if(!strcmp(wordList[i], endWord)) {
                return 2;
            }
            stack[stackItr++] = i;
        }
    }

    int distance = 0;
    int itr = 0;
    int curEnd = stackItr;
    bool visited[MAXWORDS] = {false};
    while(itr < curEnd){
        int curIdx = stack[itr++];
        if(!strcmp(wordList[curIdx], endWord)){
            return distance + 2;
        } else {
            for(int k = 0; k < counts[curIdx]; k++){
                if(!visited[wordGraph[curIdx][k]]) {
                    stack[stackItr++] = wordGraph[curIdx][k];
                    visited[wordGraph[curIdx][k]] = true;
                }
            }
        }
        if(itr == curEnd){
            curEnd = stackItr;
            distance++;
        }
    }
    return 0;
}

//pre building the graph
int ladderLengtSlow(char* beginWord, char* endWord, char** wordList, int wordListSize) {

    bool endInList = false;
    for(int i = 0; i < wordListSize; i++){
        if(!strcmp(endWord, wordList[i])) endInList = true;
    }
    if(!endInList) return 0;

    int** wordGraph = (int**) malloc(sizeof(int*) * wordListSize);
    int counts[wordListSize];
    bool tmp[MAXWORDS] = {false};
    for(int i = 0; i < wordListSize; i++){
        if(!strcmp(beginWord, wordList[i]) || !strcmp(endWord, wordList[i])) continue;

        int count = 0;
        int* cur = (int*) malloc(sizeof(int) * wordListSize);
        for(int j = 0; j < wordListSize; j++) {
            if(i == j) continue;
            if(wordSeq(wordList[i], wordList[j]) && !tmp[j] && strcmp(wordList[j], endWord)){
                cur[count++] = j;
                tmp[j] = true;
            }
        }
        counts[i] = count;
        wordGraph[i] = (int *) malloc(sizeof(int) * count);
        for(int j = 0; j < count; j++){
            wordGraph[i][j] = cur[j];
        }
        free(cur);
    }

    int stack[MAXWORDS];
    int stackItr = 0;
    bool visited[MAXWORDS] = {false};
    for(int i = 0; i < wordListSize; i++){
        if(wordSeq(beginWord, wordList[i]) == 1){
            if(!strcmp(wordList[i], endWord)) {
                return 2;
            }
            stack[stackItr++] = i;
            visited[i] = true;
        }
    }

    int distance = 1;
    int itr = 0;
    int curEnd = stackItr;

    while(itr < curEnd){
        int curIdx = stack[itr++];
        if(wordSeq(wordList[curIdx], endWord)){
            return distance + 2;
        } else {
            int k = 0;
            while(k < wordListSize) {
                if(!visited[k] && wordSeq(wordList[curIdx], wordList[k])) {
                    stack[stackItr++] = k;
                    visited[k] = true;
                }
                k++;
            }
        }
        if(itr == curEnd){
            curEnd = stackItr;
            distance++;
        }
    }
    return 0;
}

int ladderLengthFast(char* beginWord, char* endWord, char** wordList, int wordListSize) {
    int stack[MAXWORDS];
    int stackItr = 0;
    for(int i = 0; i < wordListSize; i++){
        if(wordSeq(beginWord, wordList[i]) == 1){
            if(!strcmp(wordList[i], endWord)) {
                return 2;
            }
            stack[stackItr++] = i;
        }
    }

    int distance = 0;
    int itr = 0;
    int curEnd = stackItr;
    bool visited[MAXWORDS] = {false};
    while(itr < curEnd){
        int curIdx = stack[itr++];
        if(!strcmp(wordList[curIdx], endWord)){
            return distance + 2;
        } else {
            int k = 0;
            while(k < wordListSize) {
                if(!visited[k] && wordSeq(wordList[curIdx], wordList[k])) {
                    stack[stackItr++] = k;
                    visited[k] = true;
                }
                k++;
            }
        }
        if(itr == curEnd){
            curEnd = stackItr;
            distance++;
        }
    }
    return 0;
}

int ladderLengthMostFast(char* beginWord, char* endWord, char** wordList, int wordListSize) {
    int stack[MAXWORDS];
    int stackItr = 1;
    int distance = 0;
    int itr = 0;
    int curEnd = stackItr;
    bool visited[MAXWORDS] = {false};
    while(itr < curEnd){
        int curIdx = stack[itr];
        char* curWord = itr == 0 ? beginWord : wordList[curIdx];
        itr++;
        if(!strcmp(curWord, endWord)){
            return distance + 1;
        } else {
            int k = 0;
            while(k < wordListSize) {
                if(!visited[k] && wordSeq(curWord, wordList[k]) && strcmp(wordList[k], beginWord)) {
                    stack[stackItr++] = k;
                    visited[k] = true;
                }
                k++;
            }
        }
        if(itr == curEnd){
            curEnd = stackItr;
            distance++;
        }
    }
    return 0;
}

int main() {
    char beginWord[] = "hit";
    char endWord[] = "cog";
    char* wordList[] = {"hot","hit","cog","dot","dog"};
    int wordListSize = sizeof(wordList) / sizeof(wordList[0]);

    int result = ladderLengthMostFast(beginWord, endWord, wordList, wordListSize);
    printf("Output: %d\n", result);

    return 0;
}