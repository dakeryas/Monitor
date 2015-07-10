#ifndef CONVERTER_H
#define CONVERTER_H

#include "TGraphErrors.h"
#include "TH1D.h"
#include "TH2.h"
#include "TH3D.h"
#include "LinearisedHistogram.hpp"
#include "Experiment.hpp"

namespace Converter{
  
  template <class T, class K>
  std::unique_ptr<TH1> toTH1(const Histogram<T,K>& histogram){
    
    TH1* rootHistogram;
    
    auto dimension = histogram.getDimension();
    if(dimension > 0 && dimension < 4){
      
      LinearisedHistogram<T,K> linHist(histogram);
      
      switch(dimension){
	
	case 1:{
	  
	  rootHistogram = new TH1D("","", linHist.getAxis(0).size() - 1, linHist.getAxis(0).data());
	  for(unsigned i = 0; i < linHist.getAxis(0).size(); ++i) rootHistogram->SetBinContent(i, linHist.getValue(i));
	  
	}
	  
	case 2:{
	  
	  rootHistogram = new TH2D("","", linHist.getAxis(0).size() - 1, linHist.getAxis(0).data(), linHist.getAxis(1).size() - 1, linHist.getAxis(1).data());
	  for(unsigned i = 0; i < linHist.getAxis(0).size(); ++i)
	    for(unsigned j = 0; j < linHist.getAxis(1).size(); ++j)
	      rootHistogram->SetBinContent(i, j, linHist.getValue(i + j * linHist.getAxis(0).size()));
	  
	}
	
	case 3:{
	  
	  rootHistogram = new TH3D("","", linHist.getAxis(0).size() - 1, linHist.getAxis(0).data(), linHist.getAxis(1).size() - 1, linHist.getAxis(1).data(), linHist.getAxis(2).size() - 1, linHist.getAxis(2).data());
	  for(unsigned i = 0; i < linHist.getAxis(0).size(); ++i)
	    for(unsigned j = 0; j < linHist.getAxis(1).size(); ++j)
	      for(unsigned k = 0; k < linHist.getAxis(2).size(); ++k)
		rootHistogram->SetBinContent(i, j, k, linHist.getValue(i + j * linHist.getAxis(0).size() + k * linHist.getAxis(1).size()));
	  
	}
	
      }
      
    }
    else rootHistogram = nullptr;
    
    return std::unique_ptr<TH1>(rootHistogram);
    
  }
  
  template <class T, class K>
  TGraphErrors toTGraph(const Histogram<T,K>& histogram){
    
    if(histogram.getDimension() == 1){
    
      std::vector<T> binCenters, binWidths;
      std::vector<K> values, valueErrors;
      for(const auto& pair : histogram){
	
	binCenters.emplace_back(pair.first.getEdge(0).getCenter());
	binWidths.emplace_back(pair.first.getEdge(0).getWidth()/2);
	values.emplace_back(pair.second);
	valueErrors.emplace_back(0);
	
      }
      
      return TGraphErrors(binCenters.size(), binCenters.data(), values.data(), binWidths.data(), valueErrors.data());
      
    }
    else{
      
      std::cout<<"Can only convert 1D histogram to TGraph"<<std::endl;
      return TGraphErrors();
      
    }
    
  }
  
  template <class T>
  TGraphErrors toTGraph(const Experiment<T>& experiment){
    
    std::vector<T> binCenters, binWidths, values, valueErrors;
    for(const auto& pair : experiment){
      
      binCenters.emplace_back(pair.first.getEdge(0).getCenter());
      binWidths.emplace_back(pair.first.getEdge(0).getWidth()/2);
      values.emplace_back(pair.second.getNeutrinoRate(experiment.getDistance1(), experiment.getDistance2(), experiment.getBackgroundRate()));
      valueErrors.emplace_back(pair.second.getNeutrinoRateError(experiment.getDistance1(), experiment.getDistance2(), experiment.getBackgroundRate()));
      
    }
    
    return TGraphErrors(binCenters.size(), binCenters.data(), values.data(), binWidths.data(), valueErrors.data());
    
  }
  
  template <class T, class K>
  Histogram<T,K> toHistogram(const TH1D& th1d){
    
    Histogram<T,K> histogram;
    for(int k = 1; k < th1d.GetNbinsX() + 1; ++k) histogram.setCount(Bin<T>(th1d.GetBinLowEdge(k), th1d.GetBinLowEdge(k+1)), th1d.GetBinContent(k));
    return histogram;
    
  }
  
  template <class T>
  std::string toString(const T& object){
    
    std::stringstream sstream;
    sstream<<object;
    return sstream.str();
    
  }
  
}

#endif