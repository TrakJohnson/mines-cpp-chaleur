#include <vector>
#include <math.h>
#include "doctest.h"
#include "calc/ode.h"

using namespace std;

using Matrix = DenseMatrix;

TEST_CASE("initialize solver") {
  Matrix v1(vector<vector<double>> {{1.}});
  ODESolver<Matrix> solver(0., 1., 0.1, v1);
  // test sur x' = x, x(0) = 1
  Matrix r = solver.solve_euler_explicit([](Matrix x) -> Matrix { return x; });
  Matrix exp_mat = Matrix([](int i, [[maybe_unused]] int j) ->
			    double { return exp(i/10.);}, 10, 1);
  Matrix diff = exp_mat - r;
  // vérification que l'erreur n'est pas trop grande
  CHECK(diff(9, 0) < 0.11);
}

// TEST_CASE("boundary conditions") {
//   Matrix v1(vector<vector<double>> {{0.}, {1.}, {0.}});
//   ODESolver solver(0., 1., 0.2, v1);
//   // test sur x' = x, x(0) = 1
//   Matrix r = solver.solve_euler_explicit([](Matrix x) -> Matrix { return x; },
// 					 true, 0., 0.);
//   CHECK(r(r.shape().first - 1, 0) == 0);
//   CHECK(r(r.shape().first - 1, 2) == 0);
// }


