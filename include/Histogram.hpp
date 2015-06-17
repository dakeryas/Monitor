#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include <map>
#include "Bin.hpp"

template <class T, class K>
class Histogram{

  std::map<Bin<T>, K> countMap;//map to store the counts for Bin<T>
  
public:
  Histogram() = default;
  template <class Iterator>
  Histogram(Iterator firstBin, Iterator lastBin);
  Histogram<T,K>& operator+=(const Histogram<T,K>& other);
  Histogram<T,K>& operator*=(const K& factor);
  Histogram<T,K>& operator/=(const K& factor);
  Histogram<T,K>& normalise();
  typename std::map<Bin<T>,K>::const_iterator begin() const;
  typename std::map<Bin<T>,K>::const_iterator end() const;
  typename std::map<Bin<T>,K>::iterator begin();
  typename std::map<Bin<T>,K>::iterator end();
  K getCount(const T& value) const;
  K getCount(const Bin<T>& bin) const;
  K getTotalCounts() const;
  const std::map<Bin<T>, K>& getCountMap() const;
  void addChannel(const Bin<T>& bin);
  template <class Iterator>
  void addChannels(Iterator begin, Iterator end);//copy channels pointed to from begin to end
  void addCount(const T& value);
  void setCount(const Bin<T>& bin, const K& count);
  
  
};

template <class T, class K>
std::ostream& operator<<(std::ostream& output, const Histogram<T,K>& histogram){
  
  for(const auto& pair : histogram) output<<pair.first<<std::setw(6)<<std::left<<" "<<"-->"<<std::setw(6)<<std::left<<" "<<std::setw(9)<<std::left<<pair.second<<"\n";
  return output;
  
}

template <class T, class K>
Histogram<T,K> operator+(Histogram<T,K> histogram1, const Histogram<T,K>& histogram2){
  
  return histogram1 += histogram2;
  
}

template <class T, class K>
Histogram<T,K> operator*(Histogram<T,K> histogram, const K& factor){
  
  return histogram *= factor;
  
}

template <class T, class K>
Histogram<T,K> operator*(const K& factor, Histogram<T,K> histogram){
  
  return histogram * factor;
  
}

template <class T, class K>
Histogram<T,K> operator/(Histogram<T,K> histogram, const K& factor){
  
  return histogram /= factor;
  
}

template <class T, class K>
template <class Iterator>
Histogram<T,K>::Histogram(Iterator firstBin, Iterator lastBin){
  
  for(auto it = firstBin; it != lastBin; ++it) countMap[*it] = 0;
  
}

template <class T, class K>
Histogram<T,K>& Histogram<T,K>::operator+=(const Histogram<T,K>& other){

  for(auto& pair : other.countMap) countMap[pair.first] += pair.second;
  return *this;
  
}

template <class T, class K>
Histogram<T,K>& Histogram<T,K>::operator*=(const K& factor){

  for(auto& pair : countMap) pair.second *= factor;
  return *this;
  
}

template <class T, class K>
Histogram<T,K>& Histogram<T,K>::operator/=(const K& factor){

  return *this *= 1/factor;
  
}

template <class T, class K>
Histogram<T,K>& Histogram<T,K>::normalise(){

  return *this /= getTotalCounts();
  
}

template <class T, class K>
typename std::map<Bin<T>,K>::const_iterator Histogram<T,K>::begin() const{

  return countMap.begin();
  
}

template <class T, class K>
typename std::map<Bin<T>,K>::const_iterator Histogram<T,K>::end() const{

  return countMap.end();
  
}

template <class T, class K>
typename std::map<Bin<T>,K>::iterator Histogram<T,K>::begin(){

  return countMap.begin();
  
}

template <class T, class K>
typename std::map<Bin<T>,K>::iterator Histogram<T,K>::end(){

  return countMap.end();
  
}

template <class T, class K>
K Histogram<T,K>::getCount(const T& value) const{
  
  for(auto& pair : countMap) if(pair.first.contains(value)) return pair.second;
  
}

template <class T, class K>
K Histogram<T,K>::getCount(const Bin<T>& bin) const{
  
  return countMap.at(bin);
  
}

template <class T, class K>
K Histogram<T,K>::getTotalCounts() const{

  K totalCounts = 0;
  for(const auto& pair : countMap) totalCounts += pair.second;
  return totalCounts;
  
}

template <class T, class K>
void Histogram<T,K>::addChannel(const Bin<T>& bin){
  
  countMap.emplace(bin, K{});//default construct K

}

template <class T, class K>
template <class Iterator>
void Histogram<T,K>::addChannels(Iterator begin, Iterator end){

  for(auto it = begin; it != end; ++it) addChannel(*it);
  
}

template <class T, class K>
void Histogram<T,K>::addCount(const T& value){

  for(auto& pair : countMap) if(pair.first.contains(value)) pair.second += 1; 
  
}

template <class T, class K>
void Histogram<T,K>::setCount(const Bin<T>& bin, const K& count){

  countMap[bin] = count;
  
}


#endif