#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
#include <map>
#include <sstream>
using namespace std;


int GetTagEndIndex(string line){
    int index = 0;
    while(line[index] != ' ') index++;
    return index - 1;
}

void GetEqualPosEndPos(string& line, const int start, int& equal, int& end){
    int g_count = 0;
    for(int k = start; k < line.size(); k++){
        if(line[k] == '=') equal = k;
        if(line[k] == '"') g_count++;
        if(g_count == 2) {
            end = k - 1;
            break;
        }
    }
    while(line[equal] == '=' || line[equal] == ' ') equal--;
}

int main() {
    int n, q;
    cin >> n >> q;
    cin.ignore();

    map<string, string> attributeMap;
    string currentTag;

    for (int i = 0; i < n; i++) {
        string line;
        getline(cin, line);

        if(line[0] == '<'){
            int curTagEndIndex = GetTagEndIndex(line);
            string tagName = line.substr(1, curTagEndIndex);
            int end = -1;
            int size = line.size();
            while(end < size - 1) {
                int curStart = curTagEndIndex + 2;
                int curEnd = -1;
                int curEqualPos = -1;
                GetEqualPosEndPos(line, curStart, curEqualPos, curEnd);
                string valueName = line.substr(curStart, curEqualPos - curStart + 1);
                end = curEnd + 2;
                int valueStartPos = curEnd - 1;
                while (line[valueStartPos] != '"') valueStartPos--;
                valueStartPos++;
                string value = line.substr(valueStartPos, curEnd - valueStartPos + 1);

                attributeMap[valueName] = value;

                curTagEndIndex = end - 2 + 1;
            }
        }



    }

    for (int i = 0; i < q; i++) {
        string query;
        getline(cin, query);

        if (attributeMap.find(query) != attributeMap.end()) {
            cout << attributeMap[query] << endl;
        } else {
            cout << "Not Found!" << endl;
        }
    }

    return 0;
}