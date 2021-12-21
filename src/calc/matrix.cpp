#include <iostream>
#include <stdexcept>
#include <tuple>
#include <vector>
#include "calc/matrix.h"

using namespace std;

Matrix::Matrix(const double &x, const int &_nLines, const int &_nCols) {
  vector<double> v(_nLines * _nCols, x);
  this->content = v;
  this->nLines = _nLines;
  this->nCols = _nCols;
}

Matrix::Matrix(const double &x, const int &_nLinesCols) {
  vector<double> v(_nLinesCols * _nLinesCols, x);
  this->content = v;
  this->nLines = _nLinesCols;
  this->nCols = _nLinesCols;
}


Matrix::Matrix(const vector<vector<double>> &vIn) {
  // v is a vector of lines
  // must check
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

Matrix Matrix::identity(int n) {
  Matrix mat(0., n, n);
  for (int i = 0; i < n; i++) {
    mat.set(i, i, 1);
  }
  return mat;
}

pair<int, int> Matrix::shape() const { return {this->nLines, this->nCols}; }

double Matrix::get(int line, int col) const {
  return this->content.at(this->nCols * line + col);
}

double Matrix::operator()(int i, int j) const { return this->get(i, j); }

void Matrix::set(int i, int j, double val) {
  this->content[i * this->nCols + j] = val;
}

void Matrix::display() const {
  for (int i = 0; i < this->nLines; i++) {
    for (int j = 0; j < this->nCols; j++) {
      cout << this->get(i, j) << " ";
    }
    cout << endl;
  }
}

Matrix operator+(Matrix a, const Matrix &b) {
  // On fait *une copie de a*
  if (a.shape() != b.shape()) {
    throw runtime_error("Incompatible matrix shapes (addition)");
  }

  auto [nLines, nCols] = a.shape();
  for (int i = 0; i < nLines; i++) {
    for (int j = 0; j < nCols; j++) {
      a.set(i, j, a(i, j) + b(i, j));
    }
  }
  return a;
}

Matrix operator-(const Matrix &a, const Matrix &b) {
  const Matrix opp{(-1) * b};
  return a + opp;
}

Matrix operator*(const Matrix &a, const Matrix &b) {
  // TODO: learn how to make a copy
  if (a.shape().second != b.shape().first) {
    throw runtime_error("Incompatible matrix shapes (multiplication)");
  }

  Matrix result(0, a.shape().first, b.shape().second);
  for (int i = 0; i < a.shape().first; i++) {
    for (int j = 0; j < b.shape().second; j++) {
      double s{0.};
      for (int k = 0; k < a.shape().second; k++) {
        s += a.get(i, k) * b.get(k, j);
      }
      result.set(i, j, s);
    }
  }

  return result;
}

Matrix operator*(Matrix a, double lambda) {
  auto [l, c] = a.shape();
  for (int i = 0; i < l; i++) {
    for (int j = 0; j < c; j++) {
      a.set(i, j, lambda * a.get(i, j));
    }
  }
  return a;
}

Matrix operator*(double lambda, const Matrix &a) { return a * lambda; }

bool operator==(const Matrix &a, const Matrix &b) {
  if (a.shape() != b.shape()) {
    return false;
  }
  for (int i = 0; i < a.shape().first; i++) {
    for (int j = 0; j < a.shape().second; j++) {
      // if (a())
    }
  }
  return true;
}

bool operator!=(const Matrix &a, const Matrix &b) {
  return !(a == b);
}

ostream& operator<< (ostream& os, const Matrix &m) {
  auto [l, c] = m.shape();
  for (int i = 0; i < l; i++) {
    for (int j = 0; j < c; j++) {
      os << m.get(i, j);
      if (j < c - 1) { os << " "; }
    }
    if (i < (l - 1)) { os << endl; }
  }
  return os;
}
