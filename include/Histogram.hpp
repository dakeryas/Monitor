#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include <map>
#include "Bin.hpp"
#include "Scalar.hpp"

template <class T, class K>
class Histogram{

  std::map<Bin<T>, K> countMap;//map to store the counts for Bin<T>
  
  template <class BinType, class ValueType>
  struct HistogramTypes{};//to specialise some methods for <BinType, Scalar<ValueType>>
  
  template <class BinType, class ValueType>
  Histogram<T,K>& normalise(HistogramTypes<BinType,ValueType>);
  template <class BinType, class ValueType>
  Histogram<T,K>& normalise(HistogramTypes<BinType,Scalar<ValueType>>);
  template <class BinType, class ValueType, class NormType>
  Histogram<T,K>& scaleCountsTo(HistogramTypes<BinType,ValueType>, const NormType& newNorm);
  template <class BinType, class ValueType, class NormType>
  Histogram<T,K>& scaleCountsTo(HistogramTypes<BinType,Scalar<ValueType>>, const NormType& newNorm);
  template <class BinType, class ValueType>
  void addCount(HistogramTypes<BinType,ValueType>, const Point<T>& point);
  template <class BinType, class ValueType>
  void addCount(HistogramTypes<BinType,Scalar<ValueType>>, const Point<T>& point);
  
public:
  Histogram() = default;
  template <class Iterator>
  Histogram(Iterator firstBin, Iterator lastBin);
  Histogram<T,K> operator-();
  template <class OtherBinType, class OtherValueType>
  Histogram<T,K>& operator+=(const Histogram<OtherBinType,OtherValueType>& other);
  template <class OtherBinType, class OtherValueType>
  Histogram<T,K>& operator-=(const Histogram<OtherBinType,OtherValueType>& other);
  template <class FactorType>
  Histogram<T,K>& operator*=(const FactorType& factor);
  template <class OtherBinType, class OtherValueType>
  Histogram<T,K>& operator*=(const Histogram<OtherBinType,OtherValueType>& multiplier);
  template <class FactorType>
  Histogram<T,K>& operator/=(const FactorType& factor);
  template <class OtherBinType, class OtherValueType>
  Histogram<T,K>& operator/=(const Histogram<OtherBinType,OtherValueType>& divider);
  Histogram<T,K>& normalise();
  template<class NormType>
  Histogram<T,K>& scaleCountsTo(const NormType& newNorm);
  Histogram<T,K>& shiftChannels(const Point<T>& shift);//shift all channels with 'shift'
  Histogram<T,K>& integrateDimension(unsigned dimensionToRemove);//integrate the counts over 'dimensionToRemove'
  Histogram<T,K>& integrateDimensions(std::vector<unsigned> dimensionsToRemove);//integrate the counts over 'dimensionToRemove'
  typename std::map<Bin<T>,K>::const_iterator begin() const;
  typename std::map<Bin<T>,K>::const_iterator end() const;
  typename std::map<Bin<T>,K>::iterator begin();
  typename std::map<Bin<T>,K>::iterator end();
  K getCount(const Point<T>& point) const;
  K getCount(const Bin<T>& bin) const;
  K getTotalCounts() const;
  unsigned getDimension() const;
  unsigned getNumberOfChannels() const;
  void addChannel(const Bin<T>& bin);
  template <class Iterator>
  void addChannels(Iterator begin, Iterator end);//copy channels pointed to from begin to end
  void addCount(const Point<T>& point);
  void setCount(const Bin<T>& bin, const K& count);
  
};

template <class T, class K>
std::ostream& operator<<(std::ostream& output, const Histogram<T,K>& histogram){
  
  for(const auto& pair : histogram) output<<pair.first<<std::setw(6)<<std::left<<" "<<"-->"<<std::setw(6)<<std::left<<" "<<std::setw(9)<<std::left<<pair.second<<"\n";
  return output;
  
}

template <class T1, class K1, class T2, class K2>
Histogram<T1,K1> operator+(Histogram<T1,K1> histogram1, const Histogram<T2,K2>& histogram2){
  
  return histogram1 += histogram2;
  
}

template <class T1, class K1, class T2, class K2>
Histogram<T1,K1> operator-(Histogram<T1,K1> histogram1, const Histogram<T2,K2>& histogram2){
  
  return histogram1 -= histogram2;
  
}

template <class T, class K, class FactorType>
Histogram<T,K> operator*(Histogram<T,K> histogram, const FactorType& factor){
  
  return histogram *= factor;
  
}

template <class T, class K, class FactorType>
Histogram<T,K> operator*(const FactorType& factor, Histogram<T,K> histogram){
  
  return histogram * factor;
  
}

