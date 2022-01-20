#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wold-style-cast"
#include "matplotlib-cpp/matplotlibcpp.h"
#pragma GCC diagnostic pop

#include "calc/matrix.h"
#include <string>

using namespace std;
namespace plt = matplotlibcpp;

template <typename Matrix>
void plotMatrix(double x0, double dx, double t0, double dt,
                const Matrix &results) {
  auto [timeN, n] = results.shape();
  vector<double> position(n);
  for (int i = 0; i < n; i++) {
    position[i] = x0 + i * dx;
    cout << endl;
  }

  vector<double> row(n);
  for (int i = 0; i < timeN; i++) {
    for (int j = 0; j < n; j++) {
      row[j] = results(i, j);
    }
    // plt::subplot2grid(nGraphs, 1, i, 0);
    plt::plot(position, row, "b-");

    // Clear previous plot
    plt::clf();
    plt::plot(position, row);
    plt::xlim(0, 1);
    plt::ylim(-1, 2);
    string fullDecimal = to_string(t0 + i * dt);
    plt::text(1, 1,
              "     t = " + fullDecimal.substr(0, fullDecimal.find(".") + 4));
    plt::xlabel("X");
    plt::ylabel("Température");
    plt::title("Evolution de la température en fonction du temps");
    plt::pause(0.02);
  }
  // Show plots
  // plt::show();
}

template void plotMatrix(double, double, double, double,
                         const DenseMatrix &results);
template void plotMatrix(double, double, double, double,
                         const SparseMatrix &results);
