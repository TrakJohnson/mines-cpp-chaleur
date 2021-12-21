#include <tuple>
#include <vector>
#include <iostream>

using namespace std;

class Matrix {

public:
  // Rectangular matrix
  Matrix(const double &x, const int &nLines, const int &nCols);
  // Square matrix
  Matrix(const double &x, const int &nLinesCols);
  // Matrix from 2D vector
  Matrix(const vector<vector<double>> &vIn);

  static Matrix identity(int n);
  
  pair<int, int> shape() const;
  double get(int line, int col) const;
  double operator()(int i, int j) const;
  void set(int line, int col, double value);
  void display() const;

private:
  int nLines;
  int nCols;
  vector<double> content;
};

Matrix operator+(Matrix a, const Matrix &b);
Matrix operator-(const Matrix &a, const Matrix &b);
// Multiplication matricielle
Matrix operator*(const Matrix &a, const Matrix &b);
// Multiplication par un scalaire
Matrix operator*(Matrix a, double lambda);
Matrix operator*(double lambda, const Matrix &a);
bool operator==(const Matrix &a, const Matrix &b);
bool operator!=(const Matrix &a, const Matrix &b);
// "pretty" output
ostream& operator<< (ostream& os, const Matrix &value);
