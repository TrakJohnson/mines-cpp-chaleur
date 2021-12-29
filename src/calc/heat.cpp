#include "calc/heat.h"
#include "calc/matrix.h"
#include "calc/ode.h"
#include <cmath>

HeatSystem1D::HeatSystem1D(function<double(double)> _dFunc,
                           function<double(double)> _timeZeroTemp,
                           double _time0, double _timeN, double _deltaTime,
                           double _x0, double _xN, double _deltaX,
                           bool _boundaryConditions, double _boundaryCondition0,
                           double _boundaryConditionN)
    : time0(_time0), timeN(_timeN), deltaTime(_deltaTime),
      x0(_x0), xN(_xN), deltaX(_deltaX),
      dFunc(_dFunc), timeZeroTemp(_timeZeroTemp),
      boundaryConditions(_boundaryConditions),
      boundaryCondition0(_boundaryCondition0),
      boundaryConditionN(_boundaryConditionN) {
  this->nt = int(timeN - time0) / deltaTime;
  this->nx = int(xN - x0) / deltaX;
}

// Ref:
// https://stackoverflow.com/questions/332030/when-should-static-cast-dynamic-cast-const-cast-and-reinterpret-cast-be-used
Matrix HeatSystem1D::spaceDiscretize(function<double(double)> f) {
  return Matrix(
      [f, this](int i, [[maybe_unused]] int j) -> double {
        return f(static_cast<double>(i) / this->nx);
      },
      this->nx, 1);
}

Matrix HeatSystem1D::generateKMatrix() {
  Matrix dVector = this->spaceDiscretize(this->dFunc);
  /*

  À vérifier, mais deux modifications que j'ai dû apporter à l'énoncé:
  - il faut diviser par deltax^2 la dérivée
  - il y a une erreur de signe ?
    avec définition de K de l'énoncé, on a plutôt

      T_{i+1} = T_i + (deltat/deltax^2) * K * T_i

  - on s'arrange pour que x_0(t_{i+1}) = x_0(t_{i}), x_N de même
    pour cela, on modifie la première et dernière ligne de K, par ex:

        |  1  0  0  0 |
    K = |  1 -2  1  0 |
        |  0  1 -2  1 |
        |  0  0  0  1 |

  */

  return Matrix(
      [dVector, this](unsigned int i, unsigned int j) -> double {
        // conditions aux limites
        if (this->boundaryConditions) {
          if (i == 0 || i == this->nx - 1) {
            return static_cast<double>(i == j);
          }
        }
        // tridiagonale
        if (j == i - 1) {
          return dVector(i);
        } else if (j == i) {
          return -dVector(i) - dVector(i + 1);
        } else if (j == i + 1) {
          return dVector(i + 1);
        } else {
          return 0.;
        }
      },
      this->nx, this->nx);
}

Matrix HeatSystem1D::solve_explicit() {
  // Conditions initiales
  Matrix initial{this->spaceDiscretize(timeZeroTemp)};
  if (this->boundaryConditions) {
    initial.set(0, 0, this->boundaryCondition0);
    initial.set(this->nx - 1, 0, this->boundaryConditionN);
  }

  ODESolver solver(this->time0, this->timeN, this->deltaTime, initial);
  Matrix k{this->generateKMatrix()};

  // d/dt T = 1/deltaX^2 * K * T
  // TODO: make everything linear (don't take df as argument but the matrix
  // instead)
  auto df = [k, this](Matrix temp) -> Matrix {
    return 1 / (pow(this->deltaX, 2)) * k * temp;
  };
  return solver.solve_euler_explicit(df);
}

Matrix HeatSystem1D::solve_implicit() {
  // TODO avoid code duplication from HeatSystem1D::solve_implicit
  // Conditions initiales
  Matrix initial{this->spaceDiscretize(timeZeroTemp)};
  if (this->boundaryConditions) {
    initial.set(0, 0, this->boundaryCondition0);
    initial.set(this->nx - 1, 0, this->boundaryConditionN);
  }

  ODESolver solver(this->time0, this->timeN, this->deltaTime, initial);
  Matrix k{this->generateKMatrix()};
  // cout << k << endl;
  // d/dt T = 1/deltaX^2 * K * T = m * T
  // TODO: make everything linear (don't take df as argument but the matrix
  // instead)
  return solver.solve_euler_implicit_linear(1 / (pow(this->deltaX, 2)) * k);
}
