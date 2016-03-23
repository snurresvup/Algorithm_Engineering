#include <iostream>
#include <unistd.h>
#include <math.h>

using namespace std;

const int MatrixAColumns=5;
const int MatrixARows=3;
const int MatrixBColumns=6;
const int MatrixBRows=5;
int** MatrixA;
int** MatrixB;

int** matrix_mult(int** ma, int** mb) {
  // MatrixA's rækker
  int** res = new int*[MatrixARows];
  // MatrixB's kolonner
  for (int n=0; n<MatrixARows;n++) {
    res[n] = new int[MatrixBColumns];
    for (int j=0; j<MatrixBColumns; j++) {
      int rescell = 0;
      for (int i=0; i<MatrixAColumns; i++) {
        rescell += MatrixA[n][i]*MatrixB[i][j];
      }
      res[n][j] = rescell;
      cout << res[n][j] << ' ';
    }
    cout << endl;
  }


  return res;
}

// Plads allokkering. Har MatrixA = new int*[N] og MatrixB = new int*[M] stor betydning?
// Afhængigt af hvordan man placerer de forskellige array oprettelser kan man tilgå data
// fra en anden matrix hvis man løber over sin størrelse

void setupMatrices() {
  MatrixA = new int*[MatrixARows];

  for (int n = 0; n<MatrixARows; n++){
    MatrixA[n] = new int[MatrixAColumns];
    for (int j=0; j<MatrixAColumns; j++) {
      MatrixA[n][j] = j*n;
      cout << MatrixA[n][j] << ' ';
    }
    cout << endl;
  }

  MatrixB = new int*[MatrixBRows];

  for (int m = 0; m<MatrixBRows; m++){
    MatrixB[m] = new int[MatrixBColumns];
    for (int i=0; i<MatrixBColumns; i++) {
      MatrixB[m][i] = m*i+1;
      cout << MatrixB[m][i] << ' ';
    }
    cout << endl;
  }


}

int main(int argc, char *argv[]){
  setupMatrices();
  //cout << MatrixA[9][14] << endl;
  matrix_mult(MatrixA, MatrixB);
  return 0;
}
