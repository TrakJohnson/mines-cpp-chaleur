#include <iostream>
#include <fstream>
#include "calc/matrix.h"

void writeMatrixToFile(Matrix m, string fileName) {
  ofstream f;
  f.open(fileName);
  f << m;
  f.close();
}
