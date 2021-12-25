#include <cmath>
#include "doctest.h"
#include "calc/heat.h"
#include "utils/file.h"

using namespace std;

TEST_CASE("example") {
  HeatSystem1D sys([]([[maybe_unused]] double x) -> double { return 1.; },
		   [](double x) -> double {
		     return 0.5 + sin(2 * M_PI * x) - 0.5 * cos(2 * M_PI * x);
		   },
		   // TODO le choix de dt et dx est motivé dans le README
		   0., 2., 0.005,  // t
		   0., 1., 0.1,    // x
		   true, 0., 0.);  // conditions
  Matrix r = sys.solve_explicit();
  // cout << r << endl;
  writeMatrixToFile(r, "test.txt");
}
