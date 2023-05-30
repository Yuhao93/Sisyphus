#include "parametric_processor.h"

#include "model.pb.h"
#include <cmath>

namespace {

using sisyphus::ParametricPattern;
using sisyphus::ParametricPattern_Equation;
using sisyphus::ParametricPattern_Equation_UnaryEquation;
using sisyphus::ParametricPattern_Equation_BinaryEquation;

double EvaluateEquation(int t, const ParametricPattern_Equation& equation);
double EvaluateBinaryEquation(int t, const ParametricPattern_Equation_BinaryEquation& binary_equation);
double EvaluateUnaryEquation(int t, const ParametricPattern_Equation_UnaryEquation& unary_equation);

double EvaluateEquation(int t, const ParametricPattern_Equation& equation) {
  switch (equation.equation_type_case()) {
    case ParametricPattern_Equation::kBinaryEquation:
      return EvaluateBinaryEquation(t, equation.binary_equation());
    case ParametricPattern_Equation::kUnaryEquation:
      return EvaluateUnaryEquation(t, equation.unary_equation());
    case ParametricPattern_Equation::kConstant:
      return equation.constant();
    case ParametricPattern_Equation::kVariable:
      return t;
  }
}

double EvaluateBinaryEquation(int t, const ParametricPattern_Equation_BinaryEquation& binary_equation) {
  double left = EvaluateEquation(t, binary_equation.equation_left());
  double right = EvaluateEquation(t, binary_equation.equation_right());
  switch (binary_equation.operation()) {
    case ParametricPattern_Equation_BinaryEquation::ADD:
      return left + right;
    case ParametricPattern_Equation_BinaryEquation::SUBTRACT:
      return left - right;
    case ParametricPattern_Equation_BinaryEquation::MULTIPLY:
      return left * right;
    case ParametricPattern_Equation_BinaryEquation::DIVIDE:
      return left / right;
    case ParametricPattern_Equation_BinaryEquation::POWER:
      return pow(left, right);
    case ParametricPattern_Equation_BinaryEquation::ROOT:
      if (left == 0 || (right == 0 && left <= 0) || (right < 0 && left != std::abs(left))) {
        return 0;
      }
      return pow(right, 1 / left);
    case ParametricPattern_Equation_BinaryEquation::LOG:
      if (left <= 0 || right <= 0 || left == 1) {
        return 0;
      }
      return log(right) / log(left);
    case ParametricPattern_Equation_BinaryEquation::ATAN2:
      return atan2(left, right);
  }
}

double EvaluateUnaryEquation(int t, const ParametricPattern_Equation_UnaryEquation& unary_equation) {
  double sub_value = EvaluateEquation(t, unary_equation.equation());
  switch (unary_equation.operation()) {
    case ParametricPattern_Equation_UnaryEquation::SIN:
      return sin(sub_value);
    case ParametricPattern_Equation_UnaryEquation::COS:
      return cos(sub_value);
    case ParametricPattern_Equation_UnaryEquation::TAN:
      return tan(sub_value);
    case ParametricPattern_Equation_UnaryEquation::ASIN:
      return asin(sub_value);
    case ParametricPattern_Equation_UnaryEquation::ACOS:
      return acos(sub_value);
    case ParametricPattern_Equation_UnaryEquation::ATAN:
      return atan(sub_value);
    case ParametricPattern_Equation_UnaryEquation::NEGATIVE:
      return -sub_value;
    case ParametricPattern_Equation_UnaryEquation::STEP:
      return sub_value >= 0 ? 1 : 0;
    case ParametricPattern_Equation_UnaryEquation::SIGN:
      return sub_value == 0 ? 0 : (sub_value > 0 ? 1 : -1);
    case ParametricPattern_Equation_UnaryEquation::ABS:
      return std::abs(sub_value);
    case ParametricPattern_Equation_UnaryEquation::FLOOR:
      return floor(sub_value);
    case ParametricPattern_Equation_UnaryEquation::CEIL:
      return ceil(sub_value);
    case ParametricPattern_Equation_UnaryEquation::ROUND:
      return round(sub_value);
  }
}

}

std::vector<sisyphus::CartesianCoordinate>
ParametricProcessor::GetCoordinatesFromParametricFunction(
    const sisyphus::ParametricPattern& pattern) {
  std::vector<sisyphus::CartesianCoordinate> result;
  for (int t = pattern.start_t(); t <= pattern.end_t(); t += pattern.t_step()) {
    double x_or_theta = EvaluateEquation(t, pattern.equation_for_x_or_theta());
    double y_or_r = EvaluateEquation(t, pattern.equation_for_y_or_r());
    sisyphus::CartesianCoordinate coordinate;
    switch (pattern.type()) {
      case sisyphus::ParametricPattern::CARTESIAN:
        coordinate.set_x(x_or_theta);
        coordinate.set_y(y_or_r);
        break;
      case sisyphus::ParametricPattern::POLAR:
        coordinate.set_x(y_or_r * cos(x_or_theta));
        coordinate.set_y(y_or_r * sin(x_or_theta));
        break;
    }
    result.push_back(coordinate);
  }
  return result;
}
