#ifndef BIN_H
#define BIN_H

#include "Segment.hpp"

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
  bool contains(const Point<T>& point) const;
  void addEdge(unsigned k, const Segment<T>& edge);
  void addEdge(const Segment<T>& edge);
  void emplaceEdge(const T& lowEdge, const T& upEdge);
  Bin<T>& shift(const Point<T>& shift);
  
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
    
    std::cout<<"Error: Bin of dimension "<<this->getDimension()<<" has no "<<k<<" edge\nReturning default edge"<<std::endl;
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
bool Bin<T>::contains(const Point<T>& point) const{
  
  for(auto it = std::make_pair(edges.begin(), point.begin()); it.first != edges.end() && it.second != point.end(); ++it.first, ++it.second)
    if(!it.first->contains(*it.second)) return false;
  
  return true;

}

template <class T>
void Bin<T>::addEdge(unsigned k, const Segment<T>& segment){
  
  edges.insert(k, segment);

}

template <class T>
void Bin<T>::addEdge(const Segment<T>& segment){
  
  edges.emplace_back(segment);

}

template <class T>
void Bin<T>::emplaceEdge(const T& lowEdge, const T& upEdge){
  
  addEdge(Segment<T>(lowEdge, upEdge));

}

template <class T>
Bin<T>& Bin<T>::shift(const Point<T>& shift){
  
  if(shift.getDimension() != getDimension()) std::cout<<"Warning: shifting bin with Point of wrong dimension"<<shift<<std::endl;
  
  for(auto it = std::make_pair(edges.begin(),shift.begin()); it.first != edges.end() && it.second != shift.end(); ++it.first, ++it.second)
    it.first->shift(*it.second);

  return *this;
  
}

#endif