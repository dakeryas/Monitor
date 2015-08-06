#ifndef EXPERIMENT_H
#define EXPERIMENT_H

#include <algorithm>
#include "Run.hpp"
#include "Scalar.hpp"

template <class T>
class Experiment{//class meant to hold runs in the corresponding configuration bin

  double distance1;//distance to reactor 1
  double distance2;// distance to reactor 2
  double backgroundRate;//background rate for all runs of the  map
  std::map<Bin<T>, Run> runMap;//configuration and corresponding extended run containing the detected neutrino rate
  
  template <class BinType, class ValueType>
  struct HistogramTypes{};
  
  template <class BinType, class ValueType>
  Histogram<BinType, ValueType> getRateHistogram(HistogramTypes<BinType, ValueType>) const;
  template <class BinType, class ValueType>
  Histogram<BinType, Scalar<ValueType>> getRateHistogram(HistogramTypes<BinType, Scalar<ValueType>>) const;

public:  
  Experiment(double distance1, double distance2, double backgroundRate = 0);
  double getDistance1() const;
  double getDistance2() const;
  double getBackgroundRate() const;
  typename std::map<Bin<T>, Run>::const_iterator begin() const;
  typename std::map<Bin<T>, Run>::const_iterator end() const;
  void setDistance1(double  distance1);
  void setDistance2(double distance2);
  void setBackgroundRate(double backgroundRate);
  unsigned getConfigurationSize() const;
  unsigned getNumberOfChannels() const;
  const Run& getRun(const Point<T>& configuration) const;//get run that corresponds
  template <class BinType, class ValueType, class Iterator>
  Histogram<BinType, ValueType> getScaledNeutrinoSpectrum(const Point<T>& configuration, Iterator firstBin, Iterator lastBin) const;
  template <class BinType, class ValueType, class Container>
  Histogram<BinType, ValueType> getScaledNeutrinoSpectrum(const Point<T>& configuration, const Container& bins) const;
  template <class BinType, class ValueType>
  Histogram<BinType, ValueType> getRateHistogram() const;
  void emplaceChannel(double binLowEdge, double binUpEdge);
  void addChannel(const Bin<T>& bin);
  template <class Iterator>
  void addChannels(Iterator begin, Iterator end);//copy channels pointed to from begin to end
  template <class Container>
  void addChannels(const Container& channels);//if iterable channels
  void addRun(const Point<T>& configuration, const Run& run);//add the run to the corresponding configuration
  void clear();//deletes all channels and runs
  Experiment<T>& slim();//removes all channels with no runs
  Experiment<T>& integrateChannel(unsigned channelToRemove);
  Experiment<T>& integrateChannels(std::vector<unsigned> channelsToRemove);//compacts bins and sums the corresponding Run's into the compacted bin
  
};

template <class T>
std::ostream& operator<<(std::ostream& output, const Experiment<T>& experiment){
  
  output.precision(4);
  output<<(experiment.template getRateHistogram<T,Scalar<T>>());
  
  return output;
  
}

template <class T>
template <class BinType, class ValueType>
Histogram<BinType, ValueType> Experiment<T>::getRateHistogram(HistogramTypes<BinType,ValueType>) const{

  Histogram<BinType, ValueType> rate;
  for(const auto& pair : runMap) rate.setCount(pair.first, pair.second.getNeutrinoRate(distance1, distance2, backgroundRate));
  return rate;

}

template <class T>
template <class BinType, class ValueType>
Histogram<BinType, Scalar<ValueType>> Experiment<T>::getRateHistogram(HistogramTypes<BinType,Scalar<ValueType>>) const{

  Histogram<BinType, Scalar<double>> rate;
  for(const auto& pair : runMap)
    rate.setCount(pair.first, Scalar<ValueType>(pair.second.getNeutrinoRate(distance1, distance2, backgroundRate), pair.second.getNeutrinoRateError(distance1, distance2, backgroundRate)));
  return rate;

}

template <class T>
Experiment<T>::Experiment(double distance1, double distance2, double backgroundRate):distance1(distance1),distance2(distance2),backgroundRate(backgroundRate){

}

template <class T>
double Experiment<T>::getDistance1() const{
  
  return distance1;

}

template <class T>
double Experiment<T>::getDistance2() const{
  
  return distance2;

}

template <class T>
double Experiment<T>::getBackgroundRate() const{
  
  return backgroundRate;

}

template <class T>
unsigned Experiment<T>::getConfigurationSize() const{

  if(!runMap.empty()) return runMap.begin()->first.getDimension();
  else return 0;
  
}

