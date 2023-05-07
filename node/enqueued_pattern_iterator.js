const Model = require('./gen/model_pb');
const {ANGULAR_STEPS_PER_REVOLUTION} = require('./table_constants');
const SisyphusUtil = require('./sisyphus_util');
const AbstractPatternIterator = require('./abstract_pattern_iterator');

function octantFromSegment(segment) {
  const x = SisyphusUtil.diffBetweenAngles(segment.getEnd().getAngularValue(), segment.getStart().getAngularValue());
  const y = segment.getEnd().getLinearValue() - segment.getStart().getLinearValue();
  let angle = 4 * Math.atan2(y, x) / Math.PI;

  if (angle < 0) {
    angle += 8;
  }

  return Math.floor(angle);
}

function getIsFlipped(octant) {
  switch(octant) {
    case 1:
    case 2:
    case 5:
    case 6:
      return true;
    default:
      return false;
  }
}

function setXYWithOctant(octant, segment, thisContext) {
  const a = segment.getStart().getAngularValue();
  const l = segment.getStart().getLinearValue();
  const da = SisyphusUtil.diffBetweenAngles(segment.getEnd().getAngularValue(), a);
  const dl = segment.getEnd().getLinearValue() - l;

  switch(octant) {
    case 0:
      thisContext.x = a;
      thisContext.dx = da;
      thisContext.y = l;
      thisContext.dy = dl;
      thisContext.reverseX = false;
      thisContext.reverseY = false;
      break;
    case 1:
      thisContext.x = l;
      thisContext.dx = dl;
      thisContext.y = a;
      thisContext.dy = da;
      thisContext.reverseX = false;
      thisContext.reverseY = false;
      break;
    case 2:
      thisContext.x = l;
      thisContext.dx = dl;
      thisContext.y = -a;
      thisContext.dy = -da;
      thisContext.reverseX = false;
      thisContext.reverseY = true;
      break;
    case 3:
      thisContext.x = -a;
      thisContext.dx = -da;
      thisContext.y = l;
      thisContext.dy = dl;
      thisContext.reverseX = true;
      thisContext.reverseY = false;
      break;
    case 4:
      thisContext.x = -a;
      thisContext.dx = -da;
      thisContext.y = -l;
      thisContext.dy = -dl;
      thisContext.reverseX = true;
      thisContext.reverseY = true;
      break;
    case 5:
      thisContext.x = -l;
      thisContext.dx = -dl;
      thisContext.y = -a;
      thisContext.dy = -da;
      thisContext.reverseX = true;
      thisContext.reverseY = true;
      break;
    case 6:
      thisContext.x = -l;
      thisContext.dx = -dl;
      thisContext.y = a;
      thisContext.dy = da;
      thisContext.reverseX = true;
      thisContext.reverseY = false;
      break;
    case 7:
      thisContext.x = a;
      thisContext.dx = da;
      thisContext.y = -l;
      thisContext.dy = -dl;
      thisContext.reverseX = false;
      thisContext.reverseY = true;
      break;
  }
}

function getALWithOctant(octant, x, y) {
  let a;
  let l;
  switch(octant) {
    case 0:
      a = x;
      l = y;
      break;
    case 1:
      a = y;
      l = x;
      break;
    case 2:
      a = -y;
      l = x;
      break;
    case 3:
      a = -x;
      l = y;
      break;
    case 4:
      a = -x;
      l = -y;
      break;
    case 5:
      a = -y;
      l = -x;
      break;
    case 6:
      a = y;
      l = -x;
      break;
    case 7:
      a = x;
      l = -y;
      break;
  }
  return {a, l};
}

function getDirection(reverse) {
  return reverse ? Model.Step.Movement.BACKWARDS : Model.Step.Movement.FORWARDS;
}

class EnqueuedPatternIterator extends AbstractPatternIterator {
  constructor(pattern, id) {
		super();
		this.p = pattern;
		this.id = id;
    this.currentSegment = null;
    this.segmentIndex = 0;
    this.octant = 0;
    this.x = 0;
    this.y = 0;
    this.dx = 0;
    this.dy = 0;
    this.D = 0;
    this.reverseX = false;
    this.reverseY = false;
    this.reset();
  }

  reset() {
    if (this.p.getPathSegmentList().length < 1) {
      return;
    }
    this.segmentIndex = 0;
    this.currentSegment = this.p.getPathSegmentList()[0];
    this.initializeSegment(this.currentSegment);
  }

  initializeSegment(segment) {
    this.octant = octantFromSegment(this.currentSegment);
    setXYWithOctant(this.octant, segment, this);
    this.D = 2 * this.dy - this.dx;
  }

  pattern() {
    return {
			pattern: this.p,
			id: this.id
		};
  }

  hasNext() {
    return this.segmentIndex < this.p.getPathSegmentList().length;
  }

  isExternalPattern() {
    return true;
  }

  next() {
    const isFlipped = getIsFlipped(this.octant);

    const directions = {
      aDir: Model.Step.Movement.STOP,
      lDir: Model.Step.Movement.STOP
    };
    const xDir = isFlipped ? 'lDir' : 'aDir';
    const yDir = isFlipped ? 'aDir' : 'lDir';
    const angularValue = isFlipped ? 'y' : 'x';

    this.x ++;
    directions[xDir] = getDirection(this.reverseX);
    if (this.D > 0) {
      this.y ++;
      directions[yDir] = getDirection(this.reverseY);
      this.D -= 2 * this.dx;
    }
    this.D += 2 * this.dy;
    const angle = this[angularValue];
    if (angle >= ANGULAR_STEPS_PER_REVOLUTION) {
      this[angularValue] -= (ANGULAR_STEPS_PER_REVOLUTION * (angle / ANGULAR_STEPS_PER_REVOLUTION));
    }
    let {a, l} = getALWithOctant(this.octant, this.x, this.y);
    if (a < 0) {
      a += (ANGULAR_STEPS_PER_REVOLUTION * ((-a / ANGULAR_STEPS_PER_REVOLUTION) + 1));
    }
    const targetA = this.currentSegment.getEnd().getAngularValue();
    const targetL = this.currentSegment.getEnd().getLinearValue();

    const step = new Model.Step();
    step.setAngularMovement(directions.aDir);
    step.setLinearMovement(directions.lDir);

    if (a == targetA && l == targetL) {
      if (++this.segmentIndex < this.p.getPathSegmentList().length) {
        this.currentSegment = this.p.getPathSegmentList()[this.segmentIndex];
        this.initializeSegment(this.currentSegment);
      }
    }
    
    return step;
  }
}


module.exports = EnqueuedPatternIterator;
