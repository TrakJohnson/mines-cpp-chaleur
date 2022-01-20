#include <iostream>
#include <fstream>
#include "calc/matrix.h"
#include "calc/heat.h"

template <class Matrix>
void writeMatrixToFile(const DenseMatrix &m, HeatSystem1D<Matrix> sys, string fileName) {
  ofstream f;
  f.open(fileName);
  f << sys.time0 << " " << sys.timeN << " " << sys.deltaTime << endl;
  f << sys.x0 << " " << sys.xN << " " << sys.deltaX << endl;
  f << m; 
  f.close();
}

template void writeMatrixToFile(const DenseMatrix &m,
                                HeatSystem1D<DenseMatrix> sys, string fileName);
template void writeMatrixToFile(const DenseMatrix &m, HeatSystem1D<SparseMatrix> sys, string fileName);
