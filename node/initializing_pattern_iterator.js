const AbstractPatternIterator = require('./abstract_pattern_iterator');
const EnqueuedPatternIterator = require('./enqueued_pattern_iterator');
const Model = require('./gen/model_pb');
const Gpio = require('./gpio');
const TableConstants = require('./table_constants');
const SisyphusUtil = require('./sisyphus_util');

const INITIALIZING_STAGE_ZEROING = 1;
const INITIALIZING_STAGE_CENTERING = 2;
const INITIALIZING_STAGE_CLEARING = 3;
const INITIALIZING_STAGE_TARGETING = 4;

const ZEROING_THRESHOLD = 100;

function parametricToIterator(pattern) {
  return new EnqueuedPatternIterator(SisyphusUtil.storedPatternToPattern(pattern));
}

function binaryEquation(eq) {
  const equation = new Model.ParametricPattern.Equation();
  equation.setBinaryEquation(eq);
  return equation;
}

function variable() {
  const variable = new Model.ParametricPattern.Equation();
  variable.setVariable(Model.ParametricPattern.Equation.Variable.T);
  return variable;
}

function constant(constantVal) {
  const constant = new Model.ParametricPattern.Equation();
  constant.setConstant(constantVal);
  return constant;
}

// Spiral circling from inside out
function sweepPattern() {
  const storedPattern = new Model.StoredPattern();
  const parametricPattern = new Model.ParametricPattern();
  parametricPattern.setStartT(0);
  parametricPattern.setEndT(5000);
  parametricPattern.setTStep(1);
  parametricPattern.setType(Model.ParametricPattern.Type.POLAR);

  const theta = new Model.ParametricPattern.Equation.BinaryEquation();
  theta.setOperation(Model.ParametricPattern.Equation.BinaryEquation.BinaryEquationType.DIVIDE);
  theta.setEquationLeft(variable());
  theta.setEquationRight(constant(20));
  parametricPattern.setEquationForXOrTheta(binaryEquation(theta));

  const r = new Model.ParametricPattern.Equation.BinaryEquation();
  r.setOperation(Model.ParametricPattern.Equation.BinaryEquation.BinaryEquationType.MULTIPLY);
  r.setEquationLeft(variable());
  r.setEquationRight(constant(0.00019));
  parametricPattern.setEquationForYOrR(binaryEquation(r));

  storedPattern.setParametric(parametricPattern);

  return parametricToIterator(storedPattern);
}

// Spiral circling from outside in
function targetPattern() {
  const storedPattern = new Model.StoredPattern();
  const parametricPattern = new Model.ParametricPattern();
  parametricPattern.setStartT(0);
  parametricPattern.setEndT(5000);
  parametricPattern.setTStep(1);
  parametricPattern.setType(Model.ParametricPattern.Type.POLAR);

  const theta = new Model.ParametricPattern.Equation.BinaryEquation();
  theta.setOperation(Model.ParametricPattern.Equation.BinaryEquation.BinaryEquationType.DIVIDE);
  theta.setEquationLeft(variable());
  theta.setEquationRight(constant(-20));
  parametricPattern.setEquationForXOrTheta(binaryEquation(theta));

  const r = new Model.ParametricPattern.Equation.BinaryEquation();
  r.setOperation(Model.ParametricPattern.Equation.BinaryEquation.BinaryEquationType.SUBTRACT);
  r.setEquationLeft(constant(0.95));
  const subR = new Model.ParametricPattern.Equation.BinaryEquation();
  subR.setOperation(Model.ParametricPattern.Equation.BinaryEquation.BinaryEquationType.MULTIPLY);
  subR.setEquationLeft(variable());
  subR.setEquationRight(constant(0.00019));
  r.setEquationRight(subR);
  parametricPattern.setEquationForYOrR(binaryEquation(r));

  storedPattern.setParametric(parametricPattern);

  return parametricToIterator(storedPattern);
}

class InitializingPatternIterator extends AbstractPatternIterator {
  constructor(pattern) {
    this.target = 0;
    this.current = 0;
    this.stage = INITIALIZING_STAGE_ZEROING;
    this.zeroCount = 0;
    this.centerSteps = 0;
    this.complete = false;
    this.clearingPattern = new EnqueuedPatternIterator(sweepPattern());
    this.targetingPattern = new EnqueuedPatternIterator(targetPattern());

    if (pattern.getPathSegmentList().length < 1) {
      return;
    }
    this.clearingPattern.reset();
    this.targetingPattern.reset();
    this.target = pattern.getPathSegmentList()[0].getStart().getLinearValue();
  }

  hasNext() {
    return !this.complete;
  }

  next() {
    const step = new Model.Step();
    step.setAngularMovement(Model.Step.Movement.STOP);
    step.setLinearMovement(Model.Step.Movement.STOP);

    switch (this.stage) {
      case INITIALIZING_STAGE_ZEROING:
        if (Gpio.read(Gpio.GPIO_LINEAR_FEEDBACK) == Gpio.LOW) {
          this.zeroCount = 0;
          step.setLinearMovement(Model.Step.Movement.BACKWARDS);
        } else if (++this.zeroCount == ZEROING_THRESHOLD) {
          console.log("CENTERING");
          this.stage = INITIALIZING_STAGE_CENTERING;
        }
        break;
      case INITIALIZING_STAGE_CENTERING:
        if (this.centerSteps <= TableConstants.LINEAR_CENTER_OFFSET_STEPS) {
          step.setLinearMovement(Model.Step.Movement.FORWARDS);
          this.centerSteps++;
        } else {
          console.log("CLEARING");
          this.stage = INITIALIZING_STAGE_CLEARING;
        }
        break;
      case INITIALIZING_STAGE_CLEARING:
        if (this.clearingPattern.hasNext()) {
          const s = this.clearingPattern.next();
          step.setLinearMovement(s.getLinearMovement());
          step.setAngularMovement(s.getAngularMovement());
          if (s.getLinearMovement() == Model.Step.Movement.FORWARDS) {
            this.current++;
          }
        } else {
          console.log("TARGETING");
          this.stage = INITIALIZING_STAGE_TARGETING;
        }
        break;
      case INITIALIZING_STAGE_TARGETING:
        if (!this.targetingPattern.hasNext()) {
          console.log("DONE");
          this.complete = true;
          break;
        }
        const s = this.targetingPattern.next();
        step.setLinearMovement(s.getLinearMovement());
        step.setAngularMovement(s.getAngularMovement());
        if (s.getLinearMovement() == Model.Step.Movement.BACKWARDS) {
          this.current--;
        }
        if (this.current <= this.target) {
          console.log("DONE");
          this.complete = true;
        }
        break;
    }
    return step;
  }

  isExternalPattern() {
    return false;
  }
}

module.exports = InitializingPatternIterator;