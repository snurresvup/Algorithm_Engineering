#include <iostream>
#include <unistd.h>
#include <math.h>

using namespace std;

int MatrixAColumns=5000;
int MatrixARows=3000;
int MatrixBColumns=6000;
int MatrixBRows=5000;
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
        rescell += ma[n][i]*mb[j][i];
      }
      res[n][j] = rescell;
    }
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
      MatrixA[n][j] = (j+n+1)*7;
    }
  }

  // For hver kollonne har vi en pointer til et array med count(rækker) elementer
  MatrixB = new int*[MatrixBColumns];

  for (int m = 0; m<MatrixBColumns; m++){
    MatrixB[m] = new int[MatrixBRows];
    for (int i=0; i<MatrixBRows; i++) {
      MatrixB[m][i] = (m+i+2)*7;
    }
  }
}

int main(int argc, char *argv[]){
  if (argc>1) {
    MatrixAColumns = atoi(argv[1]);
    MatrixBRows = MatrixAColumns;
    MatrixARows = MatrixAColumns/2;
    MatrixBColumns = MatrixBRows*2;
  }
  setupMatrices();
  clock_t start = clock();
  int** res = matrix_mult(MatrixA, MatrixB);

  double time = (clock() - start) / (double)(CLOCKS_PER_SEC / 1000);
  cout << res[MatrixARows-1][MatrixBColumns-1] << " " << time << endl;
  return 0;
}
