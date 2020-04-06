#include "thread_wrapper.h"

#include <windows.h>
#include <cstdio>

DWORD WINAPI RunThread(LPVOID lpParam) {
  ((Thread*)lpParam)->RunThread(nullptr);
}

ThreadWrapper::ThreadWrapper(Thread* p) {
  handle = CreateThread(NULL, 0, RunThread, p, 0, NULL);
}
