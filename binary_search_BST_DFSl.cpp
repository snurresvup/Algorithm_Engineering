#include <iostream>
#include <unistd.h>

using namespace std;

struct Node{
  int val;
  Node *left;
  Node *right;

  Node(){}
  
  Node(int v){
    this->val = v;
  }

  Node(int v, Node *l, Node *r){
    this->val = v;
    this->left = l;
    this->right = r;
  }
};

const int N=10000000;
int Inputs[N];
int Searches[N];
Node dfsl_array[N];
Node bfs_array[N];
int n = 0; //Set by main

Node* build_BST(int in[], int L, int H, double skew){
  if(L>=H) return NULL;

  int i = (H+L)/(1/skew);
  i = i!=0 ? i : 1;
  
  return new Node(in[i],
		  build_BST(in, L, i-1, skew),
		  build_BST(in, i+1, H, skew));  
}

int tree_to_dfsl(int index, Node* current_node){
  dfsl_array[index] = *current_node;

  int next_index = index+1;
  if(current_node->left != NULL){
    next_index = tree_to_dfsl(next_index, current_node->left);
  }
  if (current_node->right != NULL) {
    next_index = tree_to_dfsl(next_index, current_node->right);
  }
  return next_index;
};

int tree_to_bfs(int index, Node * current_node){
  bfs_array[index] = *current_node;
  tree_to_bfs(index*2+1, current_node->left);
  tree_to_bfs(index*2+2, current_node->right);
  return index;
};

Node binary_search_on_bst(int s, Node array[]) {
  Node current_node = array[0];

  int done = 0;
  while(!done && current_node.val != s){
    done = 1;
    if(current_node.val > s && current_node.left != NULL) {
      current_node = *current_node.left;
      done = 0;
    } else if(current_node.val < s && current_node.right != NULL) {
      current_node = *current_node.right;
      done = 0;
    }
  }

  return current_node;
};

int run_binary_search(Node (*f)(int, Node *)){
  int sum = 0;
  for (int i=0; i<N; i++){
    int res = (*f)(Searches[i], dfsl_array).val;
    sum += res;
  }
  return sum;
};

int main(int argc, char *argv[]){
  if(argc != 3 || atof(argv[2])>=1 || atof(argv[2])<=0){
    cerr << "You must provide the size of the problem, and the desired skew as arguments" << endl;
  }else{
    n = atoi(argv[1]);
    double skew = atof(argv[2]);
    
    for(int i=0; i<=n; i++) Inputs[i]=i*7;
    for(int i=0; i<N; i++) Searches[i]=rand() % (7*n);
    Node * root = build_BST(Inputs, 0, n, skew);

    int last_index = tree_to_dfsl(0, root);
    int index = tree_to_bfs(0, root);

    cout << "Lats index returned by dfsl build: " << last_index << index << endl;

    cout << run_binary_search(binary_search_on_bst) << endl;
  }
};

  
