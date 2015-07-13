#ifndef TRACER_H
#define TRACER_H

#include <iostream>
#include <map>

class Tracer{
  
public:
  enum Verbose{Quiet, Error, Warning, Debug};
  static Verbose globalVerbose;
  Tracer(Verbose verbose);
  template <class T>
  Tracer& operator<<(const T& object);
  Tracer& operator<<(std::ostream&(*manipulator)(std::ostream&));//take manipulator functions such as std::endl
  
private:
  Verbose verbose;
  
};

std::ostream& operator<<(std::ostream& output, Tracer::Verbose verbose);

template <class T>
Tracer& Tracer::operator<<(const T& object){
  
  if(globalVerbose >= verbose) std::cout<<object;

  return *this;
  
}

#endif