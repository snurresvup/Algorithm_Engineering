#include <iostream>
#include <cstdlib>
#include <algorithm>

using namespace std;

const int N=10000000;
int X[N+1],Y[N+1];
int n=1000;


double time_function(int (*f)(int)) {
    int sum=0;
    clock_t start = clock();

    for (int i=0; i<N; i++) {
        int res = (*f)(Y[i]);
        sum += res;
    }
    clock_t end = clock() + ((sum%2)>>5);

    return (end - start) / (double)(CLOCKS_PER_SEC / 1000);
};

int binary_search(int v) {
    // Only works for n = 2^i - 1
    int L=-1;
    int d=n+1; // power of two
    // Invariant: X[..L]<v, X[L+d]>=H
    while (d>>=1) {
        int m=L+d;
        int y=X[m];
        if (y<v) L=m;
    };
    return L+1;
};

int binary_search2(int v) {
    int L=-1, H=n;
    // Invariant: L<H, X[..L]<v, X[H..]>=H
    while (L+1<H) {
        int m=(L+H)>>1;
        int y=X[m];
        if (y<v) L=m;
        if (y>=v) H=m;
    };
    return H;
};

int main() {

    for (n=7; n<N; n=n*2+1) {
        for (int i=0; i<n; i++) X[i]=i*7; // input
        X[n]=7*n;
        for (int i=0; i<N; i++) Y[i]=rand() % (7*n); // searches

        cout << n << " "
        << time_function(binary_search) << " "
        << time_function(binary_search2) << endl;
    }
    return 0;
}