#include <iostream>
#include <unistd.h>

using namespace std;

class HeavyNode{
  int val;
  HeavyNode * left;
  HeavyNode * right;
  
  HeavyNode(int v, HeavyNode * l, HeavyNode * r){
    this->val = v;
    this->left = l;
    this->right = r;
  }

  int size_of_subtree(HeavyNode * n){

  }
  
}
