#include "Tracer.hpp"

Verbose Tracer::globalVerbose;//allocate space for the static variable

Tracer::Tracer(Verbose verbose):verbose(verbose){
  
  if(globalVerbose >= verbose) std::cout<<verbose<<": ";//print verbose level used by the tracer on first use, i. e. at construction

}

Tracer& Tracer::operator<<(std::ostream&(*manipulator)(std::ostream&)){
  
  if(globalVerbose >= verbose) std::cout<<*manipulator;
  return *this;

}

void Tracer::setGlobalVerbosity(Verbose globalVerbose){
  
  Tracer::globalVerbose = globalVerbose;

}