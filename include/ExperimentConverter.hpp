#ifndef EXPERIMENTCONVERTER_H
#define EXPERIMENTCONVERTER_H

#include <vector>
#include "TGraphErrors.h"
#include "Experiment.hpp"

namespace ExperimentConverter{

  template <class T>
  TGraphErrors TGraph(const Experiment<T>& experiment){
    
    std::vector<T> binCenters, binWidths, values, valueErrors;
    for(const auto& pair : experiment.getRunMap()){
      
      binCenters.emplace_back(pair.first.getBinCenter());
      binWidths.emplace_back(pair.first.getBinWidth()/2);
      values.emplace_back(pair.second.getNeutrinoRate(experiment.getDistance1(), experiment.getDistance2(), experiment.getBackgroundRate()));
      valueErrors.emplace_back(pair.second.getNeutrinoRateError(experiment.getDistance1(), experiment.getDistance2(), experiment.getBackgroundRate()));
      
    }
    
    return TGraphErrors(binCenters.size(), binCenters.data(), values.data(), binWidths.data(), valueErrors.data());
    
  } 
  
}

#endif