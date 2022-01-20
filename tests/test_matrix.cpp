#include "calc/matrix.h"
#include "calc/solve.hpp"
#include "doctest.h"
#include <chrono>
#include <iostream>
#include <time.h>
#include <typeinfo>
#include <vector>

using namespace std;

// Choix du type de matrice pour les tests
// Ce choix ne s'applique que aux tests d'algèbre
// pas aux solvers
using Matrix = SparseMatrix;

TEST_CASE("initialize matrix") {
  if (is_same<Matrix, SparseMatrix>::value) {
    cout << "[TESTS] Using sparse matrix - conjugate gradient not implemented."
         << endl;
  } else {
    cout << "[TESTS] Using dense matrix" << endl;
  }

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

    SUBCASE("Sparse product with dense") {
      SparseMatrix m(2., 2, 2);
      DenseMatrix v(1., 2, 1);
      DenseMatrix res(4., 2, 1);
      CHECK(m * v == res);
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
    vector<double> v{{1., 2., 6.}};
    Matrix m3(v);
    m1.setLine(1, vector<double>{4., 4., 2.});
    m2.setLine(2, m3);
    Matrix m4{{{1., 0., 0.}, {4., 4., 2.}, {0., 0., 1.}}};
    CHECK(m1 == m4);
  }
}

TEST_CASE("solver") {
  double epsilon = 0.001;
  SUBCASE("conjugate gradient method") {
    auto randMatrix = [](int l, int c) -> DenseMatrix {
      srand(time(NULL));
      return DenseMatrix(
          []([[maybe_unused]] int i, [[maybe_unused]] int j) -> double {
            return rand() % 100 - 50;
          },
          l, c);
    };

    // m1 doit être définie positive
    DenseMatrix m1{{{1., 2., 3.}, {2., 3., 4.}, {3., 4., 5.}}};
    DenseMatrix m2{m1 + 2. * DenseMatrix::identity(3)};
    DenseMatrix b1(3., 3, 1);
    DenseMatrix x01(1., 3, 1);

    DenseMatrix sol1 = solveSystemCG(m1, b1, x01, epsilon);
    DenseMatrix sol2 = solveSystemCG(m2, b1, x01, epsilon);
    // la solution doit être juste à epsilon près
    CHECK((m1 * sol1 - b1).norm() < epsilon);
    CHECK((m2 * sol2 - b1).norm() < epsilon);

    // le gradient conjugué semble marcher partout
    int n = 5;
    DenseMatrix m(0., n);
    DenseMatrix b(0., n, 1);
    DenseMatrix x0(0., n, 1);
    for (int i = 0; i < 10; i++) {
      b = randMatrix(n, 1);
      m = randMatrix(n, n);
      CHECK((m * solveSystemCG(m, b, x0) - b).norm() < epsilon);
    }
  }

  SUBCASE("thomas algorithm") {
    SparseMatrix m1{{{-2., 7., 0.}, {2., 2., 4.}, {0., 4., 0.}}};
    DenseMatrix b1(47., 3, 1);
    DenseMatrix sol = solveTridiagonalSystem(m1, b1);

    // cout << sol << endl;
    // cout << "precision: ";
    // cout << (m1 * sol - b1).norm() << endl;
    // CHECK((m1 * sol - b1).norm() < 0.001);

    SparseMatrix m2{{{1., 3., 0.}, {2., 1., 7.}, {0., 4., 1.}}};
    DenseMatrix b2(2., 3, 1);
    sol = solveTridiagonalSystem(m2, b2);

    // cout << sol << endl;
    // cout << "precision: ";
    // cout << (m2 * sol - b2).norm() << endl;
    CHECK((m2 * sol - b2).norm() < 0.001);
  }

  SUBCASE("solver benchmarks") {
    int n = 50;
    DenseMatrix b(0., n, 1);
    DenseMatrix m(0., n);
    DenseMatrix x0(0., n, 1);

    double triMean{0.};
    double cgMean{0.};
    int nTests{10};

    auto randTridiagonalMatrix = [](int l, int c) -> DenseMatrix {
      srand(time(NULL));
      return DenseMatrix(
          [](int i, int j) -> double {
            if (i == j || i - 1 == j || i + 1 == j) {
              return rand() % 100 - 50;
            } else {
              return 0.;
            }
          },
          l, c);
    };

    for (int i = 0; i < nTests; i++) {
      b = randTridiagonalMatrix(n, 1);
      m = randTridiagonalMatrix(n, n);
      auto start = chrono::steady_clock::now();
      CHECK((m * solveSystemCG(m, b, x0, epsilon) - b).norm() < epsilon);
      auto middle = chrono::steady_clock::now();
      CHECK((m * solveTridiagonalSystem(m, b) - b).norm() < epsilon);
      auto end = chrono::steady_clock::now();
      triMean +=
          chrono::duration_cast<chrono::microseconds>(end - middle).count() /
          1000.;
      cgMean +=
          chrono::duration_cast<chrono::microseconds>(middle - start).count() /
          1000.;
    }
    cout << "-- Solver performance tests, n = " << n << endl;
    cout << "Tridiagonal mean: " << (triMean / static_cast<float>(nTests))
         << " ms" << endl;
    cout << "Conjugate gradient mean: " << (cgMean / static_cast<float>(nTests))
         << " ms" << endl;
    cout << "(we take x0 = (0, 0, ...) for conjugate gradient, not optimal)"
         << endl;
  }
}
