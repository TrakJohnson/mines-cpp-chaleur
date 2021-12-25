#include <iostream>
#include <vector>
#include "doctest.h"
#include "calc/matrix.h"

using namespace std;

TEST_CASE("initialize matrix") {
  vector<vector<double>> vIn{{0.5, 0.5}, {0.5, 0.5}};
  Matrix m1(vIn);
  Matrix m2(0.5, 2);
  CHECK(m1 == m2);

  Matrix m3(0.5, 3);
  CHECK(m1 != m3);

  Matrix m4([](int i, int j) -> double { return i == j; }, 3, 3);
  CHECK(m4 == Matrix::identity(3));

  Matrix v1({{0., 1., 2.}});
  Matrix v2{v1.transpose()};
  CHECK(v1(1) == v2(1));
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

    SUBCASE("scalar product") {
      CHECK(2. * Matrix::identity(2) == Matrix({{2., 0.}, {0., 2.}}));
      CHECK(2 * m1 == m2);
      CHECK(id3 * 3. == 3. * id3);
    }
  }
}

TEST_CASE("matrix methods") {
  SUBCASE("extraction") {
    CHECK(Matrix::identity(3).extractCol(1) == Matrix::identity(3).extractLineAsCol(1));
    CHECK(Matrix::identity(3).transpose() == Matrix::identity(3));
  }
  SUBCASE("transposition") {
    Matrix m1({{0., -1.}, {1., 0.}});
    CHECK(m1.transpose() == (-1) * m1); 
  }
  SUBCASE("partial overwrite") {
    Matrix m1{Matrix::identity(3)};
    Matrix m2({
	{1., 0., 0.},
	{4., 4., 2.},
	{0., 0., 1.}
      });
    m1.setLine(1, vector<double> {4., 4., 2.});
    CHECK(m1 == m2);
  }
}
