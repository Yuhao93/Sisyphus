#include "pattern_thread.h"

void PatternThread::Run() {
  pattern_manager_->step();
}
