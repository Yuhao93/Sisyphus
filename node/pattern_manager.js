const StepperMotors = require('./stepper_motors');
const Gpio = require('./gpio');
const EnqueuedPatternIterator = require('./enqueued_pattern_iterator');
const InitializingPatternIterator = require('./initializing_pattern_iterator');
const os = require('./os');

const MAX_HISTORY = 10;

class PatternManager {
  constructor() {
    this.stepperMotors = new StepperMotors();
    this.patterns = [];
    this.seenPatterns = [];
    Gpio.setup();
  }

  queuePattern(pattern, id) {
    if (!os.isWindows()) {
      this.patterns.push(new InitializingPatternIterator(pattern));
    }
    this.patterns.push(new EnqueuedPatternIterator(pattern, id));
  }

  listPatterns() {
    const patternsCopy = [];
    for (const pattern of this.patterns) {
      if (pattern.isExternalPattern()) {
        patternsCopy.push(pattern.pattern());
      }
    }
    return patternsCopy;
  }

  async step() {
    if (this.patterns.length == 0) {
      return;
    }
    if (!this.patterns[0].hasNext()) {
      const pattern = this.patterns.shift();
      if (pattern.isExternalPattern()) {
        this.seenPatterns.push(pattern);
        if (this.seenPatterns.length > MAX_HISTORY) {
          this.seenPatterns.shift();
        }
      }
    }
    if (this.patterns.length > 0) {
      await this.stepperMotors.step(this.patterns[0].next());
    }
  }
}

module.exports = PatternManager;
