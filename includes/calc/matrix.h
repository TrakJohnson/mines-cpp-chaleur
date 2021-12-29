#pragma once

#include <functional>
#include <iostream>
#include <tuple>
#include <vector>

using namespace std;

class Matrix {

public:
  // Constructeur copie
  Matrix(const Matrix &m);
  // Constructeur de matrice rectangulaire
  // @param x le contenu des cellules par défaut
  // @param nLines nombre de lignes
  // @param nCols nombre de colonnes
  Matrix(const double &x, const int &nLines, const int &nCols);
  // Constructeur de matrice carrée
  // @param x le contenu des cellules par défaut
  // @param nLinesCols le nombre de lignes et de colonnes
  Matrix(const double &x, const int &nLinesCols);
  // Constructeur de matrice à partir d'un vector<vector<...>>
  // @param vIn le vecteur de lignes à partir duquel la matrice est construite
  //            tous les éléments de vIn doivent avoir la même longueur
  Matrix(const vector<vector<double>> &vIn);
  // Constructeur de vecteur colonne (matrix n * 1) à partir d'un vector<...>
  // @param vIn la colonne de la matrice
  Matrix(const vector<double> &vIn);
  // Constructeur "map"
  // @param f fonction f : (i, j) -> x, la matrice construite sera alors M = (f(i, j))_{i,j}
  // @param nLines nombre de lignes
  // @param nCols nombre de colonnes
  // Attention à vérifier que f est au moins définie sur le produit cartésien d'intervalles entiers
  // [| 0; nLines - 1 |] * [| 0; nCols - 1 |]
  Matrix(function<double(int i, int j)> f, const int &nLines, const int &nCols);

  // Renvoie la matrice identité de taile n
  static Matrix identity(int n);

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
  // TODO: OK THIS WORKS BUT WHY ?
  Matrix &operator=(const Matrix &m) = default;
  
  // Renvoie la matrice transposée
  Matrix transpose() const;
  // Ces fonctions vont probablement à la poubelle
  // TODO: these functions might induce a lot of copies?
  Matrix extractLine(int i);
  Matrix extractLineAsCol(int i);
  Matrix extractCol(int j);
  void setLine(int i, const vector<double> &v);
  void setLine(int i, const Matrix &v);

  // Renvoie la norme au carré d'un vecteur (ligne ou colonne)
  // Si l'objet n'est pas un vecteur, lève une logic_error
  // TODO: quel type d'exceptions utiliser ?
  double normSquared();
  double norm();

private:
  int nLines;
  int nCols;
  vector<double> content;
  double get(int line, int col) const;
  void display() const;
};

Matrix operator+(Matrix a, const Matrix &b);
Matrix operator-(const Matrix &a, const Matrix &b);
// Multiplication matricielle
Matrix operator*(const Matrix &a, const Matrix &b);
// Multiplication par un scalaire
Matrix operator*(Matrix a, double lambda);
Matrix operator*(double lambda, const Matrix &a);

bool operator==(const Matrix &a, const Matrix &b);
bool operator!=(const Matrix &a, const Matrix &b);
// Affichage de la matrice
ostream &operator<<(ostream &os, const Matrix &value);

// -- Méthodes de résolution de système
//   Ax = b    (1)
// Méthode du gradient conjugué
// @param a la matrice A de (1)
//          elle doit être, en théorie, symétrique définie positive
// @param b le vecteur b de (1)
// @param x0 le point de départ de l'algorithme, doit être aussi proche
//           que possible de la solution finale
Matrix solveSystemCG(const Matrix &a, const Matrix &b, const Matrix &x0,
                     double epsilon = 0.001);
// Algorithme de Thomas pour les systèmes tridiagonaux
// @param a la matrice A de (1)
//          elle doit être tridiagonale
// @param b le vecteur b de (1)
Matrix solveTridiagonalSystem(const Matrix &a, const Matrix &b);
