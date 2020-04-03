function googRequire(path) {
  goog.require(path);
  const parts = path.split('.');
  let currentScope = window;
  for (const part of parts) {
    currentScope = currentScope[part];
  }
  return currentScope;
}

const ParametricPattern = googRequire('proto.sisyphus.ParametricPattern');
const Equation = googRequire('proto.sisyphus.ParametricPattern.Equation');
const BinaryEquation = googRequire('proto.sisyphus.ParametricPattern.Equation.BinaryEquation');
const BinaryOperationType = googRequire('proto.sisyphus.ParametricPattern.Equation.BinaryEquation.BinaryOperationType');
const UnaryEquation = googRequire('proto.sisyphus.ParametricPattern.Equation.UnaryEquation');
const UnaryOperationType = googRequire('proto.sisyphus.ParametricPattern.Equation.UnaryEquation.UnaryOperationType');
const Variable = googRequire('proto.sisyphus.ParametricPattern.Equation.Variable');
const Type = googRequire('proto.sisyphus.ParametricPattern.Type');

const context = document.querySelector('#canvas').getContext('2d');

function drawTable() {
  context.fillStyle = '#fff';
  context.fillRect(0, 0, 640, 640);

  context.lineWidth = 5;
  context.strokeStyle = '#ccc';
  context.beginPath();
  context.arc(320, 320, 310, 0, 2 * Math.PI);
  context.stroke();
}

drawTable();

const COORDS_PER_SECOND = 30;

let startTime;
document.querySelector('#render').onclick = () => {
  const xEquation = parseNode(math.parse(document.querySelector('#x').value));
  const yEquation = parseNode(math.parse(document.querySelector('#y').value));

  const coords = [];
  for (let t = 0; t < 1000; t++) {
    coords.push({
      a: evaluateEquation(t, xEquation),
      r: evaluateEquation(t, yEquation)
    });
  }

  const polarCoords = coords.map(coord => {
    return {
      x: Math.cos(coord.a) * coord.r,
      y: Math.sin(coord.a) * coord.r
    };
  });

  startTime = goog.now();
  requestAnimationFrame(applyFunctions.bind(this, polarCoords));
};

function applyFunctions(coords) {
  const timeDifference = goog.now() - startTime;

  const coordsToHighlight =
      Math.floor(timeDifference * COORDS_PER_SECOND / 1000) % coords.length;

  drawTable();

  context.translate(320, 320);
  context.lineWidth = 1;
  context.lineJoin = 'round';
  context.lineCap = 'round';

  context.strokeStyle = '#f0f0f0';
  context.beginPath();
  context.moveTo(coords[coordsToHighlight].x, coords[coordsToHighlight].y);
  for (let i = coordsToHighlight; i < coords.length; i++) {
    context.lineTo(coords[i].x, coords[i].y);
  }
  context.stroke();

  context.strokeStyle = '#000';
  context.beginPath();
  context.moveTo(coords[0].x, coords[0].y);
  for (let i = 1; i < coordsToHighlight; i++) {
    context.lineTo(coords[i].x, coords[i].y);
  }
  context.stroke();

  const pointerIndex = coordsToHighlight == 0 ? 0 : coordsToHighlight - 1;
  context.fillStyle = '#000';
  context.beginPath();
  context.arc(coords[pointerIndex].x, coords[pointerIndex].y, 5, 0, 2 * Math.PI);
  context.fill();
  context.translate(-320, -320);

  requestAnimationFrame(applyFunctions.bind(this, coords));
}

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
    case BinaryOperationType.ADD:
      return left + right;
    case BinaryOperationType.SUBTRACT:
      return left - right;
    case BinaryOperationType.MULTIPLY:
      return left * right;
    case BinaryOperationType.DIVIDE:
      return left / right;
    case BinaryOperationType.POWER:
      return Math.pow(left, right);
    case BinaryOperationType.ROOT:
      return Math.pow(left, 1 / right);
    case BinaryOperationType.LOG:
      return Math.log(right) / Math.log(left);
    case BinaryOperationType.ATAN2:
      return Math.atan2(left, right);
  }
}

