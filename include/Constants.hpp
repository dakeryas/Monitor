#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <cmath>

namespace constants{
  
  namespace distance{
    
    const double L1 = 1115.20;//old value in meters (used in the simulation)
    const double L2 = 998.559;//old value in meters (used in the simulation)
    const double product = L1*L2;
    const double average = std::sqrt(product);
    
  }
  
  namespace mixing{
    
    const double th13 = 0.085; //DB at Moriond
    
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
    
    const double dayToSecond = 86400;
    const double secondToDay = 1/dayToSecond;
    
  }
  
  namespace energy{
    
    const double GWattToMWatt = 1e3; //number of MegaWatt per GigaWatt
    const double MWattToGWatt = 1/GWattToMWatt;
    
  }
  
  namespace mass{
    
    const double electron = 0.511;
    const double proton = 938.272;
    const double neutron = 939.5654;
    
  }
  
  namespace squaredMass{
    
    const double delta31 = 2.38 * 1e-3;//eV squared
    
  }
  
  void adaptUnits(double& runLenght, double& power1, double& power2){//convert values to days and GW
  
    runLenght *= time::secondToDay;
    power1 *= energy::MWattToGWatt;
    power2 *= energy::MWattToGWatt;
  
  }
  
  double crossSection(double neutrinoEnergy){
    
    if(neutrinoEnergy > (mass::neutron - mass::proton)){//if we pass the threshold
      
      double positronEnergy = neutrinoEnergy - mass::neutron + mass::proton;
      return positronEnergy * std::sqrt(std::pow(positronEnergy, 2) - std::pow(mass::electron, 2));
      
    }
    else return 0;//if the neutrino's energy is too low, the cross section is zero
    
  }
  
  double oscillation(double neutrinoEnergy, double distance, double th13 = mixing::th13, double delta31 = squaredMass::delta31){//energy in MeV and distance in m
    
    
    return 1 - std::pow(std::sin(2 * th13) * std::sin(1.27 * delta31 * distance / neutrinoEnergy), 2);
    
  }
  
}

#endif