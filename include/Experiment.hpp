#ifndef EXPERIMENT_H
#define EXPERIMENT_H

#include <map>
#include "Bin.hpp"
#include "Run.hpp"

template <class T>
class Experiment{

  double distance1;//distance to reactor 1
  double distance2;// distance to reactor 2
  double backgroundRate;//background rate for all runs of the  map
  std::map<Bin<T>, Run> runMap;//fraction and corresponding extended run containing the detected neutrino rate

public:  
  Experiment(double distance1, double distance2, double backgroundRate = 0);
  double getDistance1() const;
  double getDistance2() const;
  double getBackgroundRate() const;
  std::map<Bin<T>, Run> getRunMap() const;
  void setDistance1(double  distance1) const;
  void setDistance2(double distance2)const;
  void setBackgroundRate(double backgroundRate) const;
  void addRun(double fraction, Run run);//add the run to the corresponding fraction
  void clearRuns();
  
};

template <class T>
std::ostream& operator<<(std::ostream& output, const Experiment<T>& experiment){
  
  for(auto& pair : experiment.getRunMap()) output<<pair.first<<std::setw(6)<<std::left<<" "
    <<"-->"<<std::setw(6)<<std::left<<" "<<std::setw(6)<<std::left<<pair.second.getNeutrinoRate(experiment.getDistance1(), experiment.getDistance2(), experiment.getBackgroundRate());
  
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
std::map< Bin<T>, Run > Experiment<T>::getRunMap() const{

  return runMap;
  
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
void Experiment<T>::addRun(double fraction, Run run){

  for(auto& pair : runMap) if(pair.first.contains(fraction)) runMap[pair.first] += run;
  
}

template <class T>
void Experiment<T>::clearRuns(){
  
  runMap.clear();

}

#endif