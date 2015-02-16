#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <cmath>

namespace constants{

  namespace distance{
    
    const double L1 = 111520;//old value (used in the simulation)
    const double L2 = 99855.9;//old value (used in the simulation)
    const double squaredProduct = pow(L1*L2, 2);
    
  }
  
  namespace efficiency{
    
    const double delayed = 0.853;
    const double decay = 0.9829;
    const double spill = 1.02;
    const double theta13 = 0.9274; 
    const double global = delayed*decay*spill*theta13;
    
  }
  
}

#endif