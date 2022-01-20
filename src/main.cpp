#include <iostream>
#include <cmath>
#include <functional>
#include <chrono>
#include <ostream>
#include "calc/matrix.h"
#include "calc/heat.h"
#include "utils/file.h"
#include "utils/graph.h"

using namespace std;

// Paramètres de l'énoncé
double t0{0.};
double tN{.5};
double x0{0.};
double xN{1.};

// Permet de répondre aux questions
// @param dFunc est la fonction x -> D(x)
// @param filename le nom du fichier où les résultats sont stockés
//                 s'il est vide, on n'enregistre pas les résultats
// @param dt delta temps (résolution)
// @param dx delta position (discrétisation du problème)
// @param useImplicit choix de la méthode d'Euler (vrai pour méthode implicite)
template<typename Matrix>
void question(function<double(double)> dFunc, string filename, double dt, double dx, bool useImplicit, bool cppPlot = false) {
  HeatSystem1D<Matrix> sys(dFunc,
		   [](double x) -> double {
		     return 0.5 + sin(2 * M_PI * x) - 0.5 * cos(2 * M_PI * x);
		   },
		   // le choix de dt et dx est motivé dans le README
		   t0, tN, dt,  
		   x0, xN, dx,
		   true, 0., 0.);  // conditions au bord
  DenseMatrix r(0., 0);
  if (!useImplicit) {
    r = sys.solve_explicit();
  } else {
    r = sys.solve_implicit();
  }
  if (filename.length() > 0 && !(cppPlot)) {
    writeMatrixToFile(r, sys, filename);
  }
  if (cppPlot) {
    plotMatrix(x0, dx, t0, dt, r);
  }
}

// Chronomètre la réponse à une question
// @return le temps (en ms) mis pour la résolution, sur une moyenne de 10
// exécutions
template<typename Matrix>
double timeQuestion(function<double(double)> dFunc, double dt, double dx, bool useImplicit) {
  auto start = chrono::steady_clock::now();
  // On répète 10 fois pour avoir un temps moyen
  for (int i = 0; i < 10; i++) {
    question<Matrix>(dFunc, "perf/" + to_string(time(NULL)) + ".txt", dt, dx, useImplicit);
  }
  auto end = chrono::steady_clock::now();
  return (chrono::duration_cast<std::chrono::milliseconds>(end - start)).count() / 10.;
}

int main() {
  srand(time(NULL));
  
  auto dFuncConst = []([[maybe_unused]] double x) -> double { return 1.; };
  // Question 2
  question<DenseMatrix>(dFuncConst, "euler_explicite.txt", 0.001, 0.05, false);
  // Question 4
  question<DenseMatrix>(dFuncConst, "euler_implicite.txt", 0.001, 0.05, true);

  // Question bonus 1
  auto dFuncRand = []([[maybe_unused]] double x) -> double { return 0.01 * (rand() % 100 + 50); };
  question<DenseMatrix>(dFuncRand, "euler_explicite_bonus.txt", 0.001, 0.05, false);
  question<DenseMatrix>(dFuncRand, "euler_implicite_bonus.txt", 0.001, 0.05, true);  

  // Question bonus 2
  vector<double> params = {0.1, 0.05};  
  double dt;
  for (auto dx: params) {
    dt = (dx*dx)/2;
    cout << "--- Performance avec dx="  << dx << " et dt="<< dt << endl;
    cout << "Euler explicite: " << timeQuestion<DenseMatrix>(dFuncConst, dt, dx, false) << " ms\n";
    cout << "Euler explicite creux: " << timeQuestion<SparseMatrix>(dFuncConst, dt, dx, false) << " ms\n";
    cout << "Euler implicite: " << timeQuestion<DenseMatrix>(dFuncConst, dt, dx, true) << " ms\n";
    cout << "Euler implicite creux: " << timeQuestion<SparseMatrix>(dFuncConst, dt, dx, true) << " ms\n";
  }
  
  // Question bonus 3
  question<DenseMatrix>(dFuncConst, "euler_explicite.txt", 0.005, 0.1, false, true);
  
  return EXIT_SUCCESS;
}