function evaluateUnaryEquation(t, unaryEquation) {
  const subValue = evaluateEquation(t, unaryEquation.getEquation());
  switch (unaryEquation.getOperation()) {
    case UnaryOperationType.SIN:
      return Math.sin(subValue);
    case UnaryOperationType.COS:
      return Math.cos(subValue);
    case UnaryOperationType.TAN:
      return Math.tan(subValue);
    case UnaryOperationType.ASIN:
      return Math.asin(subValue);
    case UnaryOperationType.ACOS:
      return Math.acos(subValue);
    case UnaryOperationType.ATAN:
      return Math.atan(subValue);
    case UnaryOperationType.NEGATIVE:
      return -subValue;
  }
}

const operatorTypeMap = {
  'add': { type: BinaryOperationType.ADD, args: 2 },
  'subtract': { type: BinaryOperationType.SUBTRACT, args: 2 },
  'multiply': { type: BinaryOperationType.MULTIPLY, args: 2 },
  'divide': { type: BinaryOperationType.DIVIDE, args: 2 },
  'pow': { type: BinaryOperationType.POWER, args: 2 },
  'unaryMinus': { type: UnaryOperationType.NEGATIVE, args: 1 }
};

const functionTypeMap = {
  'sin': { type: UnaryOperationType.SIN, args: 1 },
  'cos': { type: UnaryOperationType.COS, args: 1 },
  'tan': { type: UnaryOperationType.TAN, args: 1 },
  'asin': { type: UnaryOperationType.ASIN, args: 1 },
  'acos': { type: UnaryOperationType.ACOS, args: 1 },
  'atan': { type: UnaryOperationType.ATAN, args: 1 },
  'log_2': { type: BinaryOperationType.LOG, args: 1, default_left: 2 },
  'sqrt': { type: BinaryOperationType.ROOT, args: 1, default_left: 2 },
  'atan2': { type: BinaryOperationType.ATAN2, args: 2 },
};

function parseNode(node) {
  switch (node.type) {
    case 'ConstantNode':
      return parseConstantNode(node);
    case 'OperatorNode':
      return parseOperatorNode(node);
    case 'FunctionNode':
      return parseFunctionNode(node);
    case 'ParenthesisNode':
      return parseParenthesisNode(node);
    case 'SymbolNode':
      return parseSymbolNode(node);
    default:
      throw `Unsupported node type ${node.type} at ${node.toString()}`;
  }
}

function parseConstantNode(node) {
  const equation = new Equation();
  equation.setConstant(parseFloat(node.value));
  return equation;
}

function parseOperatorNode(node) {
  const op = operatorTypeMap[node.fn];
  if (!op) {
    throw `Can not find supported operator for ${node.toString()}`;
  }
  if (node.args.length != op.args) {
    throw `Unexpected number of arguments for ${node.toString()}`;
  }
  if (op.args == 2) {
    return binaryEquation(op.type, parseNode(node.args[0]), parseNode(node.args[1]));
  } else if (op.args == 1) {
    return unaryEquation(op.type, parseNode(node.args[0]));
  } else {
    throw `Unsupported number of args ${op.args}`;
  }
}

function parseFunctionNode(node) {
  const fn = functionTypeMap[node.name];
  if (!fn) {
    throw `Can not find supported function for ${node.toString()}`;
  }
  if (node.args.length != fn.args) {
    throw `Unexpected number of arguments for ${node.toString()}`;
  }
  if (fn.args == 2) {
    return binaryEquation(fn.type, parseNode(node.args[0]), parseNode(node.args[1]));
  } else if (fn.args == 1) {
    if (typeof fn.default_left !== 'undefined') {
      return binaryEquation(fn.type, parseConstantNode({value: fn.default_left}), parseNode(node.args[0]));
    }
    return unaryEquation(fn.type, parseNode(node.args[0]));
  } else {
    throw `Unsupported number of args ${fn.args}`;
  }
}

function parseParenthesisNode(node) {
  return parseNode(node.content);
}

function parseSymbolNode(node) {
  if (node.name !== 't') {
    throw `Unsupported symbol ${node.name}`;
  }
  const equation = new Equation();
  equation.setVariable(Variable.T);
  return equation;
}

function binaryEquation(type, left, right) {
  const equation = new Equation();
  const binaryEquation = new BinaryEquation();
  binaryEquation.setOperation(type);
  binaryEquation.setEquationLeft(left);
  binaryEquation.setEquationRight(right);
  equation.setBinaryEquation(binaryEquation);
  return equation;
}

function unaryEquation(type, inner) {
  const equation = new Equation();
  const unaryEquation = new UnaryEquation();
  unaryEquation.setOperation(type);
  unaryEquation.setEquation(inner);
  equation.setUnaryEquation(unaryEquation);
  return equation;
}
