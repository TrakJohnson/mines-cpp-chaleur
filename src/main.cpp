#include <iostream>
#include <cmath>
#include "calc/matrix.h"
#include "calc/heat.h"
#include "utils/file.h"

using namespace std;

void question2() {
  HeatSystem1D sys([]([[maybe_unused]] double x) -> double { return 1.; },
		   [](double x) -> double {
		     return 0.5 + sin(2 * M_PI * x) - 0.5 * cos(2 * M_PI * x);
		   },
		   // TODO le choix de dt et dx est motivé dans le README
		   0., .5, 0.005,  // t
		   0., 1., 0.1,    // x
		   true, 0., 0.);  // conditions au bord
  Matrix r = sys.solve_explicit();
  writeMatrixToFile(r, sys, "euler_explicite.txt");
}

int main() {
  question2();
  return EXIT_SUCCESS;
}
