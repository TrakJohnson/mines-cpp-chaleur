#include "calc/ode.h"
#include "calc/matrix.h"
#include <functional>
#include <string>

ODESolver::ODESolver(double _time0, double _timeN, double _deltaT,
                     const Matrix &_initialX)
    : initialX(_initialX) { // Matrix n'a pas de constructeur par défaut on on
                            // doit mettre ça là ? TODO
  // Utiliser les listes d'initialisation lorsque:
  // - pas de constructeur défaut
  // - référence
  // - const

  this->time0 = _time0;
  this->timeN = _timeN;
  this->deltaT = _deltaT;
  this->n = (timeN - time0) / deltaT;
}

// TODO replace string with template ?
Matrix ODESolver::solve(function<Matrix(Matrix, double)> f) {
  int dim{
      this->initialX.shape().first}; // dimension de l'ev dans lequel on résout
  Matrix results(0., this->n,
                 dim); // chaque ligne est une photo à l'instant t_i
  Matrix currentX{this->initialX}; // conditions initiales à t = 0
  results.setLine(0, currentX);

  for (unsigned int i = 1; i < this->n; i++) {
    currentX = f(currentX, this->deltaT);
    results.setLine(i, currentX);
  }
  return results;
}

Matrix ODESolver::solve_euler_explicit(function<Matrix(Matrix)> df) {
  return solve([df](Matrix x, double dt) -> Matrix { return x + dt * df(x); });
}

Matrix ODESolver::solve_euler_implicit_linear(const Matrix &m) {
  return solve([m](Matrix x, double dt) -> Matrix {
    int _n{m.shape().first};
    // return solveSystem(Matrix::identity(_n) - dt * m, x, Matrix(0., _n, 1));
    return solveTridiagonalSystem(Matrix::identity(_n) - dt * m, x);
  });
}
