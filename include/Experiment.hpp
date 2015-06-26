#ifndef EXPERIMENT_H
#define EXPERIMENT_H

#include <map>
#include "Bin.hpp"
#include "Run.hpp"

template <class T>
class Experiment{//class meant to hold runs in the corresponding configuration bin

  double distance1;//distance to reactor 1
  double distance2;// distance to reactor 2
  double backgroundRate;//background rate for all runs of the  map
  std::map<Bin<T>, Run> runMap;//configuration and corresponding extended run containing the detected neutrino rate

public:  
  Experiment(double distance1, double distance2, double backgroundRate = 0);
  double getDistance1() const;
  double getDistance2() const;
  double getBackgroundRate() const;
  typename std::map<Bin<T>, Run>::const_iterator begin() const;
  typename std::map<Bin<T>, Run>::const_iterator end() const;
  void setDistance1(double  distance1) const;
  void setDistance2(double distance2) const;
  void setBackgroundRate(double backgroundRate) const;
  unsigned getConfigurationSize() const;
  unsigned getNumberOfChannels() const;
  void emplaceChannel(double binLowEdge, double binUpEdge) const;
  void addChannel(const Bin<T>& bin);
  template <class Iterator>
  void addChannels(Iterator begin, Iterator end);//copy channels pointed to from begin to end
  template <class Container>
  void addChannels(const Container& channels);//if iterable container
  void addRun(const Point<T>& configuration, const Run& run);//add the run to the corresponding configuration
  void clear();//deletes all channels and runs
  
};

template <class T>
std::ostream& operator<<(std::ostream& output, const Experiment<T>& experiment){
  
  output.precision(4);
  
  for(const auto& pair : experiment)
    output<<pair.first<<std::setw(5)<<std::left<<" "
    <<"-->"<<std::setw(5)<<std::left<<" "<<std::setw(7)<<std::left<<pair.second.getNeutrinoRate(experiment.getDistance1(), experiment.getDistance2(), experiment.getBackgroundRate())
    <<"+/-"<<std::setw(2)<<std::left<<" "<<std::setw(5)<<std::left<<pair.second.getNeutrinoRateError(experiment.getDistance1(), experiment.getDistance2(), experiment.getBackgroundRate())
    <<"\n";
  
  return output;
  
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
typename std::map<Bin<T>, Run>::const_iterator Experiment<T>::begin() const{
  
  return runMap.begin();

}

template <class T>
typename std::map<Bin<T>, Run>::const_iterator Experiment<T>::end() const{
  
  return runMap.end();

}


template <class T>
void Experiment<T>::setDistance1(double distance1) const{
  
  this->distance1 = distance1;

}

template <class T>
void Experiment<T>::setDistance2(double distance2) const{

  this->distance2 = distance2;
  
}

template <class T>
void Experiment<T>::setBackgroundRate(double backgroundRate) const{
  
  this->backgroundRate = backgroundRate;

}

template <class T>
void Experiment<T>::emplaceChannel(double binLowEdge, double binUpEdge) const{

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

  if(getConfigurationSize() == configuration.getDimension()){
  
    for(auto& pair : runMap) if(pair.first.contains(configuration)) runMap[pair.first] += run;
    
  }
  else std::cout<<configuration<<" is not compatible with previous configurations of size "<<getConfigurationSize()<<std::endl;
  
}

template <class T>
void Experiment<T>::clear(){
  
  runMap.clear();

}

#endif