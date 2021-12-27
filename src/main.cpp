#include <iostream>
#include <cmath>
#include "calc/matrix.h"
#include "calc/heat.h"
#include "utils/file.h"

using namespace std;

double t0{0.};
double tN{.5};
double x0{0.};
double xN{1.};

void question2() {
  HeatSystem1D sys([]([[maybe_unused]] double x) -> double { return 1.; },
		   [](double x) -> double {
		     return 0.5 + sin(2 * M_PI * x) - 0.5 * cos(2 * M_PI * x);
		   },
		   // TODO le choix de dt et dx est motivé dans le README
		   t0, tN, 0.01,  // t
		   x0, xN, 0.01,    // x
		   true, 0., 0.);  // conditions au bord
  Matrix r = sys.solve_explicit();
  writeMatrixToFile(r, sys, "euler_explicite.txt");
}

void question4() {
  HeatSystem1D sys([]([[maybe_unused]] double x) -> double { return 1.; },
		   [](double x) -> double {
		     return 0.5 + sin(2 * M_PI * x) - 0.5 * cos(2 * M_PI * x);
		   },
		   // TODO le choix de dt et dx est motivé dans le README
		   t0, tN, 0.01,  // t
		   x0, xN, 0.01,    // x
		   true, 0., 0.);  // conditions au bord
  Matrix r = sys.solve_implicit();
  writeMatrixToFile(r, sys, "euler_implicite.txt");
}

int main() {
  question2();
  question4();
  return EXIT_SUCCESS;
}
