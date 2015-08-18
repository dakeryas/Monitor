#ifndef TRACER_H
#define TRACER_H

#include <atomic>
#include "Verbose.hpp"

class Tracer{
  
public:
  Tracer(Verbose verbose);
  template <class T>
  Tracer& operator<<(const T& object);
  Tracer& operator<<(std::ostream&(*manipulator)(std::ostream&));//take manipulator functions such as std::endl
  static void setGlobalVerbosity(Verbose globalVerbose);
  
private:
  static std::atomic<Verbose> globalVerbose;//declare the static variable; they can only be public (in a sense)
  Verbose verbose;
  
};

template <class T>
Tracer& Tracer::operator<<(const T& object){
  
  if(globalVerbose >= verbose) std::cout<<object;

  return *this;
  
}

#endif