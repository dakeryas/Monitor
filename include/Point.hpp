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
  Point() = default;
  template <class Iterator>
  Point(Iterator firstCoordinate, Iterator lastCoordinate);
  Point(std::initializer_list<T> coordinates);
  typename std::vector<T>::const_iterator begin() const;
  typename std::vector<T>::const_iterator end() const;
  typename std::vector<T>::iterator begin();
  typename std::vector<T>::iterator end();
  const T& getCoordinate(unsigned k) const;
  unsigned getDimension() const;
  void setCoordinate(unsigned k, const T& coordinate);
  void addCoordinate(const T& coordinate);
  
};

template <class T>
std::ostream& operator<<(std::ostream& output, const Point<T>& point){
  
  output<<"(";
  
  if(point.getDimension() != 0){
    
    for(auto it = point.begin(); it != point.end() - 1; ++it) output<<std::setw(4)<<std::internal<<*it<<", ";
    output<<(*(point.end() -1));
    
  }
  
  output<<")";
  return output;
  
}

template <class T>
template <class Iterator>
Point<T>::Point(Iterator firstCoordinate, Iterator lastCoordinate):coordinates(firstCoordinate, lastCoordinate){

}

template <class T>
Point<T>::Point(std::initializer_list<T> coordinates):Point(coordinates.begin(), coordinates.end()){

}

template <class T>
typename std::vector<T>::const_iterator Point<T>::begin() const{
  
  return coordinates.begin();

}

template <class T>
typename std::vector<T>::const_iterator Point<T>::end() const{
  
  return coordinates.end();

}

template <class T>
typename std::vector<T>::iterator Point<T>::begin(){
  
  return coordinates.begin();

}

template <class T>
typename std::vector<T>::iterator Point<T>::end(){
  
  return coordinates.end();

}

template <class T>
const T& Point<T>::getCoordinate(unsigned k) const{

  return coordinates.at(k);
  
}

template <class T>
unsigned Point<T>::getDimension() const{
  
  return coordinates.size();

}

template <class T>
void Point<T>::setCoordinate(unsigned k, const T& coordinate){
  
  coordinates.at(k) = coordinate;

}

template <class T>
void Point<T>::addCoordinate(const T& coordinate){
  
  coordinates.emplace_back(coordinate);

}

#endif
