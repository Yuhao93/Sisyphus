package haodev.com.sisyphus;

import android.util.Pair;

import java.util.ArrayList;
import java.util.List;

import sisyphus.Model;

/**
 * Created by yuhao on 9/3/2018.
 */

public abstract class ParametricPatternInterpolator implements PatternInterpolator {
  private static final int MILLIS_PER_STEP = 16;
  abstract boolean accepts(Model.ParametricPattern pattern);
  abstract List<Model.CartesianCoordinate> interpolate(List<Pair<Double, Double>> pattern);

  @Override
  public boolean accepts(Model.StoredPattern pattern) {
    return pattern.hasParametric() && accepts(pattern.getParametric());
  }

  @Override
  public List<Model.CartesianCoordinate> interpolate(Model.StoredPattern pattern) {
    Model.ParametricPattern parametricPattern = pattern.getParametric();
    List<Pair<Double, Double>> points = new ArrayList<>();
    double start = parametricPattern.getStartT();
    double step = parametricPattern.getTStep() * MILLIS_PER_STEP;
    if (step <= 0) {
      throw new IllegalArgumentException(
          "Expected step size in parametric equation to be positive, got " + step);
    }
    while (start < parametricPattern.getEndT()) {
     points.add(
         new Pair<>(
             evaluateEquation(start, parametricPattern.getEquationForXOrTheta()),
             evaluateEquation(start, parametricPattern.getEquationForYOrR())));
      start += step;
    }
    return interpolate(points);
  }

  private static double evaluateEquation(double t, Model.ParametricPattern.Equation equation) {
    switch (equation.getEquationTypeCase()) {
      case BINARY_EQUATION:
        return evaluateBinaryEquation(t, equation.getBinaryEquation());
      case UNARY_EQUATION:
        return evaluateUnaryEquation(t, equation.getUnaryEquation());
      case VARIABLE:
        return evaluateVariable(t, equation.getVariable());
      case CONSTANT:
        return equation.getConstant();
      default:
        throw new IllegalArgumentException("Found empty equation");
    }
  }

  private static double evaluateBinaryEquation(double t, Model.ParametricPattern.Equation.BinaryEquation binaryEquation) {
    double leftValue = evaluateEquation(t, binaryEquation.getEquationLeft());
    double rightValue = evaluateEquation(t, binaryEquation.getEquationRight());
    switch (binaryEquation.getOpertation()) {
      case ADD:
        return leftValue + rightValue;
      case SUBTRACT:
        return leftValue - rightValue;
      case MULTIPLY:
        return leftValue * rightValue;
      case DIVIDE:
        return leftValue / rightValue;
      case POWER:
        return Math.pow(leftValue, rightValue);
      case ROOT:
        if (leftValue == .5) {
          return Math.sqrt(rightValue);
        }
        return Math.pow(rightValue, 1 / leftValue);
      case LOG:
        if (leftValue == 2) {
          return Math.log(rightValue);
        }
        if (leftValue == 10) {
          return Math.log10(rightValue);
        }
        return Math.log(rightValue) / Math.log(leftValue);
      case ATAN2:
        return Math.atan2(leftValue, rightValue);
      default:
        throw new IllegalArgumentException("Unknown binary equation in parametric equation");
    }
  }

  private static double evaluateUnaryEquation(double t, Model.ParametricPattern.Equation.UnaryEquation unaryEquation) {
    double subValue = evaluateEquation(t, unaryEquation.getEquation());
    switch (unaryEquation.getOperation()) {
      case SIN:
        return Math.sin(subValue);
      case COS:
        return Math.cos(subValue);
      case TAN:
        return Math.tan(subValue);
      case ASIN:
        return Math.asin(subValue);
      case ACOS:
        return Math.acos(subValue);
      case ATAN:
        return Math.atan(subValue);
      case NEGATIVE:
        return -subValue;
      default:
        throw new IllegalArgumentException("Unknown unary equation in parametric equation");
    }
  }

  private static double evaluateVariable(double t, Model.ParametricPattern.Equation.Variable variable) {
    switch (variable) {
      case T:
        return t;
      default:
        throw new IllegalArgumentException("Unknown variable in parametric equation");
    }
  }
}
