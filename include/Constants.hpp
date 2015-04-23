#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <cmath>

namespace constants{

  namespace distance{
    
    const double L1 = 111520;//old value (used in the simulation)
    const double L2 = 99855.9;//old value (used in the simulation)
    const double squaredProduct = std::pow(L1*L2, 2);
    const double product = L1*L2;
    
  }
  
  namespace efficiency{
    
    const double delayed = 0.853;
    const double decay = 0.9829;
    const double spill = 1.02;
    const double theta13 = 0.9274; 
    const double global = delayed*decay*spill*theta13;
    
  }
  
  namespace backgroundRate{
    
    const double cosmogenic = 0.97;
    const double fastNeutrons = 0.604;
    const double accidental = 0.07;
    const double total = cosmogenic + fastNeutrons + accidental;
    
  }
  
  namespace time{
    
    const double day = 86400;
    
  }
  
  namespace energy{
    
    const double MWPerMGW = 1e3; //number of MegaWatt per GigaWatt 
  
}

#endif