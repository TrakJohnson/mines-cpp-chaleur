#include <string>
#include "calc/matrix.h"
#include "calc/heat.h"

template <class Matrix>
void writeMatrixToFile(const DenseMatrix &m, HeatSystem1D<Matrix> sys,
                       string fileName);
