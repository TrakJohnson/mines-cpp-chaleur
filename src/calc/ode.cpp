#include "calc/ode.h"
#include "calc/matrix.h"
#include "calc/solve.hpp"
#include <functional>
#include <string>

template <class Matrix>
ODESolver<Matrix>::ODESolver(double _time0, double _timeN, double _deltaT,
                             const DenseMatrix &_initialX)
    : initialX(_initialX) { // Matrix n'a pas de constructeur par défaut pq on
                            // doit mettre ça là ? TODO
  // NOTE pour moi: Utiliser les listes d'initialisation lorsque:
  // - pas de constructeur défaut
  // - référence
  // - const
  
  this->time0 = _time0;
  this->timeN = _timeN;
  this->deltaT = _deltaT;
  this->n = (timeN - time0) / deltaT;
}

template <class Matrix>
DenseMatrix ODESolver<Matrix>::solve(function<DenseMatrix(DenseMatrix, double)> f) {
  int dim{
      this->initialX.shape().first}; // dimension de l'ev dans lequel on résout
  DenseMatrix results(0., this->n,
                 dim); // chaque ligne est une photo à l'instant t_i
  DenseMatrix currentX{this->initialX}; // conditions initiales à t = 0
  results.setLine(0, currentX);

  for (unsigned int i = 1; i < this->n; i++) {
    currentX = f(currentX, this->deltaT);
    results.setLine(i, currentX);
  }
  return results;
}

template <class Matrix>
DenseMatrix ODESolver<Matrix>::solve_euler_explicit(function<DenseMatrix(DenseMatrix)> df) {
  return solve([df](DenseMatrix x, double dt) -> DenseMatrix { return x + dt * df(x); });
}

template <class Matrix>
DenseMatrix ODESolver<Matrix>::solve_euler_implicit_linear(const Matrix &m) {
  return solve([m](DenseMatrix x, double dt) -> DenseMatrix {
    int _n{m.shape().first};
    // On utilise le solver tridiagonal
    return solveTridiagonalSystem(m.identity(_n) - dt * m, x);
  });
}

template class ODESolver<DenseMatrix>;
template class ODESolver<SparseMatrix>;
