#ifndef BINNER_H
#define BINNER_H

#include <vector>
#include  "Bin.hpp"

namespace Binner{

  template <class T>
  std::vector<Bin<T>> createBinning(unsigned numberOfBins, T firstEdge, T lastEdge){
    
    std::vector<Bin<T>> bins;
    
    T binWidth = (lastEdge - firstEdge)/numberOfBins;
    for(unsigned k = 0; k < numberOfBins; ++k) bins.emplace_back(firstEdge + k * binWidth, firstEdge + (k+1)*binWidth);
    
    return bins;
    
  }
  
}

#endif