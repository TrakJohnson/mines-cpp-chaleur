#pragma once

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wold-style-cast"
#include "eigen-3.4.0/Eigen/Core"
#include "eigen-3.4.0/Eigen/SparseCore"
#pragma GCC diagnostic pop

#include <functional>
#include <iostream>
#include <tuple>
#include <variant>
#include <vector>

using namespace std;

class DenseMatrix {

public:
  // Constructeur copie
  DenseMatrix(const DenseMatrix &m);
  // Constructeur de matrice rectangulaire
  // @param x le contenu des cellules par défaut
  // @param nLines nombre de lignes
  // @param nCols nombre de colonnes
  DenseMatrix(const double &x, const int &nLines, const int &nCols);
  // Constructeur de matrice carrée
  // @param x le contenu des cellules par défaut
  // @param nLinesCols le nombre de lignes et de colonnes
  DenseMatrix(const double &x, const int &nLinesCols);
  // Constructeur de matrice à partir d'un vector<vector<...>>
  // @param vIn le vecteur de lignes à partir duquel la matrice est construite
  //            tous les éléments de vIn doivent avoir la même longueur
  DenseMatrix(const vector<vector<double>> &vIn);
  // Constructeur de vecteur colonne (matrix n * 1) à partir d'un vector<...>
  // @param vIn la colonne de la matrice
  DenseMatrix(const vector<double> &vIn);
  // Constructeur "map"
  // @param f fonction f : (i, j) -> x, la matrice construite sera alors M =
  // (f(i, j))_{i,j}
  // @param nLines nombre de lignes
  // @param nCols nombre de colonnes
  // Attention à vérifier que f est au moins définie sur le produit cartésien
  // d'intervalles entiers
  // [| 0; nLines - 1 |] * [| 0; nCols - 1 |]
  DenseMatrix(function<double(int i, int j)> f, const int &nLines,
              const int &nCols);

  // Renvoie la matrice identité de taile n
  static DenseMatrix identity(int n);

  // Renvoie la paire (nLines, nCols)
  pair<int, int> shape() const;
  // a(i, j) est équivalent à a.get(i, j)
  double operator()(int i, int j) const;
  // a(i) est équivalent à:
  // - a.get(i, 0) si a est un vecteur colonne
  // - a.get(0, i) si a est un vecteur ligne
  // - renvoie une runtime_error sinon
  double operator()(int i) const;
  // Modification d'une cellule
  void set(int i, int j, double value);
  DenseMatrix &operator=(const DenseMatrix &m) = default;

  // Renvoie la matrice transposée
  DenseMatrix transpose() const;
  
  void setLine(int i, const vector<double> &v);
  void setLine(int i, const DenseMatrix &v);

  // Renvoie la norme au carré d'un vecteur (ligne ou colonne)
  // Si l'objet n'est pas un vecteur, lève une logic_error
  double normSquared();
  double norm();

private:
  int nLines;
  int nCols;
  vector<double> content;
  double get(int line, int col) const {
    return this->content.at(this->nCols * line + col);
  }
  void display() const;
};
    
DenseMatrix operator+(DenseMatrix a, const DenseMatrix &b);
DenseMatrix operator-(const DenseMatrix &a, const DenseMatrix &b);
// Multiplication matricielle
DenseMatrix operator*(const DenseMatrix &a, const DenseMatrix &b);
// Multiplication par un scalaire
DenseMatrix operator*(DenseMatrix a, double lambda);
DenseMatrix operator*(double lambda, const DenseMatrix &a);
bool operator==(const DenseMatrix &a, const DenseMatrix &b);
bool operator!=(const DenseMatrix &a, const DenseMatrix &b);
// Affichage de la matrice
ostream &operator<<(ostream &os, const DenseMatrix &value);

// -- Sparse matrix
typedef Eigen::SparseMatrix<double> EigenSparse;

class SparseMatrix {

public:
  SparseMatrix(const SparseMatrix &m);
  SparseMatrix(const double &x, const int &nLines, const int &nCols);
  SparseMatrix(const double &x, const int &nLinesCols);
  SparseMatrix(const vector<vector<double>> &vIn);
  SparseMatrix(const vector<double> &vIn);
  SparseMatrix(function<double(int i, int j)> f, const int &nLines,
               const int &nCols);
  static SparseMatrix identity(int n);
  pair<int, int> shape() const;
  double operator()(int i, int j) const;
  double operator()(int i) const;
  void set(int i, int j, double value);
  SparseMatrix &operator=(const SparseMatrix &m) = default;
  friend DenseMatrix operator*(SparseMatrix m, const DenseMatrix &v);
  
  SparseMatrix transpose() const;

  // nécessaire ???
  // SparseMatrix extractLine(int i);
  // SparseMatrix extractLineAsCol(int i);
  // SparseMatrix extractCol(int j);
  void setLine(int i, const vector<double> &v);
  void setLine(int i, const SparseMatrix &v);

  double normSquared();
  double norm();

private:
  EigenSparse content;
  void display() const;
  double get(int i, int j) const;
};

SparseMatrix operator+(SparseMatrix a, const SparseMatrix &b);
SparseMatrix operator-(const SparseMatrix &a, const SparseMatrix &b);
// Multiplication matricielle
SparseMatrix operator*(const SparseMatrix &a, const SparseMatrix &b);
// Multiplication par un scalaire
SparseMatrix operator*(SparseMatrix a, double lambda);
SparseMatrix operator*(double lambda, const SparseMatrix &a);

bool operator==(const SparseMatrix &a, const SparseMatrix &b);
bool operator!=(const SparseMatrix &a, const SparseMatrix &b);
// Affichage de la matrice
ostream &operator<<(ostream &os, const SparseMatrix &value);
