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

    TH1* rootHistogram{nullptr};
    
    auto dimension = histogram.getDimension();
    if(dimension > 0 && dimension < 4){
      
      LinearisedHistogram<T,K> linHist(histogram);
	
      if(dimension == 1){

	rootHistogram = new TH1D("","", linHist.getNumberOfBins(0), linHist.getAxisData(0));
	for(unsigned i = 0; i < linHist.getNumberOfBins(0); ++i)
	  rootHistogram->SetBinContent(i+1, linHist.getValue(i));
	
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
    else Tracer(Verbose::Error)<<dimension<<"-D histograms cannot be graphically represented => Returning nullptr"<<std::endl;

    return std::unique_ptr<TH1>(rootHistogram);
    
  }
  
  template <class T, class K>
  std::unique_ptr<TH1> toTH1(const Histogram<T,Scalar<K>>& histogram){

    TH1* rootHistogram{nullptr};
    
    auto dimension = histogram.getDimension();
    if(dimension > 0 && dimension < 4){
      
      LinearisedHistogram<T,Scalar<K>> linHist(histogram);
	
      if(dimension == 1){

	rootHistogram = new TH1D("","", linHist.getNumberOfBins(0), linHist.getAxisData(0));
	for(unsigned i = 0; i < linHist.getNumberOfBins(0); ++i){
	  
	  rootHistogram->SetBinContent(i+1, linHist.getValue(i).getValue());
	  rootHistogram->SetBinError(i+1, linHist.getValue(i).getError());
	  
	}
	
      }
	  
      else if(dimension == 2){

	rootHistogram = new TH2D("","", linHist.getNumberOfBins(0), linHist.getAxisData(0), linHist.getNumberOfBins(1), linHist.getAxisData(1));
	for(unsigned i = 0; i < linHist.getNumberOfBins(0); ++i)
	  for(unsigned j = 0; j < linHist.getNumberOfBins(1); ++j){

	    rootHistogram->SetBinContent(i+1, j+1, linHist.getValue(i + j * linHist.getNumberOfBins(0)).getValue());
	    rootHistogram->SetBinError(i+1, j+1, linHist.getValue(i + j * linHist.getNumberOfBins(0)).getError());
	    
	  }

      }
	
      else if(dimension == 3){

	rootHistogram = new TH3D("","", linHist.getNumberOfBins(0), linHist.getAxisData(0), linHist.getNumberOfBins(1), linHist.getAxisData(1), linHist.getNumberOfBins(2), linHist.getAxisData(2));
	for(unsigned i = 0; i < linHist.getNumberOfBins(0); ++i)
	  for(unsigned j = 0; j < linHist.getNumberOfBins(1); ++j)
	    for(unsigned k = 0; k < linHist.getNumberOfBins(2); ++k){
	      
	      rootHistogram->SetBinContent(i+1, j+1, k +1, linHist.getValue(i + j * linHist.getNumberOfBins(0) + k * linHist.getNumberOfBins(1)).getValue());
	      rootHistogram->SetBinError(i+1, j+1, k +1, linHist.getValue(i + j * linHist.getNumberOfBins(0) + k * linHist.getNumberOfBins(1)).getError());
	      
	    }
	
      }
	
    }
    else Tracer(Verbose::Error)<<dimension<<"-D histograms cannot be graphically represented => Returning nullptr"<<std::endl;

    return std::unique_ptr<TH1>(rootHistogram);
    
  }
  
  template <class T, class K>
  TGraphErrors toTGraph(const Histogram<T,Scalar<K>>& histogram){
    
    if(histogram.getDimension() == 1){
    
      std::vector<T> binCenters, binWidths;
      std::vector<K> values, valueErrors;
      for(const auto& pair : histogram){
	
	binCenters.emplace_back(pair.first.getEdge(0).getCenter());
	binWidths.emplace_back(pair.first.getEdge(0).getWidth()/2);
	values.emplace_back(pair.second.getValue());
	valueErrors.emplace_back(pair.second.getError());
	
      }
      
      return TGraphErrors(binCenters.size(), binCenters.data(), values.data(), binWidths.data(), valueErrors.data());
      
    }
    else{
      
      Tracer(Verbose::Error)<<"Can only convert 1D histogram to TGraph => Returning default TGraph"<<std::endl;
      return TGraphErrors();
      
    }
    
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
      
      Tracer(Verbose::Error)<<"Can only convert 1D histogram to TGraph => Returning default TGraph"<<std::endl;
      return TGraphErrors();
      
    }
    
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