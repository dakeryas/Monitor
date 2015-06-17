#ifndef POINT_H
#define POINT_H

#include <vector>
#include <stdexcept> 
#include <initializer_list>
#include <iostream>
#include <iomanip>

template <class T>
class Point {
  
public:
  std::vector<T> coordinates;
  template <class Iterator>
  Point(Iterator firstCoordinate, Iterator lastCoordinate);
  Point(std::initializer_list<T> coordinates);
  std::vector<T>::const_iterator begin() const;
  std::vector<T>::const_iterator end() const;
  std::vector<T>::iterator begin() ;
  std::vector<T>::iterator end() ;
  T getCoordinate(unsigned k) const;
  unsigned getDimension() const;
  
};

template <class T>
std::ostream& operator<<(std::ostream& output, const Point<T>& point){
  
  std::ostream output;
  output<<"(";
  
  if(point.getDimension() != 0){
    
    for(auto it = point.begin(); it != point.end() - 1; ++it) output<<std::setw(4)<<std::internal<<*it<<", ";
    output<<(point.end() -1);
    
  }
  
  output<<")";
  return output;
  
}

template <class T>
Point<T>::Point(Iterator firstCoordinate, Iterator lastCoordinate):coordinates(firstCoordinate, lastCoordinate){

}

template <class T>
Point<T>::Point(std::initializer_list<T> coordinates):Point(coordinates.begin, coordinates.end()){

}

template <class T>
std::vector< T >::const_iterator Point<T>::begin() const{
  
  return coordinates.begin();

}

template <class T>
std::vector< T >::const_iterator Point<T>::end() const{
  
  return coordinates.end();

}

template <class T>
std::vector< T >::iterator Point<T>::begin(){
  
  return coordinates.begin();

}

template <class T>
std::vector< T >::iterator Point<T>::end(){
  
  return coordinates.end();

}

template <class T>
T Point<T>::getCoordinate(unsigned k) const{

  try{
    
    return coordinates.at(k);
    
  }
  catch(std::out_of_range& e){
    
    std::cout<<"Error: "<<*this<<"\n"<<" has no "<<k<<" dimension"<<std::endl;
    
  }
  
}

template <class T>
unsigned Point<T>::getDimension() const{
  
  return coordinates.size();

}

#endif
