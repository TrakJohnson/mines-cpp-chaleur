#pragma once
#include <functional>
#include "calc/matrix.h"

template <class Matrix>
class ODESolver {
 public:
  ODESolver(double time0, double timeN, double deltaT, const DenseMatrix &initialX);
  // Euler explicite: x_i+1 = x_i + dt * df(x_i)
  DenseMatrix solve_euler_explicit(function<DenseMatrix(DenseMatrix)> df);
  // Euler implicite: x_i+1 = x_i + dt * df(x_i+1)
  // Dans notre cas, x -> df(x) est linéaire en x
  // donc on passe en argument la matrice représentant df
  // On se laisse la possibilité d'utiliser SparseMatrix ici, pour K
  DenseMatrix solve_euler_implicit_linear(const Matrix &m);
 private:
  // paramètres temps
  double time0;
  double timeN;
  double deltaT;
  unsigned int n;
  // conditions initiales
  const DenseMatrix initialX;
  // generic solving function
  // x_i+1 = F(x_i, deltaT)
  DenseMatrix solve(function<DenseMatrix(DenseMatrix, double)> F);
};
