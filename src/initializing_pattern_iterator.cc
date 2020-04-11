#include "initializing_pattern_iterator.h"

#include "gpio.h"
#include "wiring_pi_wrapper.h"
#include "enqueued_pattern_iterator.h"
#include "table_constants.h"
#include "sisyphus_util.h"
#include "model.pb.h"

namespace {

EnqueuedPatternIterator ParametricToIterator(const sisyphus::StoredPattern& pattern) {
  return EnqueuedPatternIterator(SisyphusUtil::StoredPatternToPattern(pattern));
}

// Spiral circling from inside out
EnqueuedPatternIterator SweepPattern() {
  sisyphus::StoredPattern stored_pattern;
  auto* pattern = stored_pattern.mutable_parametric();
  pattern->set_start_t(0);
  pattern->set_end_t(5000);
  pattern->set_t_step(1);
  pattern->set_type(sisyphus::ParametricPattern::POLAR);

  auto* theta = pattern->mutable_equation_for_x_or_theta()->mutable_binary_equation();
  theta->set_operation(sisyphus::ParametricPattern_Equation_BinaryEquation::DIVIDE);
  theta->mutable_equation_left()->set_variable(sisyphus::ParametricPattern_Equation::T);
  theta->mutable_equation_right()->set_constant(15);

  auto* r = pattern->mutable_equation_for_y_or_r()->mutable_binary_equation();
  r->set_operation(sisyphus::ParametricPattern_Equation_BinaryEquation::MULTIPLY);
  r->mutable_equation_left()->set_variable(sisyphus::ParametricPattern_Equation::T);
  // 0.95 * .0002
  r->mutable_equation_right()->set_constant(0.00019);

  return ParametricToIterator(stored_pattern);
}

// Spiral circling from outside in
EnqueuedPatternIterator TargetPattern() {
  sisyphus::StoredPattern stored_pattern;
  auto* pattern = stored_pattern.mutable_parametric();
  pattern->set_start_t(0);
  pattern->set_end_t(5000);
  pattern->set_t_step(1);
  pattern->set_type(sisyphus::ParametricPattern::POLAR);

  auto* theta = pattern->mutable_equation_for_x_or_theta()->mutable_binary_equation();
  theta->set_operation(sisyphus::ParametricPattern_Equation_BinaryEquation::DIVIDE);
  theta->mutable_equation_left()->set_variable(sisyphus::ParametricPattern_Equation::T);
  theta->mutable_equation_right()->set_constant(-15);

  auto* r = pattern->mutable_equation_for_y_or_r()->mutable_binary_equation();
  r->set_operation(sisyphus::ParametricPattern_Equation_BinaryEquation::SUBTRACT);
  r->mutable_equation_left()->set_constant(0.95);
  auto* sub_r = r->mutable_equation_right()->mutable_binary_equation();
  sub_r->set_operation(sisyphus::ParametricPattern_Equation_BinaryEquation::MULTIPLY);
  sub_r->mutable_equation_left()->set_variable(sisyphus::ParametricPattern_Equation::T);
  // 0.95 * .0002
  sub_r->mutable_equation_right()->set_constant(0.00019);

  return ParametricToIterator(stored_pattern);
}

EnqueuedPatternIterator sweep_out = SweepPattern();
EnqueuedPatternIterator sweep_in = TargetPattern();

}

InitializingPatternIterator::InitializingPatternIterator(const sisyphus::Pattern& pattern)
    : stage(INITIALIZING_STAGE_ZEROING), current(0), complete(false),
    zero_count(0), center_steps(0) {
  if (pattern.path_segment().size() < 1) {
    return;
  }
  target = pattern.path_segment(0).start().linear_value();
}

bool InitializingPatternIterator::has_next() const {
  return complete;
}

const sisyphus::Step& InitializingPatternIterator::next() {
  sisyphus::Step step;
  switch (stage) {
    case INITIALIZING_STAGE_ZEROING:
      if (Gpio::read(GPIO_LINEAR_FEEDBACK) == LOW) {
        zero_count = 0;
        step.set_linear_movement(sisyphus::Step::BACKWARDS);
      } else {
        if (++zero_count == ZEROING_THRESHOLD) {
          stage = INITIALIZING_STAGE_CENTERING;
        }
      }
      break;

    case INITIALIZING_STAGE_CENTERING:
      if (center_steps <= LINEAR_CENTER_OFFSET_STEPS) {
        step.set_linear_movement(sisyphus::Step::FORWARDS);
        center_steps++;
      } else {
        stage = INITIALIZING_STAGE_CLEARING;
      }
      break;

    case INITIALIZING_STAGE_CLEARING:
      if (sweep_out.has_next()) {
        const sisyphus::Step& s = sweep_out.next();
        step.set_linear_movement(s.linear_movement());
        step.set_angular_movement(s.angular_movement());
        if (s.linear_movement() == sisyphus::Step::FORWARDS) {
          current++;
        }
      } else {
        stage = INITIALIZING_STAGE_TARGETING;
      }
      break;

    case INITIALIZING_STAGE_TARGETING:
      if (!sweep_out.has_next()) {
        complete = true;
        break;
      }
      const sisyphus::Step& s = sweep_out.next();
      step.set_linear_movement(s.linear_movement());
      step.set_angular_movement(s.angular_movement());
      if (s.linear_movement() == sisyphus::Step::BACKWARDS) {
        current--;
      }
      if (current <= target) {
        complete = true;
      }
      break;
  }
  return std::move(step);
}

bool InitializingPatternIterator::is_external_pattern() const {
  return false;
}
