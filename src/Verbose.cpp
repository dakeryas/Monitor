#include "Verbose.hpp"

std::map <Verbose, std::string> verboseMap = {{Verbose::Quiet, "Info"}, {Verbose::Error, "Error"}, {Verbose::Warning, "Warning"}, {Verbose::Debug, "Debug"}};

std::ostream& operator<<(std::ostream& output, Verbose verbose){
  
  output<<verboseMap[verbose];
  return output;

}

std::istream& operator>>(std::istream& in, Verbose& verbose){
  
  std::string token;
  in >> token;
  
  if (token == "Quiet") verbose = Verbose::Quiet;
  else if (token == "Error") verbose = Verbose::Error;
  else if (token == "Warning") verbose = Verbose::Warning;
  else if (token == "Debug") verbose = Verbose::Debug;
  
  return in;
  
}