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
public:
  ContejeanDevieSolver(Matrix matrix) {
    int num_of_equations = matrix.size();
    int num_of_variables = matrix[0].size();
    std::vector<std::vector<int>> initial_solution = {};
    for (int i = 0; i < num_of_variables; ++i) {
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
	    }
	  }
	  temporary_solution.push_back(i);
	}
	initial_solution = {};
	for (auto i : temporary_solution) {
	  for (int j = 0; j < num_of_variables; ++j) {
	    std::vector<int> i_modified = i;
	    i_modified[j] += 1;
	    std::vector<int> one_vector = {};
	    for (int k = 0; k < j; ++k) {
	      one_vector.push_back(0);
	    }
	    one_vector.push_back(1);
	    for (int k = j + 1; k < num_of_variables; ++k) {
	      one_vector.push_back(0);
	    }	    
	    std::vector<int> equation_values_with_one_vector = {};
	    std::vector<int> equation_values_with_i_modified = {};
	    for (int k = 0; k < num_of_equations; ++k) {
	      int equation_value_with_i_modified_at_kth_row = 0;
	      for (int l = 0; l < num_of_variables; ++l) {
		equation_value_with_i_modified_at_kth_row += i_modified[l] * matrix[k][l];
	      }
	      equation_values_with_i_modified.push_back(equation_value_with_i_modified_at_kth_row);
	      equation_values_with_one_vector.push_back(one_vector[j] * matrix[k][j]);
	    }
	    int scalar_product = {};
	    for (int k = 0; k < num_of_equations; ++k) {
	      scalar_product += equation_values_with_one_vector[k] * equation_values_with_i_modified[k];
	    }
	    if (scalar_product < 0) {
	      initial_solution.push_back(i);
	    }
	  }
	}	
	int foo = 1;
	// f << 1;
      }
  }
};

int main() {
  std::vector<std::vector<int>> v = {std::vector<int>{-1, 1, 2, -3}, std::vector<int>{-1, 3, -2, -1}};
  Matrix m{v};
  ContejeanDevieSolver solver(m);
  return 0;
}
