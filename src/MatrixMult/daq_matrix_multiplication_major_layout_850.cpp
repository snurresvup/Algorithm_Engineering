#include <iostream>
#include <unistd.h>
#include <math.h>

using namespace std;

int MatrixAColumns=5000;
int MatrixARows=3000;
int MatrixBColumns=6000;
int MatrixBRows=5000;
int MatrixCRows=MatrixARows;
int MatrixCColumns=MatrixBColumns;
int** MatrixA;
int** MatrixB;
int** MatrixC;


/*int** matrix_add(int** ma, int** mb, int nbeg, int nend, int mbeg, int mend) {
  for (int i=nbeg; i<nend; i++) {
    for (int j=mbeg; j<mend; j++){
      ma[i][j] += mb[i][j];
    }
  }
  return ma;
}*/

int** matrix_mult(int** ma, int** mb, int** mc, int nbeg, int nend, int mbeg, int mend, int pbeg, int pend) {
  for (int n=nbeg; n<nend;n++) {
    for (int j=pbeg; j<pend; j++) {
      int rescell = 0;
      for (int i=mbeg; i<mend; i++) {
        rescell += ma[n][i]*mb[j][i];
      }
      mc[n][j] += rescell;
    }
  }

  return mc;
}

int** rec_matrix_mult(int** ma, int** mb, int** mc, int nbeg, int nend, int mbeg, int mend, int pbeg, int pend) {
  //if (nend-nbeg == 1 && mend-mbeg == 1 && pend-pbeg == 1) {
  //  mc[nbeg][pbeg] += ma[nbeg][mbeg]*mb[pbeg][mbeg];
  if (nend-nbeg < 850 && mend-mbeg < 850 && pend-pbeg < 850) {
    matrix_mult(ma, mb, mc, nbeg, nend, mbeg, mend, pbeg, pend);
  } else if (nend-nbeg >= mend-mbeg && nend-nbeg >= pend-pbeg) {
    int nmid = nbeg+(nend-nbeg)/2;
    rec_matrix_mult(ma, mb, mc, nbeg, nmid, mbeg, mend, pbeg, pend);
    rec_matrix_mult(ma, mb, mc, nmid, nend, mbeg, mend, pbeg, pend);
  } else if (pend-pbeg >= mend-mbeg && pend-pbeg >= nend-nbeg) {
    int pmid = pbeg+(pend-pbeg)/2;

    rec_matrix_mult(ma, mb, mc, nbeg, nend, mbeg, mend, pbeg, pmid);
    rec_matrix_mult(ma, mb, mc, nbeg, nend, mbeg, mend, pmid, pend);
  } else if (mend-mbeg >= nend-nbeg && mend-mbeg >= pend-pbeg) {
    int mmid = mbeg+(mend-mbeg)/2;

    rec_matrix_mult(ma, mb, mc, nbeg, nend, mbeg, mmid, pbeg, pend);
    rec_matrix_mult(ma, mb, mc, nbeg, nend, mmid, mend, pbeg, pend);
  }
  return mc;
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

  MatrixC = new int*[MatrixCRows];
  for (int m = 0; m<MatrixCRows; m++){
    MatrixC[m] = new int[MatrixCColumns];
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
  int** res = rec_matrix_mult(MatrixA, MatrixB, MatrixC, 0, MatrixARows, 0, MatrixAColumns, 0, MatrixBColumns);

  /*for (int m = 0; m<MatrixBColumns; m++){
    for (int i=0; i<MatrixARows; i++) {
      cout << res[m][i] << " ";
    }
    cout << endl;
  }*/

  double time = (clock() - start) / (double)(CLOCKS_PER_SEC / 1000);
  cout << MatrixC[MatrixARows-1][MatrixBColumns-1] << " " << time << endl;
  return 0;
}
