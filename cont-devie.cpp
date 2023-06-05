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
  
  std::vector<int> EvaluateAllEquationsAtVector(std::vector<int> x) {
    std::vector<int> evaluated_values = {};
    for (int i = 0; i < _matrix.size(); ++i) {
      int evaluated_value = 0;
      for (int j = 0; j < _matrix[0].size(); ++j) {
	evaluated_value += _matrix[i][j] * x[j];
      }
      evaluated_values.push_back(evaluated_value);
    }
    return evaluated_values;
  }

  int EvaluateSelectedEquationAtVector(std::vector<int> x, int i) {
    int evaluated_equation = 0;
    for (int j = 0; j < _matrix[i].size(); ++j) {
      evaluated_equation += _matrix[i][j];
    }
    return evaluated_equation;
  }
  
};

class SystemOfLinearDiophanteEquationSolver {
  
public:
  
  virtual void Solve() = 0;
  
  virtual void OutputToFile(std::string file_name) = 0;
  
};

std::vector<int> AddTwoVectors(std::vector<int> a, std::vector<int> b) {
  std::vector<int> sum;
  for (int i = 0; i < a.size(); ++i) {
    sum.push_back(a[i] + b[i]);
  }
  return sum;
}

class ContejeanDevieSolver : public SystemOfLinearDiophanteEquationSolver {
  
private:
  
  std::ofstream output_file;
  Matrix _matrix;
  
public:

  void Solve() override {
    auto initial_solution = GenerateBasisVectorsInSpace();
    std::vector<std::vector<int>> final_solution = {};
    int num_of_variables = _matrix[0].size();
    int num_of_equations = _matrix.size();
    while (initial_solution.size() != 0) {
      for (auto& i : initial_solution) {
	bool null = true;
	for (int j = 0; j < num_of_equations; ++j) {
	  int value_at_this_equation = _matrix.EvaluateSelectedEquationAtVector(i, j);
	  if (value_at_this_equation != 0) {
	    null = false;
	    break;
	  }
	}
	if (null) {
	  final_solution.push_back(i);
	}
      }
      std::vector<std::vector<int>> temporary_solution;
      for (auto& i : initial_solution) {
	bool i_is_duplicate_from_j = false;
	for (auto& j : final_solution) {
	  if (i == j) {
	    i_is_duplicate_from_j = true;
	  }
	}
	if (i_is_duplicate_from_j == true) {
	  continue;
	}
	bool comparable_in_terms_of_partial_order = false;
	for (auto& j : final_solution) {
	  for (int k = 0; k < num_of_variables; ++k) {
	    for (int l = 0; l < num_of_variables; ++l) {
	      if (i[k] <= j[k] and i[l] > j[l]) {
		comparable_in_terms_of_partial_order = true;
	      }
	    }
	  }
	}
	if (!comparable_in_terms_of_partial_order) {
	  temporary_solution.push_back(i);
	}
      }
      initial_solution = {};
      for (auto i : temporary_solution) {
	for (int j = 0; j < num_of_variables; ++j) {
	  std::vector<int> one_vector = GenerateOneVector(j);
	  std::vector<int> equation_values_with_one_vector = _matrix.EvaluateAllEquationsAtVector(one_vector);
	  std::vector<int> equation_values_with_i = _matrix.EvaluateAllEquationsAtVector(i);
	  int scalar_product = EvaluateScalarProduct(equation_values_with_one_vector, equation_values_with_i);
	  if (scalar_product < 0) {
	    std::vector<int> vector_under_test = AddTwoVectors(i, one_vector);
	    initial_solution.push_back(vector_under_test);
	  }
	}
      }
    }
  }

  void OutputToFile(std::string file_name) override {
    ;
  }
  
  std::vector<std::vector<int>> GenerateBasisVectorsInSpace() {
    int num_of_equations = _matrix.size();
    int num_of_variables = _matrix[0].size();
    std::vector<std::vector<int>> basis_vectors = {};
    for (int i = 0; i < num_of_variables; ++i) {
      std::vector<int> this_vector = {};
      for (int j = 0; j < i; ++j) {
	this_vector.emplace_back(0);
      }
      this_vector.emplace_back(1);
      for (int j = i + 1; j < num_of_variables; j++) {
	this_vector.emplace_back(0);
      }
      basis_vectors.push_back(this_vector);
    }
    return basis_vectors;
  }

  std::vector<int> GenerateOneVector(int j) {
    std::vector<int> one_vector = {};
    for (int k = 0; k < j; ++k) {
      one_vector.push_back(0);
    }
    one_vector.push_back(1);
    for (int k = j + 1; k < _matrix[0].size(); ++k) {
      one_vector.push_back(0);
    }	    
    return one_vector;
  }

  int EvaluateScalarProduct(std::vector<int> i, std::vector<int> j) {
    int scalar_product = 0;
    for (int k = 0; k < i.size(); ++k) {
      scalar_product += i[k] * j[k];
    }
    return scalar_product;
  }
  
  ContejeanDevieSolver(Matrix matrix) : _matrix(matrix) {}
  
};

int main() {
  std::vector<std::vector<int>> v = {std::vector<int>{-1, 1, 2, -3}, std::vector<int>{-1, 3, -2, -1}};
  Matrix m{v};
  
  ContejeanDevieSolver solver(m);
  solver.Solve();
  solver.OutputToFile("test.out");
  return 0;
}