template <class T1, class K1, class T2, class K2>
Histogram<T1,K1> operator*(Histogram<T1,K1> histogram1, const Histogram<T2,K2>& histogram2){
  
  return histogram1 *= histogram2;
  
}

template <class T, class K, class FactorType>
Histogram<T,K> operator/(Histogram<T,K> histogram, const FactorType& factor){
  
  return histogram /= factor;
  
}

template <class T1, class K1, class T2, class K2>
Histogram<T1,K1> operator/(Histogram<T1,K1> histogram1, const Histogram<T2,K2>& histogram2){
  
  return histogram1 /= histogram2;
  
}

template <class T, class K>
template <class BinType, class ValueType>
Histogram<T,K>& Histogram<T,K>::normalise(HistogramTypes<BinType,ValueType>){
  
  ValueType totalCounts = getTotalCounts();
  if(totalCounts != ValueType{}) return *this /= totalCounts;
  else{
    
    Tracer(Verbose::Warning)<<"Histogram has no counts: already normalised!"<<std::endl;
    return *this;
    
  }

}

template <class T, class K>
template <class BinType, class ValueType>
Histogram<T,K>& Histogram<T,K>::normalise(HistogramTypes<BinType,Scalar<ValueType>>){
  
  ValueType totalCounts = getTotalCounts().getValue();//drop the "Scalar" when normalising since we don't want to double count the error on the bin contents
  if(totalCounts != ValueType{}) return *this /= totalCounts;
  else{
    
    Tracer(Verbose::Warning)<<"Histogram has no counts: already normalised!"<<std::endl;
    return *this;
    
  }

}

template <class T, class K>
template <class BinType, class ValueType, class NormType>
Histogram<T,K>& Histogram<T,K>::scaleCountsTo(HistogramTypes<BinType,ValueType>, const NormType& newNorm){
  
  ValueType totalCounts = getTotalCounts();
  if(totalCounts != K{}) return *this *= newNorm/totalCounts;
  else return *this;

}

template <class T, class K>
template <class BinType, class ValueType, class NormType>
Histogram<T,K>& Histogram<T,K>::scaleCountsTo(HistogramTypes<BinType,Scalar<ValueType>>, const NormType& newNorm){

  ValueType totalCounts = getTotalCounts().getValue();//drop the "Scalar" when normalising since we don't want to double count the error on the bin contents
  if(totalCounts != ValueType{}) return *this *= newNorm/totalCounts;
  else return *this;

}

template <class T, class K>
template <class BinType, class ValueType>
void Histogram<T,K>::addCount(HistogramTypes<BinType,ValueType>, const Point<T>& point){

  auto it = std::find_if(countMap.begin(), countMap.end(),[&](const auto& pairBin){return pairBin.first.contains(point);});
  if(it != countMap.end()) it->second += ValueType{1};
  else Tracer(Verbose::Warning)<<"No channel matches: "<<point<<" => Count not added"<<std::endl;
  
}

template <class T, class K>
template <class BinType, class ValueType>
void Histogram<T,K>::addCount(HistogramTypes<BinType,Scalar<ValueType>>, const Point<T>& point){

  auto it = std::find_if(countMap.begin(), countMap.end(),[&](const auto& pairBin){return pairBin.first.contains(point);});
  if(it != countMap.end()) it->second += Scalar<ValueType>{1, 1};//add the statistical error when dealing with Scalar<>
  else Tracer(Verbose::Warning)<<"No channel matches: "<<point<<" => Count not added"<<std::endl;
  
}

template <class T, class K>
template <class Iterator>
Histogram<T,K>::Histogram(Iterator firstBin, Iterator lastBin){
  
  for(auto it = firstBin; it != lastBin; ++it)
    if(it->getDimension() == firstBin->getDimension()) countMap[*it] = K{};
  
}

template <class T, class K>
Histogram<T,K> Histogram<T,K>::operator-(){

  Histogram<T,K> oppositeHistogram{*this};
  for(auto& pair : oppositeHistogram.countMap) pair.second = -pair.second;
  return oppositeHistogram;
  
}

template <class T, class K>
template <class OtherBinType, class OtherValueType>
Histogram<T,K>& Histogram<T,K>::operator+=(const Histogram<OtherBinType,OtherValueType>& other){

  for(auto& pair : other) countMap[pair.first] += pair.second;
  return *this;
  
}

template <class T, class K>
template <class OtherBinType, class OtherValueType>
Histogram<T,K>& Histogram<T,K>::operator-=(const Histogram<OtherBinType,OtherValueType>& other){

  for(auto& pair : other) countMap[pair.first] -= pair.second;
  return *this;
  
}

template <class T, class K>
template <class FactorType>
Histogram<T,K>& Histogram<T,K>::operator*=(const FactorType& factor){

  for(auto& pair : countMap) pair.second *= factor;
  return *this;
  
}

