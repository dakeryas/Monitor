#ifndef EXPERIMENT_H
#define EXPERIMENT_H

#include <algorithm>
#include "Run.hpp"
#include "Scalar.hpp"

template <class T,class K>
class Experiment{//class meant to hold runs in the corresponding configuration bin

  K distance1;//distance to reactor 1
  K distance2;// distance to reactor 2
  K backgroundRate;//background rate for all runs of the  map
  std::map<Bin<T>, Run<K>> runMap;//configuration and corresponding extended run containing the detected neutrino rate

public:  
  Experiment(K distance1, K distance2, K backgroundRate = 0);
  K getDistance1() const;
  K getDistance2() const;
  K getBackgroundRate() const;
  typename std::map<Bin<T>, Run<K>>::const_iterator begin() const;
  typename std::map<Bin<T>, Run<K>>::const_iterator end() const;
  void setDistance1(K  distance1);
  void setDistance2(K distance2);
  void setBackgroundRate(K backgroundRate);
  unsigned getConfigurationSize() const;
  unsigned getNumberOfChannels() const;
  const Run<K>& getRun(const Point<T>& configuration) const;//get run that corresponds
  template <class BinType, class ValueType, class Iterator>
  Histogram<BinType, ValueType> getScaledNeutrinoSpectrum(const Point<T>& configuration, Iterator firstBin, Iterator lastBin) const;
  template <class BinType, class ValueType, class Container>
  Histogram<BinType, ValueType> getScaledNeutrinoSpectrum(const Point<T>& configuration, const Container& bins) const;
  template <class BinType, class ValueType>
  Histogram<BinType, ValueType> getRateHistogram() const;
  void emplaceChannel(T binLowEdge, T binUpEdge);
  void addChannel(const Bin<T>& bin);
  template <class Iterator>
  void addChannels(Iterator begin, Iterator end);//copy channels pointed to from begin to end
  template <class Container>
  void addChannels(const Container& channels);//if iterable channels
  void addRun(const Point<T>& configuration, const Run<K>& run);//add the run to the corresponding configuration
  void clear();//deletes all channels and runs
  Experiment<T,K>& slim();//removes all channels with no runs
  Experiment<T,K>& integrateChannel(unsigned channelToRemove);
  Experiment<T,K>& integrateChannels(std::vector<unsigned> channelsToRemove);//compacts bins and sums the corresponding Run<K>'s into the compacted bin
  
};

template <class T,class K>
std::ostream& operator<<(std::ostream& output, const Experiment<T,K>& experiment){
  
  output.precision(4);
  output<<(experiment.template getRateHistogram<T,Scalar<K>>());
  
  return output;
  
}

template <class T,class K>
Experiment<T,K>::Experiment(K distance1, K distance2, K backgroundRate):distance1(distance1),distance2(distance2),backgroundRate(backgroundRate){

}

template <class T,class K>
K Experiment<T,K>::getDistance1() const{
  
  return distance1;

}

template <class T,class K>
K Experiment<T,K>::getDistance2() const{
  
  return distance2;

}

template <class T,class K>
K Experiment<T,K>::getBackgroundRate() const{
  
  return backgroundRate;

}

template <class T,class K>
unsigned Experiment<T,K>::getConfigurationSize() const{

  if(!runMap.empty()) return runMap.begin()->first.getDimension();
  else return 0;
  
}

template <class T,class K>
unsigned Experiment<T,K>::getNumberOfChannels() const{

  return runMap.size();
  
}

template <class T,class K>
const Run<K>& Experiment<T,K>::getRun(const Point<T>& configuration) const{

  auto it = std::find_if(runMap.begin(), runMap.end(),[&](const auto& pairRun){return pairRun.first.contains(configuration);});
  if(it != runMap.end()) return it->second;
  else{
    
    Tracer(Verbose::Error)<<"No run matches: "<<configuration<<" => Returning first run"<<std::endl;
    return runMap.begin()->second;
    
  }
  
}

