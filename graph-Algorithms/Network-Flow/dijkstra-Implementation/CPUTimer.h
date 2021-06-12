#ifndef CPUTIMER_H
#define CPUTIMER_H

#include <time.h>
#include <iostream>

using namespace std;

// CPUTimer
//
// For ECS 110 - UC Davis - By Ted Krovetz
//
// This class is a convenient way to count CPU time.
// The ANSI C function clock() is used to get the current
// user + system time that the current application
// (and it's child processes) has expended.
// At creation, a variable of CPUTimer type records
// the current value of clock().
//
// reset() - sets the stored variable to the current
//           value of clock(). In essence, resetting the clock.
//
// cur_CPUTime() - returns the difference between the current
//                 clock() value and the previously stored value.
//                 In essence, returning how much CPU time has passed.
//
// Example: to time a function (possibly main())
//
//   #include "CPUTimer"
//   void foo(void) {
//     CPUTimer ct;
//     ... whatever foo does ...
//     cerr << ct.cur_CPUTime() << endl;
//   }

class CPUTimer {
private:
  clock_t tick_count;
    
public:
  CPUTimer(void);
  void reset(void);
  double cur_CPUTime(void);
};

// AutoCPUTimer
//
// AutoCPUTimer is derived through C++ inheritance. It
// inherits all the public members of CPUTimer, but
// includes a destructor which will automatically
// output the CPU time used to cerr (stderr).
// Example: to time a function (possibly main())
//
//   #include "CPUTimer"
//   void foo(void) {
//     AutoCPUTimer at;
//     ... whatever foo does ...
//   }
//
// This example will have identical output to the
// previous example, however the output to cerr is
// done automatically,.

class AutoCPUTimer : public CPUTimer {
public:
  ~AutoCPUTimer(void);
};

// Implementation --
// It is generally not good to expose the mechanics of your ADT
// In the public interface (i.e. the header file). It is here
// however, to make program timing as simple as possible.
// There is _NO_ .cpp file for these classes. #include'ing
// is sufficient for their use.

CPUTimer::CPUTimer(void)
{
  tick_count = clock();
}

void CPUTimer::reset(void)
{ 
  tick_count = clock();
}

double CPUTimer::cur_CPUTime(void)
{
  return double(clock() - tick_count) / CLOCKS_PER_SEC;
}

AutoCPUTimer::~AutoCPUTimer(void)
{
  cerr << cur_CPUTime() << endl;
}

#endif

