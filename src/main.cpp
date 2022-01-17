#include <iostream>
#include <cmath>
#include <functional>
#include <chrono>
#include <ostream>
#include "calc/matrix.h"
#include "calc/heat.h"
#include "utils/file.h"

using namespace std;

// Paramètres de l'énoncé
double t0{0.};
double tN{.5};
double x0{0.};
double xN{1.};

// Choix entre matrices creuses et denses
using MatrixType = DenseMatrix;

// Permet de répondre aux questions
// @param dFunc est la fonction x -> D(x)
// @param filename le nom du fichier où les résultats sont stockés
//                 s'il est vide, on n'enregistre pas les résultats
// @param dt delta temps (résolution)
// @param dx delta position (discrétisation du problème)
// @param useImplicit choix de la méthode d'Euler (vrai pour méthode implicite)
void question(function<double(double)> dFunc, string filename, double dt, double dx, bool useImplicit) {
  HeatSystem1D<MatrixType> sys(dFunc,
		   [](double x) -> double {
		     return 0.5 + sin(2 * M_PI * x) - 0.5 * cos(2 * M_PI * x);
		   },
		   // le choix de dt et dx est motivé dans le README
		   t0, tN, dt,  
		   x0, xN, dx,
		   true, 0., 0.);  // conditions au bord
  MatrixType r(0., 0);
  if (!useImplicit) {
    r = sys.solve_explicit();
  } else {
    r = sys.solve_implicit();
  }
  if (filename.length() > 0) {
    writeMatrixToFile(r, sys, filename);
  }
}

// Chronomètre la réponse à une question
// @return le temps (en ms) mis pour la résolution, sur une moyenne de 10 exécutions
double timeQuestion(function<double(double)> dFunc, double dt, double dx, bool useImplicit) {
  auto start = chrono::steady_clock::now();
  // On répète 10 fois pour avoir un temps moyen
  for (int i = 0; i < 10; i++) {
    question(dFunc, "perf/" + to_string(time(NULL)) + ".txt", dt, dx, useImplicit);
  }
  auto end = chrono::steady_clock::now();
  return (chrono::duration_cast<std::chrono::milliseconds>(end - start)).count() / 10.;
}

int main() {
  srand(time(NULL));
  
  auto dFuncConst = []([[maybe_unused]] double x) -> double { return 1.; };
  // Question 2
  question(dFuncConst, "euler_explicite.txt", 0.001, 0.05, false);
  // Question 4
  question(dFuncConst, "euler_implicite.txt", 0.001, 0.05, true);

  // Question bonus 1
  auto dFuncRand = []([[maybe_unused]] double x) -> double { return 0.01 * (rand() % 100 + 50); };
  question(dFuncRand, "euler_explicite_bonus.txt", 0.001, 0.05, false);
  question(dFuncRand, "euler_implicite_bonus.txt", 0.001, 0.05, true);  

  // Question bonus 2
  vector<double> params = {0.1, 0.05, 0.04, 0.03, 0.02, 0.01};  
  double dt;
  for (auto dx: params) {
    dt = (dx*dx)/2;
    cout << "--- Performance avec dx="  << dx << " et dt="<< dt << endl;
    cout << "Euler explicite: " << timeQuestion(dFuncConst, dt, dx, false) << " ms\n";
    cout << "Euler implicite: " << timeQuestion(dFuncConst, dt, dx, true) << " ms\n";
  }
  /*
    --- Performance avec dx=0.1 et dt=0.005
    Euler explicite: 0.9 ms
    Euler implicite: 1.7 ms
    --- Performance avec dx=0.05 et dt=0.00125
    Euler explicite: 12.1 ms
    Euler implicite: 19.5 ms
    --- Performance avec dx=0.04 et dt=0.0008
    Euler explicite: 28.3 ms
    Euler implicite: 44.7 ms
    --- Performance avec dx=0.03 et dt=0.00045
    Euler explicite: 83.7 ms
    Euler implicite: 130 ms
    --- Performance avec dx=0.02 et dt=0.0002
    Euler explicite: 417.6 ms
    Euler implicite: 642.3 ms
    --- Performance avec dx=0.01 et dt=5e-05
    Euler explicite: 6462.5 ms
    Euler implicite: 9733.9 ms
  */
  return EXIT_SUCCESS;
}
