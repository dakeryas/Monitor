#include "Constants.hpp"

namespace constants{
  
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
  
  double oscillation(double neutrinoEnergy, double distance, double th13, double delta31){//energy in MeV and distance in m
    
    
    return 1 - std::pow(std::sin(2 * th13) * std::sin(1.27 * delta31 * distance / neutrinoEnergy), 2);
    
  }
  
}