template <class T,class K>
template <class BinType, class ValueType, class Iterator>
Histogram<BinType, ValueType> Experiment<T,K>::getScaledNeutrinoSpectrum(const Point<T>& configuration, Iterator firstBin, Iterator lastBin) const{

  return getRun(configuration).template getScaledNeutrinoSpectrum<BinType,ValueType>(distance1, distance2, backgroundRate, firstBin, lastBin);
  
}

template <class T,class K>
template <class BinType, class ValueType, class Container>
Histogram<BinType, ValueType> Experiment<T,K>::getScaledNeutrinoSpectrum(const Point<T>& configuration, const Container& bins) const{

  return getScaledNeutrinoSpectrum<BinType,ValueType>(configuration, bins.begin(), bins.end());
  
}

template <class T,class K>
template <class BinType, class ValueType>
Histogram<BinType, ValueType> Experiment<T,K>::getRateHistogram() const{

  Histogram<BinType, ValueType> rate;
  for(const auto& pair : runMap) rate.setCount(pair.first, pair.second.template getNeutrinoRate<ValueType>(distance1, distance2, backgroundRate));
  return rate;

}

template <class T,class K>
typename std::map<Bin<T>, Run<K>>::const_iterator Experiment<T,K>::begin() const{
  
  return runMap.begin();

}

template <class T,class K>
typename std::map<Bin<T>, Run<K>>::const_iterator Experiment<T,K>::end() const{
  
  return runMap.end();

}

template <class T,class K>
void Experiment<T,K>::setDistance1(K distance1){
  
  this->distance1 = distance1;

}

template <class T,class K>
void Experiment<T,K>::setDistance2(K distance2){

  this->distance2 = distance2;
  
}

template <class T,class K>
void Experiment<T,K>::setBackgroundRate(K backgroundRate){
  
  this->backgroundRate = backgroundRate;

}

template <class T,class K>
void Experiment<T,K>::emplaceChannel(T binLowEdge, T binUpEdge){

  addChannel(Bin<T>(binLowEdge, binUpEdge));  
  
}

template <class T,class K>
void Experiment<T,K>::addChannel(const Bin<T>& bin){
  
  runMap.emplace(bin, Run<K>{});//default construct the Run<K> to zero neutrinos and zero time

}

template <class T,class K>
template <class Iterator>
void Experiment<T,K>::addChannels(Iterator begin, Iterator end){

  for(auto it = begin; it != end; ++it) addChannel(*it);
  
}

template <class T,class K>
template <class Container>
void Experiment<T,K>::addChannels(const Container& channels){
  
  addChannels(channels.begin(), channels.end());

}

template <class T,class K>
void Experiment<T,K>::addRun(const Point<T>& configuration, const Run<K>& run){

  auto it = std::find_if(runMap.begin(), runMap.end(),[&](const auto& pair){return pair.first.contains(configuration);});
  if(it != runMap.end()) it->second += run;
  else Tracer(Verbose::Warning)<<"No channel matches: "<<configuration<<" => Run<K> not added"<<std::endl;
  
}

template <class T,class K>
void Experiment<T,K>::clear(){
  
  runMap.clear();

}

template <class T,class K>
Experiment<T,K>& Experiment<T,K>::slim(){
  
  auto itDelete = std::find_if(runMap.begin(),runMap.end(),[](const auto& pair){return pair.second == Run<K>{};});
  while(itDelete != runMap.end()){
    
    auto itPastGarbage = runMap.erase(itDelete);//delete element and return an iterator to element past the one deleted
    itDelete = std::find_if(itPastGarbage,runMap.end(),[](const auto& pair){return pair.second == Run<K>{};});//start from the last deleted element for efficiency
    
  }
  
  return *this;

}

template <class T,class K>
Experiment<T,K>& Experiment<T,K>::integrateChannel(unsigned channelToRemove){

  return integrateChannel({channelToRemove});
  
}

template <class T,class K>
Experiment<T,K>& Experiment<T,K>::integrateChannels(std::vector<unsigned> channelsToRemove){

  std::map<Bin<T>, Run<K>> integratedMap;
  for(auto& pair : runMap) integratedMap[compact(pair.first, channelsToRemove)] += pair.second;//compact the bin add the content of the old bin to the new map at the compacted bin
  std::swap(runMap, integratedMap);//update countMap

  return *this;
  
}

#endif