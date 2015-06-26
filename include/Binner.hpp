#ifndef BINNER_H
#define BINNER_H

#include <vector>
#include "Axis.hpp"
#include  "Bin.hpp"

template <class T>
class Binner{
  std::vector<Axis<T>> axes;
  std::vector<Bin<T>> bins;
  std::vector<unsigned> currentIndices;
  std::vector<unsigned >multiplier;
  void prepare();
  unsigned getGlobalIndex() const;
  void fillBins(unsigned k);
public:
  Binner() = default;
  template <class Iterator>
  Binner(Iterator beginAxis, Iterator endAxis);
  Binner(std::initializer_list<Axis<T>> axes);
  Binner(unsigned numberOfDivisions, const T& lowEdge, const T& upEdge);
  const std::vector<Bin<T>>& getBins() const;
  const std::vector<Bin<T>>& generateBinning();
  template <class Iterator>
  void setAxes(Iterator beginAxis, Iterator endAxis);
  void setAxes(std::initializer_list<Axis<T>> axes);
  void setAxes(unsigned numberOfDivisions, const T& lowEdge, const T& upEdge);
  
};

template <class T>
std::ostream& operator<<(std::ostream& output, const Binner<T>& binner){
  
  for(const auto& bin : binner.getBins()) output<<bin<<"\n";
  return output;
  
}

template <class T>
void Binner<T>::prepare(){

  unsigned totalNumberOfBins = 1;
  for(const auto& axis : axes) totalNumberOfBins *= axis.getNumberOfDivisions();
  bins.resize(totalNumberOfBins);
  
  for(unsigned k = 0; k < axes.size() - 1; ++k) multiplier.at(k+1) *= axes.at(k).getNumberOfDivisions() * multiplier.at(k);//the first multiplier should be 1
  
}

template <class T>
unsigned Binner<T>::getGlobalIndex() const{
  
  unsigned globalIndex{};
  for(unsigned k = 0; k < axes.size(); ++k) globalIndex += currentIndices.at(k) * multiplier.at(k);
  return globalIndex;
  
}

template <class T>
void Binner<T>::fillBins(unsigned rank){

  if(rank == axes.size() - 1){//stop condition
    
    for(unsigned k = 0; k < axes.at(rank).getNumberOfDivisions(); ++k){
      
      currentIndices.at(rank) = k;//update indices
      
      for(unsigned i = 0; i < axes.size(); ++i)//build all edges for current bin
	bins.at(getGlobalIndex()).emplaceEdge(axes.at(i).getLowEdge() + currentIndices.at(i)*axes.at(i).getSpacing(), axes.at(i).getLowEdge() + (currentIndices.at(i)+1)*axes.at(i).getSpacing());
      
    }
    
  }
  else if(rank < axes.size() - 1){
    
    for(unsigned k = 0; k < axes.at(rank).getNumberOfDivisions(); ++k){
      
      currentIndices.at(rank) = k;//update indices
      fillBins(rank + 1);//recursive call
      
    }
    
  }
    
}

template <class T>
template <class Iterator>
Binner<T>::Binner(Iterator beginAxis, Iterator endAxis):axes(beginAxis,endAxis),currentIndices(axes.size(), 0), multiplier(axes.size(), 1){
  
  prepare();

}

template <class T>
Binner<T>::Binner(std::initializer_list<Axis<T>> axes):Binner(axes.begin(),axes.end()){
  
}

template <class T>
Binner<T>::Binner(unsigned numberOfDivisions, const T& lowEdge, const T& upEdge):Binner({Axis<T>(numberOfDivisions, lowEdge, upEdge)}){

}

template <class T>
const std::vector<Bin<T>>& Binner<T>::getBins() const{
  
  return bins;

}

template <class T>
const std::vector<Bin<T>>& Binner<T>::generateBinning(){
  
  fillBins(0);//start at rank 0
  return getBins();
  
}

template <class T>
template <class Iterator>
void Binner<T>::setAxes(Iterator beginAxis, Iterator endAxis){
  
  axes.assign(beginAxis,endAxis);
  currentIndices.assign(axes.size(), 0);
  multiplier.assign(axes.size(), 1);
  prepare();

}

template <class T>
void Binner<T>::setAxes(std::initializer_list< Axis< T > > axes){

  setAxes(axes.begin(), axes.end());
  
}

template <class T>
void Binner<T>::setAxes(unsigned numberOfDivisions, const T& lowEdge, const T& upEdge){
  
  setAxes({Axis<T>(numberOfDivisions, lowEdge, upEdge)});

}

#endif