#pragma once
#include <calc/matrix.h>

// -- Méthodes de résolution de système
//
//   Ax = b    (1)
//
// Méthode du gradient conjugué (CG = conjugate gradient)
// @param a la matrice A de (1)
//          elle doit être, en théorie, symétrique définie positive
// @param b le vecteur b de (1)
// @param x0 le point de départ de l'algorithme, doit être aussi proche
//           que possible de la solution finale
inline DenseMatrix solveSystemCG(const DenseMatrix &_a, const DenseMatrix &_b,
                                 const DenseMatrix &x0, double epsilon = 0.01) {
  // Les détails sur le choix de méthode de résolution du système
  // et petites manoeuvres effectuées ici sont dans le README
  DenseMatrix a(0., 0, 0);
  DenseMatrix b(0., 0, 0);

  if (_a.transpose() != _a) {
    a = _a.transpose() * _a;
    b = _a.transpose() * _b;
  } else {
    a = _a;
    b = _b;
  }

  // Algorithme du gradient conjugué
  DenseMatrix r{b - a * x0};
  DenseMatrix p{r};
  DenseMatrix x{x0};
  DenseMatrix rNew{x};
  double alpha{0.};
  double beta{0.};
  while (r.norm() >= epsilon) {
    alpha = r.normSquared() / (p.transpose() * a * p)(0);
    x = x + alpha * p;
    rNew = r - alpha * a * p;
    beta = rNew.normSquared() / r.normSquared();
    p = rNew + beta * p;
    r = rNew;
  }
  return x;
}

// Algorithme de Thomas pour les systèmes tridiagonaux
// @param a la matrice A de (1)
//          elle doit être tridiagonale
// @param b le vecteur b de (1)
inline DenseMatrix solveTridiagonalSystem(const DenseMatrix &a,
                                          const DenseMatrix &b) {
  int _n{a.shape().first};
  // Les diagonales de gauche à droite
  // cf. https://en.wikipedia.org/wiki/Tridiagonal_matrix_algorithm
  // Attention, on a adopté une numérotation différente à celle de wikipedia
  // vecA et vecB sont des vecteurs de taille _n - 1 et non pas _n
  DenseMatrix vecA(
      [a](int i, [[maybe_unused]] int j) -> double { return a(i + 1, i); },
      _n - 1, 1);
  DenseMatrix vecB(
      [a](int i, [[maybe_unused]] int j) -> double { return a(i, i); }, _n, 1);
  DenseMatrix vecC(
      [a](int i, [[maybe_unused]] int j) -> double { return a(i, i + 1); },
      _n - 1, 1);
  DenseMatrix d{b};
  DenseMatrix sol(0., _n, 1);
  double w{0.};

  for (int i = 1; i < _n; i++) {
    w = vecA(i - 1) / vecB(i - 1);
    vecB.set(i, 0, vecB(i) - w * vecC(i - 1));
    d.set(i, 0, d(i) - w * d(i - 1));
  }
  sol.set(_n - 1, 0, d(_n - 1) / vecB(_n - 1));
  for (int i = _n - 2; i >= 0; i--) {
    sol.set(i, 0, (d(i) - vecC(i) * sol(i + 1)) / vecB(i));
  }
  return sol;
}

inline DenseMatrix solveTridiagonalSystem(const SparseMatrix &a,
                                          const DenseMatrix &b) {
  int _n{a.shape().first};
  // Les diagonales de gauche à droite
  // cf. https://en.wikipedia.org/wiki/Tridiagonal_matrix_algorithm
  // Attention, on a adopté une numérotation différente à celle de wikipedia
  // vecA et vecB sont des vecteurs de taille _n - 1 et non pas _n
  DenseMatrix vecA(
      [a](int i, [[maybe_unused]] int j) -> double { return a(i + 1, i); },
      _n - 1, 1);
  DenseMatrix vecB(
      [a](int i, [[maybe_unused]] int j) -> double { return a(i, i); }, _n, 1);
  DenseMatrix vecC(
      [a](int i, [[maybe_unused]] int j) -> double { return a(i, i + 1); },
      _n - 1, 1);
  DenseMatrix d{b};
  DenseMatrix sol(0., _n, 1);
  double w{0.};

  for (int i = 1; i < _n; i++) {
    w = vecA(i - 1) / vecB(i - 1);
    vecB.set(i, 0, vecB(i) - w * vecC(i - 1));
    d.set(i, 0, d(i) - w * d(i - 1));
  }
  sol.set(_n - 1, 0, d(_n - 1) / vecB(_n - 1));
  for (int i = _n - 2; i >= 0; i--) {
    sol.set(i, 0, (d(i) - vecC(i) * sol(i + 1)) / vecB(i));
  }
  return sol;
}