template <class T, class K>
template <class OtherBinType, class OtherValueType>
Histogram<T,K>& Histogram<T,K>::operator*=(const Histogram<OtherBinType,OtherValueType>& multiplier){

  for(auto itPair = std::make_pair(begin(),multiplier.begin()); itPair.first != end() && itPair.second != multiplier.end(); ++itPair.first, ++itPair.second)
    itPair.first->second *= itPair.second->second; 
  return *this;
  
}

template <class T, class K>
template <class FactorType>
Histogram<T,K>& Histogram<T,K>::operator/=(const FactorType& factor){

  if(factor != K{}) return *this *= 1/factor;
  else{
    
    Tracer(Verbose::Warning)<<"Histogram division by "<<factor<<" not allowed!"<<std::endl;
    return *this;
    
  }
  
}

template <class T, class K>
template <class OtherBinType, class OtherValueType>
Histogram<T,K>& Histogram<T,K>::operator/=(const Histogram<OtherBinType,OtherValueType>& divider){

  K zero{};
  
  for(auto itPair = std::make_pair(begin(),divider.begin()); itPair.first != end() && itPair.second != divider.end(); ++itPair.first, ++itPair.second){
    
    if(itPair.second->second != zero) itPair.first->second /= itPair.second->second;
    else Tracer(Verbose::Warning)<<"Histogram division by "<<zero<<" not allowed!"<<std::endl;
    
  }
    
  return *this;
  
}

template <class T, class K>
Histogram<T,K>& Histogram<T,K>::normalise(){

  return normalise(HistogramTypes<T,K>{});
  
}

template <class T, class K>
template <class NormType>
Histogram<T,K>& Histogram<T,K>::scaleCountsTo(const NormType& newNorm){

  return scaleCountsTo(HistogramTypes<T,K>{}, newNorm);

}

template <class T, class K>
Histogram<T,K>& Histogram<T,K>::shiftChannels(const Point<T>& point){
  
  std::map<Bin<T>,K> shiftedMap;//we cannot modify the keys of a map, so create a new map
  for(const auto& pair : countMap) shiftedMap[shift(pair.first, point)] = pair.second;//shift the bins before inserting the key in the map
  std::swap(countMap, shiftedMap);//update countMap
  
  return *this;

}

template <class T, class K>
Histogram<T,K>& Histogram<T,K>::integrateDimension(unsigned dimensionToRemove){

  return integrateDimensions({dimensionToRemove});
  
}

template <class T, class K>
Histogram<T,K>& Histogram<T,K>::integrateDimensions(std::vector<unsigned> dimensionsToRemove){

  std::map<Bin<T>, K> integratedMap;
  for(auto& pair : countMap) integratedMap[compact(pair.first, dimensionsToRemove)] += pair.second;//compact the bin add the content of the old bin to the new map at the compacted bin
  std::swap(countMap, integratedMap);//update countMap

  return *this;
  
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
K Histogram<T,K>::getCount(const Point<T>& point) const{
  
  auto it = std::find_if(countMap.begin(), countMap.end(),[&](const auto& pairBin){return pairBin.first.contains(point);});
  if(it != countMap.end()) return it->second;
  else{
    
    Tracer(Verbose::Error)<<"No channel matches: "<<point<<" => Returning first content"<<std::endl;
    return countMap.begin()->second;
    
  }
  
}

template <class T, class K>
K Histogram<T,K>::getCount(const Bin<T>& bin) const{
  
  return countMap.at(bin);
  
}

template <class T, class K>
K Histogram<T,K>::getTotalCounts() const{

  K totalCounts{};
  for(const auto& pair : countMap) totalCounts += pair.second;
  return totalCounts;
  
}

template <class T, class K>
unsigned Histogram<T,K>::getDimension() const{

  if(!countMap.empty()) return countMap.begin()->first.getDimension();
  else return 0;
  
}

template <class T, class K>
unsigned Histogram<T,K>::getNumberOfChannels() const{

  return countMap.size();
  
}

template <class T, class K>
void Histogram<T,K>::addChannel(const Bin<T>& bin){
  
  if(getDimension() ==  bin.getDimension() || getDimension() == 0)
    countMap.emplace(bin, K{});//default construct K

}

template <class T, class K>
template <class Iterator>
void Histogram<T,K>::addChannels(Iterator begin, Iterator end){

  for(auto it = begin; it != end; ++it) addChannel(*it);
  
}

template <class T, class K>
void Histogram<T,K>::addCount(const Point<T>& point){
  
  addCount(HistogramTypes<T,K>{}, point);
  
}

template <class T, class K>
void Histogram<T,K>::setCount(const Bin<T>& bin, const K& count){

  countMap[bin] = count;
  
}


#endif