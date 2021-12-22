#pragma once
#include <functional>
#include "calc/matrix.h"

// TODO use ref for function or not jsplus ?
// TODO where do I put the docs ? header files or implementation ?

class ODESolver {
 public:
  ODESolver(double time0, double timeN, double deltaT, const Matrix &initialX);
  // Euler explicite: x_i+1 = x_i + dt * df(x_i)
  Matrix solve_euler_explicit(function<Matrix(Matrix)> df);
  // Euler implicite: x_i+1 = x_i + dt * df(x_i+1)
  Matrix solve_euler_implicit(function<Matrix(Matrix)> df);
 private:
  // paramètres temps
  double time0;
  double timeN;
  double deltaT;
  unsigned int n;
  // conditions initiales
  const Matrix initialX;
  // generic solving function
  // x_i+1 = F(x_i, deltaT)
  Matrix solve(function<Matrix(Matrix, double)> F);
};
