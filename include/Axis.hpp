#ifndef AXIS_H
#define AXIS_H

#include "Segement.hpp"

template <class T>
class Axis: public Segment<T>{
  
  unsigned numberOfDivisions;
public:
  Axis(unsigned numberOfDivisions, const Segment<T>& );
  Axis(unsigned numberOfDivisions, const T& lowEdge, const T& upEdge);
  unsigned getNumberOfDivisions() const;
  T getSpacing() const;
  
};

template <class T>
Axis<T>::Axis(unsigned numberOfDivisions, const Segment<T>& segment):Segment<T>(segment),numberOfDivisions(numberOfDivisions){

}

template <class T>
Axis<T>::Axis(unsigned numberOfDivisions, const T& lowEdge, const T& upEdge):Axis(numberOfDivisions, Segment<T>(lowEdge, upEdge)){

}

template <class T>
unsigned Axis<T>::getNumberOfDivisions() const{

  return numberOfDivisions;
  
}

template <class T>
T Axis<T>::getSpacing() const{
  
  try{
    
    return (*this)/numberOfDivisions;
    
  }
  catch(std::invalid_argument& e){
    
    std::cout<<e.what()<<std::endl;
    return T{};
    
  }

}

#endif