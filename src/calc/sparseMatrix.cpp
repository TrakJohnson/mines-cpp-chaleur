#include "calc/matrix.h"
#include "eigen-3.4.0/Eigen/Core"
#include "eigen-3.4.0/Eigen/SparseCore"
#include <cmath>
#include <iostream>
#include <limits>
#include <numeric>

using namespace std;
typedef Eigen::SparseMatrix<double> EigenSparse;

SparseMatrix::SparseMatrix(const SparseMatrix &m) { this->content = m.content; }

SparseMatrix::SparseMatrix(const double &x, const int &_nLines,
                           const int &_nCols) {
  EigenSparse m(_nLines, _nCols);
  this->content = m;
  // En principe, la matrice est creuse donc initialisée avec des 0.
  if (abs(x) > numeric_limits<double>::epsilon()) {
    for (int i = 0; i < _nLines; i++) {
      for (int j = 0; j < _nCols; j++) {
        this->set(i, j, x);
      }
    }
  }
}

SparseMatrix::SparseMatrix(const double &x, const int &_nLinesCols)
    : SparseMatrix(x, _nLinesCols, _nLinesCols) {}

SparseMatrix::SparseMatrix(const vector<vector<double>> &vIn) {
  int nLines = vIn.size();
  int nCols = vIn.at(0).size();
  EigenSparse m(nLines, nCols);
  this->content = m;

  for (int i = 0; i < nLines; i++) {
    for (int j = 0; j < nCols; j++) {
      this->set(i, j, vIn.at(i).at(j));
    }
  }
}

SparseMatrix::SparseMatrix(const vector<double> &vIn) {
  int nLines = vIn.size();
  int nCols = 1;
  EigenSparse m(nLines, nCols);
  this->content = m;

  for (int i = 0; i < nLines; i++) {
    this->set(i, 0, vIn.at(i));
  }
}

SparseMatrix::SparseMatrix(function<double(int i, int j)> f, const int &_nLines,
                           const int &_nCols)
    : SparseMatrix(0., _nLines, _nCols) {
  for (int i = 0; i < _nLines; i++) {
    for (int j = 0; j < _nCols; j++) {
      this->set(i, j, f(i, j));
    }
  }
}

SparseMatrix SparseMatrix::identity(int n) {
  SparseMatrix mat(0., n);
  EigenSparse m(n, n);
  m.setIdentity();
  mat.content = m;
  return mat;
}

pair<int, int> SparseMatrix::shape() const {
  return {this->content.rows(), this->content.cols()};
}

double SparseMatrix::get(int i, int j) const {
  return this->content.coeff(i, j);
}

double SparseMatrix::operator()(int i, int j) const { return this->get(i, j); }
double SparseMatrix::operator()(int i) const {
  if (this->content.cols() == 1) {
    return this->get(i, 0);
  } else if (this->content.rows() == 1) {
    return this->get(0, i);
  } else {
    throw runtime_error(
        "Attempted to get value with vector notation on non-vector");
  }
}

void SparseMatrix::set(int i, int j, double val) {
  this->content.coeffRef(i, j) = val;
}

SparseMatrix SparseMatrix::transpose() const {
  return SparseMatrix(
      [this](int i, int j) -> double { return this->get(j, i); },
      this->content.cols(), this->content.rows());
}

// SparseMatrix SparseMatrix::extractLine(int i) {}

// SparseMatrix SparseMatrix::extractLineAsCol(int i) {}

// SparseMatrix SparseMatrix::extractCol(int j) {}

void SparseMatrix::setLine(int i, const vector<double> &v) {
  for (int j = 0; j < this->content.cols(); j++) {
    this->set(i, j, v.at(j));
  }
}

double SparseMatrix::normSquared() { return this->content.squaredNorm(); }

double SparseMatrix::norm() { return this->content.norm(); }

// TODO: template this
void SparseMatrix::setLine(int i, const SparseMatrix &v) {}

SparseMatrix operator+(SparseMatrix a, const SparseMatrix &b) {
  // On fait *une copie de a*
  auto [xa, ya] = a.shape();
  auto [xb, yb] = b.shape();
  if (xa != xb || ya != yb) {
    // TODO: create a custom exception for matrix incompatibiliy
    // cout << a << endl << b << endl;
    throw runtime_error("Incompatible matrix shapes (addition): (" +
                        to_string(xa) + "," + to_string(ya) + ") and (" +
                        to_string(xb) + "," + to_string(yb) + ")");
  }

  auto [nLines, nCols] = a.shape();
  for (int i = 0; i < nLines; i++) {
    for (int j = 0; j < nCols; j++) {
      a.set(i, j, a(i, j) + b(i, j));
    }
  }
  return a;
}

SparseMatrix operator-(const SparseMatrix &a, const SparseMatrix &b) {
  const SparseMatrix opp{(-1) * b};
  return a + opp;
}

SparseMatrix operator*(const SparseMatrix &a, const SparseMatrix &b) {
  // TODO: learn how to make a copy
  auto [xa, ya] = a.shape();
  auto [xb, yb] = b.shape();
  if (ya != xb) {
    string s = "Incompatible matrix shapes (multiplication): (" +
               to_string(xa) + "," + to_string(ya) + ") and (" + to_string(xb) +
               "," + to_string(yb) + ")";
    throw runtime_error(s);
  }

  SparseMatrix result(0, a.shape().first, b.shape().second);
  for (int i = 0; i < a.shape().first; i++) {
    for (int j = 0; j < b.shape().second; j++) {
      double s{0.};
      for (int k = 0; k < a.shape().second; k++) {
        s += a(i, k) * b(k, j);
      }
      result.set(i, j, s);
    }
  }

  return result;
}

SparseMatrix operator*(SparseMatrix a, double lambda) {
  auto [l, c] = a.shape();
  for (int i = 0; i < l; i++) {
    for (int j = 0; j < c; j++) {
      a.set(i, j, lambda * a(i, j));
    }
  }
  return a;
}

SparseMatrix operator*(double lambda, const SparseMatrix &a) {
  return a * lambda;
}

bool operator==(const SparseMatrix &a, const SparseMatrix &b) {
  if (a.shape() != b.shape()) {
    return false;
  }

  for (int i = 0; i < a.shape().first; i++) {
    for (int j = 0; j < a.shape().second; j++) {
      if (abs(a(i, j) - b(i, j)) > numeric_limits<double>::epsilon()) {
        return false;
      }
    }
  }
  return true;
}

bool operator!=(const SparseMatrix &a, const SparseMatrix &b) {
  return !(a == b);
}

ostream &operator<<(ostream &os, const SparseMatrix &m) {
  auto [l, c] = m.shape();
  for (int i = 0; i < l; i++) {
    for (int j = 0; j < c; j++) {
      os << m(i, j);
      if (j < c - 1) {
        os << " ";
      }
    }
    if (i < (l - 1)) {
      os << endl;
    }
  }
  return os;
}
