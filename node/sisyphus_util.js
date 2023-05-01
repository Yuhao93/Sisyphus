const Model = require('./gen/model_pb');
const {ANGULAR_STEPS_PER_REVOLUTION, LINEAR_RANGE_STEPS} = require('./table_constants');
const {getCoordinatesFromParametricFunction} = require('./parametric_processor');

function polarToPoint(polar) {
  const point = new Model.Segment.Point();
  point.setAngularValue(Math.floor(ANGULAR_STEPS_PER_REVOLUTION * ClampBetween2Pi(polar.getA()) / (2 * Math.PI)));
  point.setLinearValue(Math.floor(LINEAR_RANGE_STEPS * polar.getR()));
  return point;
}

function pointToPolar(point) {
  const polar = new Model.PolarCoordinate();
  polar.setA((point.getAngularValue() * 2 * Math.PI) / ANGULAR_STEPS_PER_REVOLUTION);
  polar.setR(point.getLinearValue() / LINEAR_RANGE_STEPS);
  return polar;
}

function segmentFromPoints(start, end) {
  const segment = Model.Segment();
  segment.setStart(polarToPoint(start));
  segment.setEnd(polarToPoint(end));
  return segment;
}

function segmentIsPoint(segment) {
  return segment.getStart().getAngularValue() == segment.getEnd().getAngularValue()
      && segment.getStart().getLinearValue() == segment.getEnd().getLinearValue();
}

function clampBetween2Pi(val) {
  const pi2 = 2 * Math.PI;
  if (val < 0) {
    const cycles = Math.floor(-val / pi2);
    return val + (cycles + 1) * pi2;
  } else if (val >= pi2) {
    const cycles = Math.floor(val / pi2);
    return val - cycles * pi2;
  }
  return val;
}

function diffBetweenAngles(angle1, angle2) {
  const diff = angle1 - angle2;
  const diffMagnitude = Math.abs(diff);
  return diffMagnitude > (ANGULAR_STEPS_PER_REVOLUTION / 2)
      ? (ANGULAR_STEPS_PER_REVOLUTION - diffMagnitude) * (diff < 0 ? 1 : -1)
      : diff;
}

function polarFromCartesian(coordinate) {
  const x = coordinate.getX();
  const y = coordinate.getY();
  const r = Math.sqrt(x * x + y * y);
  const a = Math.atan2(y, x);
  const newCoordinate = Model.PolarCoordinate();
  newCoordinate.setR(r);
  newCoordinate.setA(a);
  return newCoordinate;
}

function patternFromPolarCoordinates(coordinates) {
  const p = new Model.Pattern();
  if (coordinates.length == 0) {
    return p;
  }
  if (coordinates.length == 1) {
    p.addPathSegment(segmentFromPoints(coordinates[0], coordinates[0]));
    return p;
  }
  let prev = coordinates[0];
  for (let i = 1; i < coordinates.size(); i++) {
    const current = coordinates[i];
    const segment = segmentFromPoints(prev, current);
    if (!segmentIsPoint(segment)) {
      p.addPathSegment(segment);
    }
    prev = current;
  }
  if (p.getPathSegmentList().length == 0) {
    p.addPathSegment(segmentFromPoints(prev, prev));
  }
  return p;
}

function patternFromCartesianCoordinates(coordinates) {
  const polar = [];
  for (const coordinate of coordinates) {
    polar.push(polarFromCartesian(coordinate));
  }
  return patternFromPolarCoordinates(polar);
}

function storedPatternToPattern(pattern) {
  if (pattern.hasCartesian()) {
    return patternFromCartesianCoordinates(pattern.getCartesian().getCoordinateList());
  } else if (pattern.hasPolar()) {
    return patternFromPolarCoordinates(pattern.getPolar().getCoordinateList());
  } else if (pattern.hasParametric()) {
    return patternFromCartesianCoordinates(
      getCoordinatesFromParametricFunction(pattern.getParametric()));
  }
  return new Model.Pattern();
}

function patternToPolarStoredPattern(pattern) {
  const storedPattern = new Model.StoredPattern();
  if (pattern.getPathSegmentList().length == 0) {
    return storedPattern;
  }

  const polarCoords = [];
  polarCoords.push(pointToPolar(pattern.getPathSegmentList()[0].getStart()));

  for (const segment of pattern.getPathSegmentList()) {
    polarCoords.push(pointToPolar(segment.end()));
  }
  storedPattern.setPolar(polarCoords);
  return storedPattern;
}

module.exports = {
  clampBetween2Pi,
  diffBetweenAngles,
  polarFromCartesian,
  patternFromPolarCoordinates,
  patternFromCartesianCoordinates,
  storedPatternToPattern,
  patternToPolarStoredPattern
};