template <class T>
unsigned Experiment<T>::getNumberOfChannels() const{

  return runMap.size();
  
}

template <class T>
const Run& Experiment<T>::getRun(const Point<T>& configuration) const{

  auto it = std::find_if(runMap.begin(), runMap.end(),[&](const auto& pairRun){return pairRun.first.contains(configuration);});
  if(it != runMap.end()) return it->second;
  else{
    
    Tracer(Verbose::Error)<<"No run matches: "<<configuration<<" => Returning first run"<<std::endl;
    return runMap.begin()->second;
    
  }
  
}

template <class T>
template <class BinType, class ValueType, class Iterator>
Histogram<BinType, ValueType> Experiment<T>::getScaledNeutrinoSpectrum(const Point<T>& configuration, Iterator firstBin, Iterator lastBin) const{

  return getRun(configuration).template getScaledNeutrinoSpectrum<BinType,ValueType>(distance1, distance2, backgroundRate, firstBin, lastBin);
  
}

template <class T>
template <class BinType, class ValueType, class Container>
Histogram<BinType, ValueType> Experiment<T>::getScaledNeutrinoSpectrum(const Point<T>& configuration, const Container& bins) const{

  return getScaledNeutrinoSpectrum<BinType,ValueType>(configuration, bins.begin(), bins.end());
  
}

template <class T>
template <class BinType, class ValueType>
Histogram<BinType, ValueType> Experiment<T>::getRateHistogram() const{

  return getRateHistogram(HistogramTypes<BinType, ValueType>());//use the default constructor for 'type' because you only need to identify the type of 'type'

}

template <class T>
typename std::map<Bin<T>, Run>::const_iterator Experiment<T>::begin() const{
  
  return runMap.begin();

}

template <class T>
typename std::map<Bin<T>, Run>::const_iterator Experiment<T>::end() const{
  
  return runMap.end();

}

template <class T>
void Experiment<T>::setDistance1(double distance1){
  
  this->distance1 = distance1;

}

template <class T>
void Experiment<T>::setDistance2(double distance2){

  this->distance2 = distance2;
  
}

template <class T>
void Experiment<T>::setBackgroundRate(double backgroundRate){
  
  this->backgroundRate = backgroundRate;

}

template <class T>
void Experiment<T>::emplaceChannel(double binLowEdge, double binUpEdge){

  addChannel(Bin<T>(binLowEdge, binUpEdge));  
  
}

template <class T>
void Experiment<T>::addChannel(const Bin<T>& bin){
  
  runMap.emplace(bin, Run{});//default construct the Run to zero neutrinos and zero time

}

template <class T>
template <class Iterator>
void Experiment<T>::addChannels(Iterator begin, Iterator end){

  for(auto it = begin; it != end; ++it) addChannel(*it);
  
}

template <class T>
template <class Container>
void Experiment<T>::addChannels(const Container& channels){
  
  addChannels(channels.begin(), channels.end());

}

template <class T>
void Experiment<T>::addRun(const Point<T>& configuration, const Run& run){

  auto it = std::find_if(runMap.begin(), runMap.end(),[&](const auto& pair){return pair.first.contains(configuration);});
  if(it != runMap.end()) it->second += run;
  else Tracer(Verbose::Warning)<<"No channel matches: "<<configuration<<" => Run not added"<<std::endl;
  
}

template <class T>
void Experiment<T>::clear(){
  
  runMap.clear();

}

template <class T>
Experiment<T>& Experiment<T>::slim(){
  
  auto itDelete = std::find_if(runMap.begin(),runMap.end(),[](const auto& pair){return pair.second == Run{};});
  while(itDelete != runMap.end()){
    
    auto itPastGarbage = runMap.erase(itDelete);//delete element and return an iterator to element past the one deleted
    itDelete = std::find_if(itPastGarbage,runMap.end(),[](const auto& pair){return pair.second == Run{};});//start from the last deleted element for efficiency
    
  }
  
  return *this;

}

template <class T>
Experiment<T>& Experiment<T>::integrateChannel(unsigned channelToRemove){

  return integrateChannel({channelToRemove});
  
}

template <class T>
Experiment<T>& Experiment<T>::integrateChannels(std::vector<unsigned> channelsToRemove){

  std::map<Bin<T>, Run> integratedMap;
  for(auto& pair : runMap) integratedMap[compact(pair.first, channelsToRemove)] += pair.second;//compact the bin add the content of the old bin to the new map at the compacted bin
  std::swap(runMap, integratedMap);//update countMap

  return *this;
  
}

#endif