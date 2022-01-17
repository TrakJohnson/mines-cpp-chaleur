#include "calc/matrix.h"
#include "calc/solve.h"
#include "doctest.h"
#include <iostream>
#include <time.h>
#include <vector>

// #define SPARSE false

using namespace std;

using Matrix = SparseMatrix;

TEST_CASE("initialize matrix") {
  if (SPARSE) {
    cout << "TESTS: using sparse matrix" << endl;
  } else {
    cout << "TESTS: using dense matrix" << endl;
  }
  // }
  vector<vector<double>> vIn{{0.5, 0.5}, {0.5, 0.5}};
  Matrix m1(vIn);
  Matrix m2(0.5, 2);
  CHECK(m1 == m2);

  Matrix m3(0.5, 3);
  CHECK(m1 != m3);

  Matrix m4([](int i, int j) -> double { return i == j; }, 3, 3);
  CHECK(m4 == Matrix::identity(3));

  Matrix v1{{0., 1., 2.}};
  Matrix v2{v1.transpose()};
  CHECK(v1(0) == v2(0));
}

TEST_CASE("matrix algebra") {
  Matrix m1(0.5, 3);
  Matrix m2(1, 3);
  Matrix id3{Matrix::identity(3)};

  SUBCASE("addition") { CHECK(m1 + m1 == m2); }

  SUBCASE("substraction") { CHECK((m2 - m1) == m1); }

  SUBCASE("product") {
    SUBCASE("matrix product") {
      Matrix v1(1, 3, 1);
      CHECK(id3 * v1 == v1);
      CHECK(m1 * id3 == m1);

      Matrix m3({{1., 2., 3.}, {2., 3., 4.}});
      Matrix m4({{2., 3.}, {3., 4.}, {4., 5.}});
      Matrix m34({{20., 26.}, {29., 38.}});
      Matrix m43({{8., 13., 18.}, {11., 18., 25.}, {14., 23., 32.}});
      CHECK(m3 * m4 == m34);
      CHECK(m4 * m3 == m43);

      CHECK(m1 * Matrix(0., 3) == Matrix(0., 3));
    }

    SUBCASE("product with a scalar") {
      CHECK(2. * Matrix::identity(2) == Matrix({{2., 0.}, {0., 2.}}));
      CHECK(2 * m1 == m2);
      CHECK(id3 * 3. == 3. * id3);
    }
  }

  SUBCASE("norm") {
    Matrix v1(2., 4, 1);
    CHECK(v1.norm() == 4.);
    CHECK(v1.norm() == v1.transpose().norm());
  }
}

TEST_CASE("matrix methods") {
  SUBCASE("transposition") {
    Matrix m1({{0., -1.}, {1., 0.}});
    CHECK(m1.transpose() == (-1) * m1);
  }
  SUBCASE("partial overwrite") {
    Matrix m1{Matrix::identity(3)};
    Matrix m2({{1., 0., 0.}, {4., 4., 2.}, {0., 0., 1.}});
    m1.setLine(1, vector<double>{4., 4., 2.});
    CHECK(m1 == m2);
  }
}

TEST_CASE("solver") {
  SUBCASE("conjugate gradient method") {
    // m1 doit être définie positive
    Matrix m1{{{1., 2., 3.}, {2., 3., 4.}, {3., 4., 5.}}};
    Matrix m2{m1 + 2. * Matrix::identity(3)};
    Matrix b1(3., 3, 1);
    Matrix x01(1., 3, 1);

    auto randMatrix = [](int l, int c) -> Matrix {
      srand(time(NULL));
      return Matrix(
          []([[maybe_unused]] int i, [[maybe_unused]] int j) -> double {
            return rand() % 100 - 50;
          },
          l, c);
    };

    double epsilon = 0.001;
    Matrix sol1 = solveSystemCG(m1, b1, x01, epsilon);
    Matrix sol2 = solveSystemCG(m2, b1, x01, epsilon);
    // la solution doit être juste à epsilon près
    CHECK((m1 * sol1 - b1).norm() < epsilon);
    CHECK((m2 * sol2 - b1).norm() < epsilon);

    // random checks
    int n = 5;
    Matrix m(0., n);
    Matrix b(0., n, 1);
    Matrix x0(0., n, 1);
    for (int i = 0; i < 10; i++) {
      b = randMatrix(n, 1);
      m = randMatrix(n, n);
      CHECK((m * solveSystemCG(m, b, x0) - b).norm() < epsilon);
    }
  }

  SUBCASE("thomas algorithm") {
    Matrix m1{{{-2., 7., 0.}, {2., 2., 4.}, {0., 4., 0.}}};
    Matrix b1(47., 3, 1);
    Matrix sol = solveTridiagonalSystem(m1, b1);
    cout << sol << endl;
    cout << "precision: ";
    cout << (m1 * sol - b1).norm() << endl;
    CHECK((m1 * sol - b1).norm() < 0.001);
  }
}
