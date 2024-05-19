#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;

const int MOD = 1 << 31;

char tmp[1 << 28] = {0};


int main() {
    long long N, S, P, Q;
    cin >> N >> S >> P >> Q;

    long long slow = S % MOD;

    int count = 0;

    for(int i = 0; i < N; ++i){
        long long idx = slow / 8;
        int bitPosition = slow % 8;
        char& c = tmp[idx];
        if(!(c & char(1 << bitPosition))){
            c |= (1 << (bitPosition));
            count++;
        }
        slow = (slow * P + Q) % MOD;
    }

    cout << count << endl;

    return 0;
}

//