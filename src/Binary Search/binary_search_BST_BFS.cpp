#include <iostream>
#include <unistd.h>


using namespace std;

const int N=10000000;
int Searches[N], Inputs[N];
int n = 0; //Set from args in main

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

int run_binary_search(int (*f)(int)) {
  int sum = 0;
  for (int i=0; i<N; i++){
    int res = (*f)(Searches[i]);
    sum += res;
  }
  return sum;
}

int build_binary_search_tree_BFS(int in_A[], int L, int H, int index){
    if (L >= H) return 0;

    int mid = (L+H)>>1;
    Inputs[index] = in_A[mid-1];
    build_binary_search_tree_BFS(in_A, L, mid-1, index*2+1);
    build_binary_search_tree_BFS(in_A, mid+1, H, index*2+2);

    return 0;
};

int binary_search_on_BST(int s) {
    int current_index = 0;
    int current_node = Inputs[current_index];
    bool done = 0;
    while (!done) {
        done = 1;
        while (current_node > s &&
                (current_index*2+1 < (sizeof(Inputs) / sizeof(*Inputs)))) {
            current_index = current_index * 2 + 1;
            current_node = Inputs[current_index];
            done = 0;
        }
        while (current_node < s &&
                (current_index*2+2 < (sizeof(Inputs) / sizeof(*Inputs)))) {
            current_index = current_index*2+2;
            current_node = Inputs[current_index];
            done = 0;
        }
    }
    return current_index;
};

int main(int argc, char *argv[]){
  if(argc != 2){
    cerr << "You must provide an argument that is a power of 2" << endl;
  }else{
    n = atoi(argv[1]);
    for(int i=0; i<=n; i++) Inputs[i]=i*7;
    for(int i=0; i<N; i++) Searches[i]=rand() % (7*n);
    build_binary_search_tree_BFS(Inputs, 0, n, 0);

    cout << run_binary_search(binary_search_on_BST) << endl;
  }
};
