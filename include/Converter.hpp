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
	
      if(dimension == 1){

	rootHistogram = new TH1D("","", linHist.getNumberOfBins(0), linHist.getAxisData(0));
	for(unsigned i = 0; i < linHist.getNumberOfBins(0); ++i) rootHistogram->SetBinContent(i+1, linHist.getValue(i));
	
      }
	  
      else if(dimension == 2){
 
	rootHistogram = new TH2D("","", linHist.getNumberOfBins(0), linHist.getAxisData(0), linHist.getNumberOfBins(1), linHist.getAxisData(1));
	for(unsigned i = 0; i < linHist.getNumberOfBins(0); ++i)
	  for(unsigned j = 0; j < linHist.getNumberOfBins(1); ++j)
	    rootHistogram->SetBinContent(i+1, j+1, linHist.getValue(i + j * linHist.getNumberOfBins(0)));

      }
	
      else if(dimension == 3){

	rootHistogram = new TH3D("","", linHist.getNumberOfBins(0), linHist.getAxisData(0), linHist.getNumberOfBins(1), linHist.getAxisData(1), linHist.getNumberOfBins(2), linHist.getAxisData(2));
	for(unsigned i = 0; i < linHist.getNumberOfBins(0); ++i)
	  for(unsigned j = 0; j < linHist.getNumberOfBins(1); ++j)
	    for(unsigned k = 0; k < linHist.getNumberOfBins(2); ++k)
	      rootHistogram->SetBinContent(i+1, j+1, k +1, linHist.getValue(i + j * linHist.getNumberOfBins(0) + k * linHist.getNumberOfBins(1)));
	
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