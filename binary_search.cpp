#include <iostream>
#include <unistd.h>

using namespace std;

const int N=10000000;
int Searches[N], Inputs_A[N], Inputs_BST[N];
int n = 0; //Updated in main

double time(int (*f)(int)) {
    int sum=0;
    clock_t start = clock();

    for (int i = 0; i<N; i++){
        int res = (*f)(Searches[i]);
        sum += res;
    }
    clock_t end = clock() + ((sum%2)>>5);

    return (end - start) / (double)(CLOCKS_PER_SEC / 1000);
};

int binary_search(int s) {
    int L=-1, H=n;

    while (L+1<H){
        int mid = (L+H)>>1;
        int val = Inputs_A[mid];
        if(val<s) L=mid;
        else H=mid;
    };
    return H;
};

int binary_search_on_BST(int s) {
    int current_index = 0;
    int current_node = Inputs_BST[current_index];
    bool done = 0;
    while (!done) {
        done = 1;
        while (current_node > s &&
                (current_index*2+1 < (sizeof(Inputs_BST) / sizeof(*Inputs_BST)))) {
            current_index = current_index * 2 + 1;
            current_node = Inputs_BST[current_index];
            done = 0;
        }
        while (current_node < s &&
                (current_index*2+2 < (sizeof(Inputs_BST) / sizeof(*Inputs_BST)))) {
            current_index = current_index*2+2;
            current_node = Inputs_BST[current_index];
            done = 0;
        }
    }
    return current_index;
};

int build_binary_search_tree(int in_A[], int L, int H, int index){
    if (L >= H) return 0;

    int mid = (L+H)>>1;
    Inputs_BST[index] = in_A[mid-1];
    build_binary_search_tree(in_A, L, mid-1, index*2+1);
    build_binary_search_tree(in_A, mid+1, H, index*2+2);

    return 0;
};

int main() {
    for(n=7; n<N; n=n*2+1){
        for (int i=0; i<n; i++) Inputs_A[i]=i*7;
        Inputs_A[n]=7*n;
        for(int i=0; i<N; i++) Searches[i]=rand() % (7*n);
        build_binary_search_tree(Inputs_A, 0, n, 0);
        cout << n << " " << time(binary_search) << " " << time(binary_search_on_BST) << endl;
    }
    return 0;
};