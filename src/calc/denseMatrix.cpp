#include "calc/matrix.h"
#include <cassert>
#include <cmath>
#include <iostream>
#include <stdexcept>
#include <tuple>
#include <vector>

using namespace std;

DenseMatrix::DenseMatrix(const DenseMatrix &m) {
  this->content = m.content;
  this->nLines = m.nLines;
  this->nCols = m.nCols;
}

DenseMatrix::DenseMatrix(const double &x, const int &_nLines,
                         const int &_nCols) {
  vector<double> v(_nLines * _nCols, x);
  this->content = v;
  this->nLines = _nLines;
  this->nCols = _nCols;
}

DenseMatrix::DenseMatrix(const double &x, const int &_nLinesCols) : DenseMatrix(x, _nLinesCols, _nLinesCols){
}

DenseMatrix::DenseMatrix(const vector<vector<double>> &vIn) {
  // TODO must check
  // - none of the dimensions is 0
  // - each line has same length
  this->nLines = vIn.size();
  this->nCols = vIn.at(0).size();

  vector<double> base(this->nLines * this->nCols, 0);
  this->content = base;
  for (int i = 0; i < nLines; i++) {
    for (int j = 0; j < nCols; j++) {
      this->set(i, j, vIn.at(i).at(j));
    }
  }
}

DenseMatrix::DenseMatrix(const vector<double> &vIn) {
  this->nLines = vIn.size();
  this->nCols = 1;

  vector<double> base(this->nLines * this->nCols, 0);
  this->content = base;
  for (int i = 0; i < nLines; i++) {
    this->set(i, 0, vIn.at(i));
  }
}

DenseMatrix::DenseMatrix(function<double(int i, int j)> f, const int &_nLines,
                         const int &_nCols) {
  this->nLines = _nLines;
  this->nCols = _nCols;
  vector<double> v(_nLines * _nCols);
  this->content = v;
  
  for (int i = 0; i < _nLines; i++) {
    for (int j = 0; j < _nCols; j++) {
      this->set(i, j, f(i, j));
    }
  }
}

DenseMatrix DenseMatrix::identity(int n) {
  DenseMatrix mat(0., n, n);
  for (int i = 0; i < n; i++) {
    mat.set(i, i, 1);
  }
  return mat;
}

pair<int, int> DenseMatrix::shape() const {
  return {this->nLines, this->nCols};
}

double DenseMatrix::operator()(int i, int j) const { return this->get(i, j); }
double DenseMatrix::operator()(int i) const {
  if (this->nCols == 1) {
    return this->get(i, 0);
  } else if (this->nLines == 1) {
    return this->get(0, i);
  } else {
    throw runtime_error(
        "Attempted to get value with vector notation on non-vector");
  }
}

void DenseMatrix::set(int i, int j, double val) {
  if (i >= this->nLines || j >= this->nCols) {
    throw runtime_error("Invalid access: index (" + to_string(i) + "," +
                        to_string(j) + ") on shape (" +
                        to_string(this->nLines) + "," + to_string(this->nCols) +
                        ")");
  }
  this->content[i * this->nCols + j] = val;
}

DenseMatrix DenseMatrix::transpose() const {
  return DenseMatrix([this](int i, int j) -> double { return this->get(j, i); },
                     this->nCols, this->nLines);
}

void DenseMatrix::setLine(int i, const vector<double> &v) {
  for (int j = 0; j < this->nCols; j++) {
    this->set(i, j, v.at(j));
  }
}

void DenseMatrix::setLine(int i, const DenseMatrix &v) {
  for (int j = 0; j < this->nCols; j++) {
    this->set(i, j, v(0, j));
  }
}

// TODO: pourquoi utiliser *this ??
double DenseMatrix::normSquared() {
  if (this->nCols == 1) {
    // Ce calcul donne une matrice 1x1, d'où le "(0)"
    return (this->transpose() * *this)(0);
  } else if (this->nLines == 1) {
    return (*this * this->transpose())(0);
  } else {
    throw logic_error("normSquared only implemented for vectors");
  }
}

double DenseMatrix::norm() { return sqrt(this->normSquared()); }

DenseMatrix operator+(DenseMatrix a, const DenseMatrix &b) {
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

DenseMatrix operator-(const DenseMatrix &a, const DenseMatrix &b) {
  // const DenseMatrix opp{(-1) * b};
  return a + (-1) * b;
}

DenseMatrix operator*(const DenseMatrix &a, const DenseMatrix &b) {
  // TODO: learn how to make a copy
  auto [xa, ya] = a.shape();
  auto [xb, yb] = b.shape();
  if (ya != xb) {
    string s = "Incompatible matrix shapes (multiplication): (" +
               to_string(xa) + "," + to_string(ya) + ") and (" + to_string(xb) +
               "," + to_string(yb) + ")";
    throw runtime_error(s);
  }

  DenseMatrix result(0, a.shape().first, b.shape().second);
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

DenseMatrix operator*(DenseMatrix a, double lambda) {
  auto [l, c] = a.shape();
  for (int i = 0; i < l; i++) {
    for (int j = 0; j < c; j++) {
      a.set(i, j, lambda * a(i, j));
    }
  }
  return a;
}

DenseMatrix operator*(double lambda, const DenseMatrix &a) {
  return a * lambda;
}

bool operator==(const DenseMatrix &a, const DenseMatrix &b) {
  if (a.shape() != b.shape()) {
    return false;
  }

  for (int i = 0; i < a.shape().first; i++) {
    for (int j = 0; j < a.shape().second; j++) {
      if (a(i, j) != b(i, j)) {
        return false;
      }
    }
  }
  return true;
}

bool operator!=(const DenseMatrix &a, const DenseMatrix &b) {
  return !(a == b);
}

ostream &operator<<(ostream &os, const DenseMatrix &m) {
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
