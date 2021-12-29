#include "calc/matrix.h"
#include <iostream>
#include <stdexcept>
#include <tuple>
#include <vector>
#include <cmath>
#include <cassert>

using namespace std;

Matrix::Matrix(const Matrix &m) {
  this->content = m.content;
  this->nLines = m.nLines;
  this->nCols = m.nCols;
}

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

Matrix::Matrix(const vector<double> &vIn) {
  this->nLines = vIn.size();
  this->nCols = 1;

  vector<double> base(this->nLines * this->nCols, 0);
  this->content = base;
  for (int i = 0; i < nLines; i++) {
    this->set(i, 0, vIn.at(i));
  }
}

Matrix::Matrix(function<double(int i, int j)> f, const int &_nLines, const int &_nCols) {
  this->nLines = _nLines;
  this->nCols = _nCols;
  vector<double> v(_nLines * _nCols);
  this->content = v;
  // TODO find a way to use map
  for (int i = 0; i < _nLines; i++) {
    for (int j = 0; j < _nCols; j++) {
      this->set(i, j, f(i, j));
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
double Matrix::operator()(int i) const {
  if (this->nCols == 1) {
    return this->get(i, 0);
  } else if (this->nLines == 1) {
    return this->get(0, i);
  } else {
    throw runtime_error("Attempted to get value with vector notation on non-vector");
  }
}

void Matrix::set(int i, int j, double val) {
  if (i >= this->nLines || j >= this->nCols) {
    throw runtime_error("Invalid access: index (" + to_string(i) + "," +  to_string(j) + ") on shape (" + to_string(this->nLines) + "," + to_string(this->nCols) + ")");
  }
  this->content[i * this->nCols + j] = val;
}

Matrix Matrix::transpose() const {
  return Matrix([this](int i, int j) -> double { return this->get(j, i); }, this->nCols, this->nLines);
}

Matrix Matrix::extractLine(int i) {
  return Matrix([this, i]([[maybe_unused]] int dummy, int j) -> double { return this->get(i, j); }, 1, this->shape().second);
}

Matrix Matrix::extractLineAsCol(int i) {
  return this->extractLine(i).transpose();
}

Matrix Matrix::extractCol(int j) {
  return Matrix([this, j](int i, [[maybe_unused]] int d) -> double { return this->get(i, j); }, this->shape().first, 1);
}

void Matrix::setLine(int i, const vector<double> &v) {
  for (int j = 0; j < this->nCols; j++) {
    this->set(i, j, v.at(j));
  }
}

// TODO: pourquoi utiliser *this ??
double Matrix::normSquared() {
  if (this->nCols == 1) {
    // Ce calcul donne une matrice 1x1, d'où le "(0)"
    return (this->transpose() * *this)(0);
  } else if (this->nLines == 1) {
    return (*this * this->transpose())(0);
  } else {
    throw logic_error("Matrix::norm only implemented for vectors");
  }
}

double Matrix::norm() {
  return sqrt(this->normSquared());
}

// TODO: template this
void Matrix::setLine(int i, const Matrix &v) {
  for (int j = 0; j < this->nCols; j++) {
    this->set(i, j, v(0, j));
  }
}

Matrix operator+(Matrix a, const Matrix &b) {
  // On fait *une copie de a*
  auto [xa, ya] = a.shape();
  auto [xb, yb] = b.shape();
  if (xa != xb || ya != yb) {
    // TODO: create a custom exception for matrix incompatibiliy
    // cout << a << endl << b << endl;
    throw runtime_error("Incompatible matrix shapes (addition): (" + to_string(xa) + "," +  to_string(ya) + ") and (" + to_string(xb) + "," + to_string(yb) + ")");
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
  auto [xa, ya] = a.shape();
  auto [xb, yb] = b.shape();
  if (ya != xb) {
    string s = "Incompatible matrix shapes (multiplication): (" + to_string(xa) + "," +  to_string(ya) + ") and (" + to_string(xb) + "," + to_string(yb) + ")";
    throw runtime_error(s);
  }

  Matrix result(0, a.shape().first, b.shape().second);
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

Matrix operator*(Matrix a, double lambda) {
  auto [l, c] = a.shape();
  for (int i = 0; i < l; i++) {
    for (int j = 0; j < c; j++) {
      a.set(i, j, lambda * a(i, j));
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
      if (a(i, j) != b(i, j)) {
	return false;
      }
    }
  }
  return true;
}

bool operator!=(const Matrix &a, const Matrix &b) { return !(a == b); }

ostream &operator<<(ostream &os, const Matrix &m) {
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

Matrix solveSystemCG(const Matrix &_a, const Matrix &_b, const Matrix &x0, double epsilon) {
  // TODO Les détails sur le choix de méthode de résolution du système
  // et petites manoeuvres effectuées ici sont dans le README
  Matrix a(0., 0, 0);
  Matrix b(0., 0, 0);
  // TODO FIXME the && false is temp !! TODO FIXME
  if (_a.transpose() != _a) {
    a = _a.transpose() * _a;
    b = _a.transpose() * _b;
  } else {
    a = _a;
    b = _b;
  }
  // Algorithme du gradient conjugué
  Matrix r{b - a * x0};
  Matrix p{r};
  Matrix x{x0};
  Matrix rNew{x};
  double alpha{0.};
  double beta{0.};
  while (r.norm() >= epsilon) {
    alpha = r.normSquared() / (p.transpose() * a * p)(0);
    x = x + alpha * p;
    rNew = r - alpha * a * p;
    beta = rNew.normSquared() / r.normSquared();
    p = rNew + beta * p;
    r = rNew;
  }
  return x;
}

Matrix solveTridiagonalSystem(const Matrix &a, const Matrix &b) {
  int _n{a.shape().first};
  // Les diagonales de gauche à droite
  // cf. https://en.wikipedia.org/wiki/Tridiagonal_matrix_algorithm
  // Attention, on a adopté une numérotation différente à celle de wikipedia
  // vecA et vecB sont des vecteurs de taille _n - 1 et non pas _n

  // TODO: create a function constructor for vectors to avoid the [[maybe_unused]]
  Matrix vecA([a](int i, [[maybe_unused]] int j) -> double { return a(i + 1, i); }, _n - 1, 1);
  Matrix vecB([a](int i, [[maybe_unused]] int j) -> double { return a(i, i); }, _n, 1);
  Matrix vecC([a](int i, [[maybe_unused]] int j) -> double { return a(i, i + 1); }, _n - 1, 1);
  Matrix d{b};
  Matrix sol(0., _n, 1);
  double w{0.};
  // TODO: create set definition for vectors: set(int, double)
  // and avoid index issues
  for (int i = 1; i < _n; i++) {
    w = vecA(i - 1) / vecB(i - 1);
    vecB.set(i, 0, vecB(i) - w * vecC(i - 1));
    d.set(i, 0, d(i) - w * d(i - 1));
  }
  sol.set(_n - 1, 0, d(_n - 1) / vecB(_n - 1));
  for (int i = _n - 2; i >= 0; i--) {
    sol.set(i, 0, (d(i) - vecC(i) * sol(i + 1)) / vecB(i));
  }
  return sol;
}
