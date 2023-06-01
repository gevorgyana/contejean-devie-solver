#include <fstream>
#include <vector>
#include <algorithm>

class Matrix {
  std::vector<std::vector<int> > _matrix;
public:
  Matrix(std::vector<std::vector<int> >& ref_matrix) : _matrix(ref_matrix) {};
  std::vector<int> operator [] (int i) {
    return _matrix[i];
  }
  auto size() {
    return _matrix.size();
  }
};

class SystemOfLinearDiophanteEquationSolver {};

class ContejeanDevieSolver : SystemOfLinearDiophanteEquationSolver {
  std::ofstream f;
  ContejeanDevieSolver(Matrix matrix) {
    int num_of_equations = matrix.size();
    int num_of_variables = matrix[0].size();
    std::vector<std::vector<int>> initial_solution = {};
    for (int i = 0; i < num_of_equations; ++i) {
      std::vector<int> this_vector = {};
      for (int j = 0; j < i; ++j) {
	this_vector.emplace_back(0);
      }
      this_vector.emplace_back(1);
      for (int j = i + 1; j < num_of_variables; j++) {
	this_vector.emplace_back(0);
      }
      initial_solution.push_back(this_vector);
    }
    std::vector<std::vector<int>> final_solution = {};
    while (initial_solution.size() != 0) {
	for (int i = 0; i < num_of_equations; ++i) {
	  const std::vector<int>& current_equation = matrix[i];
	  for (auto& j : initial_solution) {
	    int value_at_this_equation = 0;
	    for (int k = 0; k < num_of_variables; ++k) {
	      value_at_this_equation += current_equation[k] * j[k];
	    }
	    if (value_at_this_equation == 0) {
	      initial_solution.push_back(current_equation);
	    }
	  }
	};
	std::vector<std::vector<int>> temporary_solution = {};
	for (auto& i : initial_solution) {
	  for (auto& j : final_solution) {
	    if (i == j) {
	      continue;
	    }
	    for (int k = 0; k < num_of_variables; ++k) {
	      for (int l = 0; l < num_of_variables; ++l) {
		if (i[k] <= j[k] and i[l] > j[l]) {
		  continue;
		}
	      }
	      temporary_solution.push_back(i);
	    }
	  }
	}
	for (int i = 0; i < num_of_variables; ++i) {
	  for (auto j : temporary_solution) {
	    std::vector<int> i_modified = j;
	    i_modified[i] += 1;
	    int scalar_product = 0;
	    for (int k = 0; k < num_of_variables; ++k) {
	      scalar_product += i_modified[k] * j[k];
	    }
	    if (scalar_product < 0) {
	      initial_solution.push_back(i_modified);
	    }
	  }
	}
	
	f << 1;
      }
  }
};

void solve_system_of_equations() {
  std::vector<std::vector<int>> v = {{-1, 1, 2, -3}, {-1, 3, -2, -1}};
  Matrix m{v};
}
