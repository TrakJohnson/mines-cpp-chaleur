#pragma once

#include <tuple>
#include <vector>
#include <functional>
#include <iostream>

using namespace std;

class Matrix {

public:
  // Constructeur copie
  Matrix(const Matrix &m);
  // Rectangular mat
  Matrix(const double &x, const int &nLines, const int &nCols);
  // Square matrix
  Matrix(const double &x, const int &nLinesCols);
  // Matrix from 2D vector
  Matrix(const vector<vector<double>> &vIn);
  // This constructor takes f : (i, j) -> x
  // and gives M = (f(i, j))_{i,j}
  Matrix(function<double(int i, int j)> f, const int &nLines, const int &lCos);
  
  static Matrix identity(int n);
  
  pair<int, int> shape() const;
  double operator()(int i, int j) const;
  void set(int line, int col, double value);
  // TODO: OK THIS WORKS BUT WHY ?
  Matrix& operator=(const Matrix &m) = default;
  
  Matrix transpose();
  // TODO: these functions might induce a lot of copies?
  Matrix extractLine(int i);
  Matrix extractLineAsCol(int i);
  Matrix extractCol(int j);
  void setLine(int i, const vector<double> &v);
  void setLine(int i, const Matrix &v);
    
private:
  int nLines;
  int nCols;
  vector<double> content;
  double get(int line, int col) const;
  void display() const;
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
