#include "Tracer.hpp"

std::map <Tracer::Verbose, std::string> verboseMap = {{Tracer::Quiet, "Info"}, {Tracer::Error, "Error"}, {Tracer::Warning, "Warning"}, {Tracer::Debug, "Debug"}};

std::ostream& operator<<(std::ostream& output, Tracer::Verbose verbose){
  
  output<<verboseMap[verbose];
  return output;

}

Tracer::Tracer(Verbose verbose):verbose(verbose){
  
  if(globalVerbose >= verbose) std::cout<<verbose<<": ";//print verbose level used by the tracer on first use, i. e. at construction

}

Tracer& Tracer::operator<<(std::ostream&(*manipulator)(std::ostream&)){
  
  if(globalVerbose >= verbose) std::cout<<*manipulator;
  return *this;

}

