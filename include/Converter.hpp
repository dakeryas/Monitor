#ifndef CONVERTER_H
#define CONVERTER_H

#include <vector>
#include "TGraphErrors.h"
#include "TH1D.h"
#include "Experiment.hpp"
#include "Histogram.hpp"

namespace Converter{

  template <class T, class K>
  TGraphErrors toTGraph(const Histogram<T,K>& histogram){
    
    std::vector<T> binCenters, binWidths;
    std::vector<K> values, valueErrors;
    for(const auto& pair : histogram){
      
      binCenters.emplace_back(pair.first.getBinCenter());
      binWidths.emplace_back(pair.first.getBinWidth()/2);
      values.emplace_back(pair.second);
      valueErrors.emplace_back(0);
      
    }
    
    return TGraphErrors(binCenters.size(), binCenters.data(), values.data(), binWidths.data(), valueErrors.data());
    
  }
  
  template <class T>
  TGraphErrors toTGraph(const Experiment<T>& experiment){
    
    std::vector<T> binCenters, binWidths, values, valueErrors;
    for(const auto& pair : experiment.getRunMap()){
      
      binCenters.emplace_back(pair.first.getBinCenter());
      binWidths.emplace_back(pair.first.getBinWidth()/2);
      values.emplace_back(pair.second.getNeutrinoRate(experiment.getDistance1(), experiment.getDistance2(), experiment.getBackgroundRate()));
      valueErrors.emplace_back(pair.second.getNeutrinoRateError(experiment.getDistance1(), experiment.getDistance2(), experiment.getBackgroundRate()));
      
    }
    
    return TGraphErrors(binCenters.size(), binCenters.data(), values.data(), binWidths.data(), valueErrors.data());
    
  }
  
  Histogram<double,double> toHistogram(const TH1D& th1d){
    
    Histogram<double,double> histogram;
    for(int k = 1; k < th1d.GetNbinsX() + 1; ++k) histogram.setCount(Bin<double>(th1d.GetBinLowEdge(k), th1d.GetBinLowEdge(k+1)), th1d.GetBinContent(k));
    return histogram;
    
  }
  
  
  
}

#endif