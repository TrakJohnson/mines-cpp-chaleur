#pragma once
#include <functional>
#include "calc/matrix.h"
#include "calc/ode.h"

using namespace std;

class HeatSystem1D {
 public:
  HeatSystem1D(function<double(double)> dFunc,
	       function<double(double)> timeZeroTemp,
	       double time0, double timeN, double deltaTime,
	       double x0, double xN, double deltaX,
	       bool boundaryConditions = false,
	       double boundaryCondition0 = 0.,
	       double boundaryConditionN = 0.);
  // paramètres de résolution
  // publics car besoin dans utils/file.cpp
  double time0;
  double timeN;
  double deltaTime;
  double x0;
  double xN;
  double deltaX;
  // Chaque ligne i de la matrice résultante est l'état du système à t_i
  Matrix solve_explicit();
  Matrix solve_implicit();
 private:
  // paramètres système
  function<double(double)> dFunc;
  // paramètres résolution
  unsigned int nt;
  unsigned int nx;
  // conditions initiales
  function<double(double)> timeZeroTemp;
  // conditions limites
  bool boundaryConditions;
  double boundaryCondition0;
  double boundaryConditionN;
  // utils
  Matrix spaceDiscretize(function<double(double)> f);
  Matrix generateKMatrix();
};
