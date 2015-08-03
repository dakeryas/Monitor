#ifndef BIN_H
#define BIN_H

#include <algorithm>
#include "Segment.hpp"
#include "Tracer.hpp"

template <class T>
class Bin{

  std::vector<Segment<T>> edges;
  
public:
  Bin() = default;
  template <class Iterator>
  Bin(Iterator firstEdge, Iterator lastEdge);
  Bin(std::initializer_list<Segment<T>> edges);
  Bin(const T& lowEdge, const T& upEdge);//mainly aimed for 1-D bins that are basically one Segment
  bool operator<(const Bin<T>& other) const;
  unsigned getDimension() const;
  Segment<T> getEdge(unsigned k) const;
  Point<T> getCenter() const;
  void setDimension(unsigned k);//keeps the old edges
  void clear();//strips the bin of all its edges
  bool contains(const Point<T>& point) const;
  template <class... Args>
  void emplaceEdge(unsigned k, Args&&... args);
  template <class... Args>
  void emplaceBackEdge(Args&&... args);
  void setEdge(unsigned k, const Segment<T>& edge);
  void setEdge(unsigned k, const T& lowEdge, const T& upEdge);
  Bin<T>& shift(const Point<T>& shift);
  Bin<T>& compact(unsigned dimensionToRemove);
  Bin<T>& compact(std::vector<unsigned> dimensionsToRemove);
  
};

template <class T>
std::ostream& operator<<(std::ostream& output, const Bin<T>& bin){
  
  for(unsigned k = 0; k < bin.getDimension() - 1; ++k) output<<bin.getEdge(k)<<" x ";
  output<<bin.getEdge(bin.getDimension() -1);
  return output;
  
}

template <class T>
Bin<T> shift(Bin<T> bin, const Point<T>& shift){
  
  bin.shift(shift);
  return bin;
  
}

template <class T>
Bin<T> compact(Bin<T> bin, unsigned dimensionToRemove){
  
  return compact(bin, {dimensionToRemove});
  
}

template <class T>
Bin<T> compact(Bin<T> bin, std::vector<unsigned> dimensionsToRemove){
  
  bin.compact(dimensionsToRemove);
  return bin;
  
}

template <class T>
template <class Iterator>
Bin<T>::Bin(Iterator firstEdge, Iterator lastEdge):edges(firstEdge, lastEdge){
  
}

template <class T>
Bin<T>::Bin(std::initializer_list< Segment< T > > edges):Bin(edges.begin(),edges.end()){
  
}

template <class T>
Bin<T>::Bin(const T& lowEdge, const T& upEdge):edges({Segment<T>(lowEdge, upEdge)}){
  
}

template <class T>
bool Bin<T>::operator<(const Bin<T>& other) const{
  
  for(auto it = std::make_pair(edges.begin(), other.edges.begin()); it.first != edges.end() && it.second != other.edges.end(); ++it.first, ++it.second){
    
    if(it.first->getCenter() < it.second->getCenter()) return true;//if x1 < x2 then Bin1 < Bin2
    else if(it.first->getCenter() > it.second->getCenter()) return false;//if x1 > x2 then Bin1 !< Bin2
    
  }
    
  return false;

}

template <class T>
unsigned Bin<T>::getDimension() const{

  return edges.size();
  
}

template <class T>
Segment< T > Bin<T>::getEdge(unsigned k) const{

  try{
    
    return edges.at(k);
    
  }
  catch(std::out_of_range& e){
    
    Tracer(Verbose::Warning)<<"Bin of dimension "<<this->getDimension()<<" has no "<<k<<" edge => Returning default edge"<<std::endl;
    return Segment<T>{};
    
  }
  
}

template <class T>
Point< T > Bin<T>::getCenter() const{

  Point<T> center{};
  for(const auto& edge : edges) center.addCoordinate(edge.getCenter());
  return center;
  
}

template <class T>
void Bin<T>::setDimension(unsigned k){
  
  edges.resize(k);

}

template <class T>
void Bin<T>::clear(){
  
  setDimension(0);

}

template <class T>
bool Bin<T>::contains(const Point<T>& point) const{
  
  for(auto it = std::make_pair(edges.begin(), point.begin()); it.first != edges.end() && it.second != point.end(); ++it.first, ++it.second)
    if(!it.first->contains(*it.second)) return false;
  
  return true;

}

template <class T>
template <class... Args>
void Bin<T>::emplaceEdge(unsigned k, Args&&... args){
  
  if(k > edges.size()) Tracer(Verbose::Warning)<<"Bin of dimension "<<this->getDimension()<<" cannot add a "<<k<<" edge => Edge not added"<<std::endl;
  else edges.emplace(edges.begin() + k, std::forward<Args>(args)...);

}

template <class T>
template <class... Args>
void Bin<T>::emplaceBackEdge(Args&&... args){
  
  emplaceEdge(edges.size(), std::forward<Args>(args)...);

}

template <class T>
void Bin<T>::setEdge(unsigned k, const Segment<T>& edge){

  try{
    
    edges.at(k) = edge;
    
  }
  catch(std::out_of_range& e){
    
    Tracer(Verbose::Warning)<<"Bin of dimension "<<this->getDimension()<<" has no "<<k<<" edge => Edge not set"<<std::endl;
    
  }
  
}

template <class T>
void Bin<T>::setEdge(unsigned k, const T& lowEdge, const T& upEdge){

  setEdge(k, Segment<T>(lowEdge, upEdge));
  
}

template <class T>
Bin<T>& Bin<T>::shift(const Point<T>& shift){
  
  if(shift.getDimension() != getDimension()) Tracer(Verbose::Warning)<<"Shifting bin with Point of wrong dimension"<<shift<<std::endl;
  
  for(auto it = std::make_pair(edges.begin(),shift.begin()); it.first != edges.end() && it.second != shift.end(); ++it.first, ++it.second)
    it.first->shift(*it.second);

  return *this;
  
}

template <class T>
Bin<T>& Bin<T>::compact(unsigned dimensionToRemove){
  
  return compact({dimensionToRemove});

}

template <class T>
Bin<T>& Bin<T>::compact(std::vector<unsigned> dimensionsToRemove){
  
  std::sort(dimensionsToRemove.begin(), dimensionsToRemove.end(), [](unsigned i, unsigned j){return i > j;});//reverse sort
  
  for(const auto& dimensionToRemove : dimensionsToRemove)
    if(dimensionToRemove < edges.size()) edges.erase(edges.begin() + dimensionToRemove);
  
  return *this;

}

#endif