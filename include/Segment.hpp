#ifndef SEGMENT_H
#define SEGMENT_H

#include <exception>
#include "Point.hpp"

template <class T>
class Segment{

  T lowEdge;
  T upEdge;
  
public:
  Segment() = default;
  Segment(const T& lowEdge, const T& upEdge);
  bool operator<(const Segment<T>& other) const;
  template <class K>
  T operator/=(const K& divider) const;
  bool contains(const T& value) const;//excludes upEdge
  void setEdges(const T& lowEdge, const T& upEdge);
  T getCenter() const;
  T getLowEdge() const;
  T getUpEdge() const;
  T getWidth() const;
  Segment<T>& shift(const T& value);
  
};

template <class T>
std::ostream& operator<<(std::ostream& output, const Segment<T>& segment){
  
  output<<"["<<std::setw(5)<<std::internal<<segment.getLowEdge()<<", "<<std::setw(5)<<std::internal<<segment.getUpEdge()<<"]";
  return output;
  
}

template <class T, class K>
T operator/(Segment<T> segment, const K& divider){
  
  return segment /= divider;
  
}

template <class T>
Segment<T> shift(Segment<T> segment, const T& value){//returns a segment shifted from 'value'
  
  segment.shift(value);
  return segment;
  
}


template <class T>
Segment<T>::Segment(const T& lowEdge, const T& upEdge):lowEdge(lowEdge),upEdge(upEdge){
  
}

template <class T>
bool Segment<T>::operator<(const Segment<T>& other) const{
  
  return getCenter() < other.getCenter();

}

template <class T>
template <class K>
T Segment<T>::operator/=(const K& divider) const{
  
  if(divider != T{}) return (upEdge - lowEdge)/divider;
  else throw std::invalid_argument("Trying to divide by '0' (or default constructor)");

}

template <class T>
bool Segment<T>::contains(const T& value) const{
  
  if(value >= lowEdge && value < upEdge) return true;
  else return false;

}

template <class T>
void Segment<T>::setEdges(const T& lowEdge, const T& upEdge){
  
  this->lowEdge = lowEdge;
  this->upEdge = upEdge;

}

template <class T>
T Segment<T>::getCenter() const{
  
  return (upEdge + lowEdge)*0.5;

}

template <class T>
T Segment<T>::getLowEdge() const{
  
  return lowEdge;

}

template <class T>
T Segment<T>::getUpEdge() const{
  
  return upEdge;

}

template <class T>
T Segment<T>::getWidth() const{
  
 return upEdge - lowEdge;

}

template <class T>
Segment<T>& Segment<T>::shift(const T& value){
  
  lowEdge += value;
  upEdge += value;
  
  return *this;

}

#endif