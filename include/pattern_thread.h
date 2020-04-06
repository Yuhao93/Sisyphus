#ifndef PATTERN_THREAD_H
#define PATTERN_THREAD_H

#include "application.h"
#include "thread.h"
#include "pattern_manager.h"

class PatternThread : public Thread {
public:
  PatternThread(Application* app)
      : pattern_manager_(&app->pattern_manager) {}
  void Run();

private:
   PatternManager* pattern_manager_;
};

#endif
