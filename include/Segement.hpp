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
  T operator/=(const T& divider) const;
  bool contains(const T& value) const;//excludes upEdge
  void setEdges(const T& lowEdge, const T& upEdge);
  T getCenter() const;
  T getLowEdge() const;
  T getUpEdge() const;
  T getWidth() const;
  
};

template <class T>
std::ostream& operator<<(std::ostream& output, const Segment<T>& segment){
  
  output<<"["<<std::setw(4)<<std::internal<<segment.getLowEdge()<<", "<<std::setw(4)<<std::internal<<segment.getUpEdge()<<"]";
  return output;
  
}

template <class T>
T operator/(Segment<T> segment, const T& divider){
  
  return segment /= divider;
  
}

template <class T>
Segment<T>::Segment(const T& lowEdge, const T& upEdge):lowEdge(lowEdge),upEdge(upEdge){
  
}

template <class T>
bool Segment<T>::operator<(const Segment<T>& other) const{
  
  return getCenter() < other.getCenter();

}

template <class T>
T Segment<T>::operator/=(const T& divider) const{
  
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

#endif