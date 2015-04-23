#ifndef BIN_H
#define BIN_H

#include <ostream>
#include <iomanip>

template <class T>
class Bin{

  T binLowEdge;
  T binUpEdge;
  
public:  
  Bin(T binLowEdge, T binUpEdge);
  bool contains(T value);//excludes binUpEdge
  void setEdges(T binLowEdge, T binUpEdge);
  T getBinCenter() const;
  T getBinLowEdge() const;
  T getBinUpEdge() const;
  
};

template <class T>
std::ostream& operator<<(std::ostream& output, const Bin<T>& bin){
  
  output<<"["<<std::setw(4)<<std::internal<<bin.getBinLowEdge()<<", "<<std::setw(4)<<std::internal<<bin.getBinUpEdge()<<"]";
  return output;
  
}

template <class T>
Bin<T>::Bin(T binLowEdge, T binUpEdge):binLowEdge(binLowEdge),binUpEdge(binUpEdge){
  
}

template <class T>
bool Bin<T>::contains(T value){
  
  if(value >= binLowEdge && value < binUpEdge) return true;
  else return false;

}

template <class T>
void Bin<T>::setEdges(T binLowEdge, T binUpEdge){
  
  this->binLowEdge = binLowEdge;
  this->binUpEdge = binUpEdge;

}

template <class T>
T Bin<T>::getBinCenter() const{
  
  return (binUpEdge - binLowEdge)*0.5;

}

template <class T>
T Bin<T>::getBinLowEdge() const{
  
  return binLowEdge;

}

template <class T>
T Bin<T>::getBinUpEdge() const{
  
  return binUpEdge;

}

#endif