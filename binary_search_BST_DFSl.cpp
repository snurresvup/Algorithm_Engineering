
#include <iostream>
#include <unistd.h>
#include <math.h>

using namespace std;

struct Node{
  int val;
  Node *left;
  Node *right;

  Node(){}
  
  Node(int v, Node *l, Node *r){
    this->val = v;
    this->left = l;
    this->right = r;
  }
};

const int N=300000;
const long bfs_size = 1000000000000;
int Inputs[N];
int Searches[N];
Node dfsl_array[N];
Node dfsr_array[N];
Node bfs_array[bfs_size];
int n = 0; //Set by main

int run_binary_search(Node (*f)(int, Node *), Node* arr){
  int sum = 0;
  for (int i=0; i<N; i++){
    int res = (*f)(Searches[i], arr).val;
    sum += res;
  }
  return sum;
};

double time(Node (*f)(int, Node*), Node* arr) {
    int sum=0;
    clock_t start = clock();

    for (int i = 0; i<N; i++){
      int res = ((*f)(Searches[i], arr)).val;
      sum += res;
    }
    clock_t end = clock() + ((sum%2)>>5);

    return (end - start) / (double)(CLOCKS_PER_SEC / 1000);
};

Node* build_BST(int in[], int L, int H, double skew){
  if(L>=H) return NULL;

  int i = L + floor((H-L)/(1/skew));

  if(i == L){
    return new Node(in[i],
		    NULL,
		    build_BST(in, L+1, H, skew));
  }
  if(i == H){
    return new Node(in[i],
		    build_BST(in, L, H-1, skew),
		    NULL);
  }
  
  return new Node(in[i],
		  build_BST(in, L, i, skew),
		  build_BST(in, i+1, H, skew));
};

int tree_to_dfsl(int index, Node* current_node){
  dfsl_array[index].val = current_node->val;

  int next_index = index+1;
  if(current_node->left != NULL){
    dfsl_array[index].left = &dfsl_array[next_index];
    next_index = tree_to_dfsl(next_index, current_node->left);
  }
  
  if (current_node->right != NULL) {
    dfsl_array[index].right = &dfsl_array[next_index];
    next_index = tree_to_dfsl(next_index, current_node->right);
  }

  return next_index;
};

int tree_to_dfsr(int index, Node * current_node){
  dfsr_array[index].val = current_node->val;

  int next_index = index+1;
  if(current_node->right != NULL){
    dfsr_array[index].right = &dfsr_array[next_index];
    next_index = tree_to_dfsr(next_index, current_node->right);
  }

  if(current_node->left != NULL){
    dfsr_array[index].left = &dfsr_array[next_index];
    next_index = tree_to_dfsr(next_index, current_node->left);
  }

  return next_index;
};

Node * tree_to_bfs(int index, Node * current_node){
  bfs_array[index].val = current_node->val;

  if(current_node->left != NULL){
    bfs_array[index].left = tree_to_bfs(index*2+1, current_node->left);
  }
  
  if(current_node->right != NULL){
    bfs_array[index].right = tree_to_bfs(index*2+2, current_node->right);
  }

  return &bfs_array[index];
};

int count_nodes(Node * root){
  int count = 1;
  if(root->left != NULL){
    count += count_nodes(root->left);
  }
  if(root->right != NULL){
    count += count_nodes(root->right);
  }
  return count;
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

void print_tree(Node * root){
  cout << root->val << endl;
  if(root->left != NULL){
    print_tree(root->left);
  }
  if(root->right != NULL){
    print_tree(root->right);
  }
}

/*int main(){
  for (int i=0; i<8; i++) Inputs[i]=i;
  Node * root = build_BST(Inputs, 0, 8, 0.3);
  cout << "Number of nodes in tree: " << count_nodes(root) << endl;
  print_tree(root);
  return 1;
  }*/

int main(int argc, char *argv[]){
  if(argc != 3 || atof(argv[2])>=1 || atof(argv[2])<=0){
    cerr << "You must provide the size of the problem, and the desired skew as arguments" << endl;
  }else if(atoi(argv[1]) > 0){
    n = atoi(argv[1]);
    double skew = atof(argv[2]);
    
    for(int i=0; i<=n; i++) Inputs[i]=i*7;
    for(int i=0; i<N; i++) Searches[i]=rand() % (7*n);
    Node * root = build_BST(Inputs, 0, n, skew);

    int last_index = tree_to_dfsl(0, root);
    Node * r = tree_to_bfs(0, root);

    cout << "Root value in bfs tree: " << r->val << " --- Last index from dfsl build: " << last_index << endl;

    cout << run_binary_search(binary_search_on_bst, dfsl_array) << endl;
  }else if(atoi(argv[1]) <= 0){
    cout << "start"<<endl;
    double skew = atof(argv[2]);

    for(n=7; n<N; n=n*2+1){
      for(int i=0; i<n; i++) Inputs[i]=i*7;
      
      for(int i=0; i<N; i++) Searches[i]=rand()%(7*n);

      Node * root = build_BST(Inputs, 0, n, skew);      

      for(int i=0; i<N; i++){
	dfsl_array[i].val = 0;
	dfsl_array[i].left = NULL;
	dfsl_array[i].right = NULL;
      }
      
      tree_to_dfsl(0, root);

      Node * r = tree_to_bfs(0, root);
      if(r == NULL){
	cout << "BFS layout no longer fits" << endl;
      }else{
	cout << "Nodes in BST " << count_nodes(root) << " Nodes in bfs " << count_nodes(&bfs_array[0]) << " Nodes in dfsl " << count_nodes(&dfsl_array[0]) << endl;
	cout << "BFS layout: " << time(binary_search_on_bst, bfs_array);
      }
      
      cout <<", DFSL layout: " << time(binary_search_on_bst, dfsl_array) << endl;
    }
  }
};
