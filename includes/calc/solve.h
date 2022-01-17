#include <calc/matrix.h>

template<class Matrix>
Matrix solveSystemCG(const Matrix &_a, const Matrix &_b, const Matrix &x0,
                     double epsilon) {
  // TODO Les détails sur le choix de méthode de résolution du système
  // et petites manoeuvres effectuées ici sont dans le README
  Matrix a(0., 0, 0);
  Matrix b(0., 0, 0);
  // TODO FIXME the && false is temp !! TODO FIXME
  if (_a.transpose() != _a) {
    a = _a.transpose() * _a;
    b = _a.transpose() * _b;
  } else {
    a = _a;
    b = _b;
  }
  // Algorithme du gradient conjugué
  Matrix r{b - a * x0};
  Matrix p{r};
  Matrix x{x0};
  Matrix rNew{x};
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

template<class Matrix>
Matrix solveTridiagonalSystem(const Matrix &a, const Matrix &b) {
  int _n{a.shape().first};
  // Les diagonales de gauche à droite
  // cf. https://en.wikipedia.org/wiki/Tridiagonal_matrix_algorithm
  // Attention, on a adopté une numérotation différente à celle de wikipedia
  // vecA et vecB sont des vecteurs de taille _n - 1 et non pas _n

  // TODO: create a function constructor for vectors to avoid the
  // [[maybe_unused]]
  Matrix vecA(
      [a](int i, [[maybe_unused]] int j) -> double { return a(i + 1, i); },
      _n - 1, 1);
  Matrix vecB([a](int i, [[maybe_unused]] int j) -> double { return a(i, i); },
              _n, 1);
  Matrix vecC(
      [a](int i, [[maybe_unused]] int j) -> double { return a(i, i + 1); },
      _n - 1, 1);
  Matrix d{b};
  Matrix sol(0., _n, 1);
  double w{0.};
  // TODO: create set definition for vectors: set(int, double)
  // and avoid index issues
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

// DenseMatrix solveSystemCG(const DenseMatrix &_a, const DenseMatrix &_b,
//                           const DenseMatrix &x0, double epsilon);
// // SparseMatrix solveSystemCG(const SparseMatrix &_a, const SparseMatrix &_b, const SparseMatrix &x0,
// 			  // double epsilon);
// DenseMatrix solveTridiagonalSystem(const DenseMatrix &a, const DenseMatrix &b);
// // SparseMatrix solveTridiagonalSystem(const SparseMatrix &a, const SparseMatrix &b);
