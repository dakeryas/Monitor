#ifndef LINEARISED_HISTOGRAM_H
#define LINEARISED_HISTOGRAM_H

#include <vector>
#include <set>
#include "Histogram.hpp"

namespace Converter{
  
  namespace{
    
    template <class T, class K>
    class LinearisedHistogram{
      
      std::vector<std::vector<T>> axes;
      std::vector<K> values;
      K defaultValue;
      void prepare(const Histogram<T,K>& histogram);
      
    public:
      LinearisedHistogram() = default;
      LinearisedHistogram(const Histogram<T,K>& histogram);
      const std::vector<std::vector<T>>& getAxes() const;
      const std::vector<T>& getAxis(unsigned k) const;
      const T* getAxisData(unsigned k) const;
      unsigned getNumberOfAxes() const;
      unsigned getNumberOfBins(unsigned axisNumber) const;
      const std::vector<K>& getValues() const;
      const K& getValue(unsigned k) const;
      unsigned getNumberOfValues() const;
      void linearise(const Histogram<T,K>& histogram);
      
    };
    
    template <class T, class K>
    std::ostream& operator<<(std::ostream& output, const LinearisedHistogram<T,K>& linHist){
      
      for(unsigned k = 0; k < linHist.getNumberOfValues(); ++k){
	
	for(unsigned axisNumber = 0; axisNumber < linHist.getNumberOfAxes() - 1; ++axisNumber)
	  output<<linHist.getAxis(axisNumber).at(k)<<" x ";
	
	output<<linHist.getAxis(linHist.getNumberOfAxes() - 1).at(k);
	
	output<<std::setw(6)<<std::left<<" "<<"-->"<<std::setw(6)<<std::left<<" "<<std::setw(9)<<std::left<<linHist.getValue(k)<<"\n";
	
	
      }
      return output;
      
    }
    
    template <class T, class K>
    void LinearisedHistogram<T,K>::prepare(const Histogram<T,K>& histogram){
      
      axes.clear();
      values.clear();
      std::vector<std::set<T>> axesSets(histogram.getDimension());
      
      for(const auto& pair : histogram){
      
	for(unsigned k = 0; k < axesSets.size(); ++k){
	  
	  axesSets[k].insert(pair.first.getEdge(k).getLowEdge());//instert all edges not to miss the first or the last bin edge of the row
	  axesSets[k].insert(pair.first.getEdge(k).getUpEdge());
	  
	}
      
      values.emplace_back(pair.second);
      
      }

      for(const auto& axisSet : axesSets) axes.emplace_back(axisSet.begin(), axisSet.end());
      
    }
    
    template <class T, class K>
    LinearisedHistogram<T,K>::LinearisedHistogram(const Histogram<T,K>& histogram){
	
      prepare(histogram);
      
    }

    template <class T, class K>
    const std::vector<std::vector<T>>& LinearisedHistogram<T,K>::getAxes() const{

      return axes;
      
    }
    
    template <class T, class K>
    const std::vector<T>& LinearisedHistogram<T,K>::getAxis(unsigned k) const{
      
      return axes.at(k);
      
    }
    
    template <class T, class K>
    const T* LinearisedHistogram<T,K>::getAxisData(unsigned k) const{
      
      if(k < axes.size()) return axes.at(k).data();
      else return nullptr;
      
    }
    
    template <class T, class K>
    unsigned LinearisedHistogram<T,K>::getNumberOfAxes() const{
      
      return axes.size();
      
    }
    
    template <class T, class K>
    unsigned LinearisedHistogram<T,K>::getNumberOfBins(unsigned k) const{
      
      if(k < axes.size()) return axes.at(k).size() -1;
      else return 0;
      
    }
    
    template <class T, class K>
    const std::vector<K>& LinearisedHistogram<T,K>::getValues() const{

      return values;
      
    }
    
    template <class T, class K>
    const K& LinearisedHistogram<T,K>::getValue(unsigned k) const{

      try{

	return values.at(k);
	
      }
      catch(std::out_of_range& e){//if there are no entries in the original Histogram, it may mean that the counts where 'zero'
	
	std::cout<<"Error: No "<<k<<" value: returning default K{}"<<std::endl;
	return defaultValue;
	
      }
      
    }
    
    template <class T, class K>
    unsigned LinearisedHistogram<T,K>::getNumberOfValues() const{
      
      return values.size();
      
    }

    template <class T, class K>
    void LinearisedHistogram<T,K>::linearise(const Histogram<T,K>& histogram){
	
      prepare(histogram);
      
    }
    
  }
  
}

#endif