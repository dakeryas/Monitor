#ifndef CONVERTER_H
#define CONVERTER_H

#include <vector>
#include <set>
#include "TGraphErrors.h"
#include "TH1D.h"
#include "TH2.h"
#include "TH3D.h"
#include "Experiment.hpp"
#include "Histogram.hpp"

namespace Converter{

  template <class T, class K>
  std::unique_ptr<TH1> toTH1(const Histogram<T,K>& histogram){
    
    TH1* rootHistogram;
    
    auto dimension = histogram.getDimension();
    if(dimension > 0 && dimension < 4){
      
      std::vector<std::set<T>> binEdgesSets(dimension);
      std::vector<K> values;
      for(const auto& pair : histogram){
	
	for(unsigned k = 0; k < dimension; ++k){
	  
	  binEdgesSets[k].insert(pair.first.getEdge(k).getLowEdge());//instert all edges not to miss the first or the last bin edge of the row
	  binEdgesSets[k].insert(pair.first.getEdge(k).getUpEdge());
	  
	}
	
	values.emplace_back(pair.second);
	
      }
      
      switch(dimension){
	
	case 1:{
	  
	  std::vector<T> binEdgesX(binEdgesSets[0].begin(), binEdgesSets[0].end());
	  rootHistogram = new TH1D("","", binEdgesX.size() + 1, binEdgesX.data());
	  for(unsigned i = 0; i < binEdgesX.size(); ++i) rootHistogram->SetBinContent(i, values.at(i));
	  
	}
	  
	case 2:{
	  
	  std::vector<T> binEdgesX(binEdgesSets[0].begin(), binEdgesSets[0].end());
	  std::vector<T> binEdgesY(binEdgesSets[1].begin(), binEdgesSets[1].end());
	  rootHistogram = new TH2D("","", binEdgesX.size() + 1, binEdgesX.data(), binEdgesY.size() + 1, binEdgesY.data());
	  for(unsigned i = 0; i < binEdgesX.size(); ++i)
	    for(unsigned j = 0; j < binEdgesY.size(); ++j)
	      rootHistogram->SetBinContent(i, j, values.at(i + j * binEdgesX.size()));
	  
	}
	
	case 3:{
	  
	  std::vector<T> binEdgesX(binEdgesSets[0].begin(), binEdgesSets[0].end());
	  std::vector<T> binEdgesY(binEdgesSets[1].begin(), binEdgesSets[1].end());
	  std::vector<T> binEdgesZ(binEdgesSets[2].begin(), binEdgesSets[2].end());
	  rootHistogram = new TH3D("","", binEdgesX.size() + 1, binEdgesX.data(), binEdgesY.size() + 1, binEdgesY.data(), binEdgesZ.size() + 1, binEdgesZ.data());
	  for(unsigned i = 0; i < binEdgesX.size(); ++i)
	    for(unsigned j = 0; j < binEdgesY.size(); ++j)
	      for(unsigned k = 0; k < binEdgesZ.size(); ++k)
		rootHistogram->SetBinContent(i, j, values.at(i + j * binEdgesX.size() + k * binEdgesY.size()));
	  
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