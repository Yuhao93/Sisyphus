const Model = require('./gen/model_pb');
const Equation = Model.ParametricPattern.Equation;

function evaluateEquation(t, equation) {
  switch (equation.getEquationTypeCase()) {
    case Equation.EquationTypeCase.BINARY_EQUATION:
      return evaluateBinaryEquation(t, equation.getBinaryEquation());
    case Equation.EquationTypeCase.UNARY_EQUATION:
      return evaluateUnaryEquation(t, equation.getUnaryEquation());
    case Equation.EquationTypeCase.CONSTANT:
      return equation.getConstant();
    case Equation.EquationTypeCase.VARIABLE:
      return t;
  }
}

function evaluateBinaryEquation(t, binaryEquation) {
  const left = evaluateEquation(t, binaryEquation.getEquationLeft());
  const right = evaluateEquation(t, binaryEquation.getEquationRight());
  switch (binaryEquation.getOperation()) {
    case Equation.BinaryEquation.BinaryEquationType.ADD:
      return left + right;
    case Equation.BinaryEquation.BinaryEquationType.SUBTRACT:
      return left - right;
    case Equation.BinaryEquation.BinaryEquationType.MULTIPLY:
      return left * right;
    case Equation.BinaryEquation.BinaryEquationType.DIVIDE:
      return left / right;
    case Equation.BinaryEquation.BinaryEquationType.POWER:
      return Mathy.pow(left, right);
    case Equation.BinaryEquation.BinaryEquationType.ROOT:
      if (left == 0 || (right == 0 && left <= 0) || (right < 0 && left != Math.abs(left))) {
        return 0;
      }
      return pow(right, 1 / left);
    case Equation.BinaryEquation.BinaryEquationType.LOG:
      if (left <= 0 || right <= 0 || left == 1) {
        return 0;
      }
      return Math.log(right) / Math.log(left);
    case Equation.BinaryEquation.BinaryEquationType.ATAN2:
      return Math.atan2(left, right);
  }
}

function evaluateUnaryEquation(t, unaryEquation) {
  subValue = evaluateEquation(t, unaryEquation.getEquation());
  switch (unaryEquation.getOperation()) {
    case Equation.UnaryEquation.UnaryEquationType.SIN:
      return Math.sin(subValue);
    case Equation.UnaryEquation.UnaryEquationType.COS:
      return Math.cos(subValue);
    case Equation.UnaryEquation.UnaryEquationType.TAN:
      return Math.tan(subValue);
    case Equation.UnaryEquation.UnaryEquationType.ASIN:
      return Math.asin(subValue);
    case Equation.UnaryEquation.UnaryEquationType.ACOS:
      return Math.acos(subValue);
    case Equation.UnaryEquation.UnaryEquationType.ATAN:
      return Math.atan(subValue);
    case Equation.UnaryEquation.UnaryEquationType.NEGATIVE:
      return -subValue;
    case Equation.UnaryEquation.UnaryEquationType.STEP:
      return subValue >= 0 ? 1 : 0;
    case Equation.UnaryEquation.UnaryEquationType.SIGN:
      return subValue == 0 ? 0 : (subValue > 0 ? 1 : -1);
    case Equation.UnaryEquation.UnaryEquationType.ABS:
      return Math.abs(subValue);
  }
}

function getCoordinatesFromParametricFunction(pattern) {
  const result = [];
  for (let t = pattern.getStartT(); t <= pattern.getEndT(); t += pattern.getTStep()) {
    const xOrTheta = evaluateEquation(t, pattern.getEquationForXOrTheta());
    const yOrR = evaluateEquation(t, pattern.getEquationForYOrR());
    const coordinate = new Model.CartesianCoordinate();
    switch (pattern.type()) {
      case Model.ParametricPattern.Type.CARTESIAN:
        coordinate.setX(xOrTheta);
        coordinate.setY(yOrR);
        break;
      case Model.ParametricPattern.Type.POLAR:
        coordinate.setX(yOrR * cos(xOrTheta));
        coordinate.setY(yOrR * sin(xOrTheta));
        break;
    }
    result.push(coordinate);
  }
  return result;
}

module.exports = {getCoordinatesFromParametricFunction};