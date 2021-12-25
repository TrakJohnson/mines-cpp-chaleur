#include <iostream>
#include <fstream>
#include "calc/matrix.h"
#include "calc/heat.h"

void writeMatrixToFile(const Matrix &m, HeatSystem1D sys, string fileName) {
  ofstream f;
  f.open(fileName);
  f << sys.time0 << " " << sys.timeN << " " << sys.deltaTime << endl;
  f << sys.x0 << " " << sys.xN << " " << sys.deltaX << endl;
  f << m; 
  f.close();
}
