#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include <map>
#include "Bin.hpp"

template <class T>
class Histogram{

  std::map<Bin<T>, unsigned> countMap;//map to store the counts for Bin<T>
  
public:
  template <class Iterator>
  Histogram(Iterator firstBin, Iterator lastBin);
  Histogram<T>& operator+=(const Histogram<T>& other);
  void addCount(const T& value);
  unsigned getCount(Bin<T> bin) const;
  unsigned getTotalCounts() const;
  const std::map<Bin<T>, unsigned>& getCountMap() const;
  
};

template <class T>
std::ostream& operator<<(std::ostream& output, const Histogram<T>& histogram){
  
  for(const auto& pair : histogram.getCountMap()) output<<pair.first<<std::setw(6)<<std::left<<" "<<"-->"<<std::setw(6)<<std::left<<" "<<std::setw(9)<<std::left<<pair.second<<"\n";
  return output;
  
}

template <class T>
Histogram<T> operator+(Histogram<T> histogram1, const Histogram<T>& histogram2){
  
  return histogram1 += histogram2;
  
}

template <class T>
template <class Iterator>
Histogram<T>::Histogram(Iterator firstBin, Iterator lastBin){
  
  for(auto it = firstBin; it != lastBin; ++it) countMap[*it] = 0;
  
}

template <class T>
Histogram<T>& Histogram<T>::operator+=(const Histogram<T>& other){

  for(auto& pair : other.countMap) countMap[pair.first] += pair.second;
  return *this;
  
}

template <class T>
void Histogram<T>::addCount(const T& value){

  for(auto& pair : countMap) if(pair.first.contains(value)) pair.second += 1; 
  
}

template <class T>
unsigned Histogram<T>::getCount(Bin<T> bin) const{
  
  return countMap.at(bin);
  
}

template <class T>
unsigned Histogram<T>::getTotalCounts() const{

  unsigned totalCounts = 0;
  for(const auto& pair : countMap) totalCounts += pair.second;
  return totalCounts;
  
}

template <class T>
const std::map<Bin<T>, unsigned>& Histogram<T>::getCountMap() const{
  
  return countMap;

}

#endif