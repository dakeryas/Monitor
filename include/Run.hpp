#ifndef RUN_H
#define RUN_H

#include <vector>
#include <iostream>
#include <cmath>
#include <limits>
#include "Particle.hpp"
#include "Histogram.hpp"

template <class T>
class Run{

  std::vector<Particle> neutrinos;//neutrinos detected during the run
  T time;// lenght of the run
  T spentEnergy1;//energy spent by reactor 1 during the run
  T spentEnergy2;//energy spent by reactor 2 during the run
  
public:  
  Run();
  template <class Iterator>
  Run(Iterator beginNeutrino, Iterator endNeutrino, T time, T power1, T power2);//the energy is filled as power*time
  template <class Container>
  Run(const Container& neutrinos, T time, T power1, T power2);//for iterable containters
  Run<T>& operator+=(const Run<T>& other);
  bool operator==(const Run<T>& other) const;
  unsigned getNumberOfCandidates() const;
  T getRunningTime() const;
  T getSpentEnergy1() const;
  T getSpentEnergy2() const;
  T getMeanSpentEnergy(T distance1, T distance2) const;//get the mean spent energy for two reactors situated at distance1 and distance2 from the detector
  template <class ReturnType = T>
  ReturnType getNeutrinoRate(T distance1, T distance2, T backgroundRate) const;//get the rate with respect to the total energy spent and substract the background noise, if you want the error as well, set ReturnType to Scalar<T>
  template<class BinType, class ValueType, class Iterator>
  Histogram<BinType, ValueType> getNeutrinoSpectrum(Iterator firstBin, Iterator lastBin) const;
  template<class BinType, class ValueType, class Container>
  Histogram<BinType, ValueType> getNeutrinoSpectrum(const Container& bins) const;
  template<class BinType, class ValueType, class Iterator>
  Histogram<BinType, ValueType> getScaledNeutrinoSpectrum(T distance1, T distance2, T backgroundRate, Iterator firstBin, Iterator lastBin) const;
  template<class BinType, class ValueType, class Container>
  Histogram<BinType, ValueType> getScaledNeutrinoSpectrum(T distance1, T distance2, T backgroundRate, const Container& bins) const;
  
};

template <class T>
std::ostream& operator<<(std::ostream& output, const Run<T>& run){

  output<<std::setw(13)<<std::left<<"Neutrinos"<<": "<<run.getNumberOfCandidates()
    <<std::setw(14)<<std::left<<"\nLength"<<": "<<run.getRunningTime()
    <<std::setw(14)<<std::left<<"\nSpentEnergy1"<<": "<<run.getSpentEnergy1()
    <<std::setw(14)<<"\nSpentEnergy2"<<": "<<run.getSpentEnergy2();
  return output;
  
}

template <class T>
Run<T>::Run():neutrinos(T{}),time(T{}),spentEnergy1(T{}),spentEnergy2(T{}){
  
}

template <class T>
template <class Iterator>
Run<T>::Run(Iterator beginNeutrino, Iterator endNeutrino, T time, T power1, T power2):neutrinos(beginNeutrino,endNeutrino),time(time),spentEnergy1(power1*time),spentEnergy2(power2*time){
  
}

template <class T>
template <class Container>
Run<T>::Run(const Container& neutrinos, T time, T power1, T power2):Run<T>(neutrinos.begin(),neutrinos.end(),time, power1, power2){
  
}

template <class T>
Run<T> operator+(Run<T> run1, const Run<T>& run2){
  
  return run1 += run2;

}

template <class T>
Run<T>& Run<T>::operator+=(const Run<T>& other){
  
  neutrinos.insert(neutrinos.end(), other.neutrinos.begin(), other.neutrinos.end());
  time += other.time;
  spentEnergy1 += other.spentEnergy1;
  spentEnergy2 += other.spentEnergy2;
  
  return *this;

}

template <class T>
bool Run<T>::operator==(const Run<T>& other) const{
  
  for(auto it = std::make_pair(neutrinos.begin(), other.neutrinos.begin()); it.first != neutrinos.end() && it.second != other.neutrinos.end(); ++it.first, ++it.second)
    if(it.first->getEnergy() != it.second->getEnergy()) return false;
    
  if(time != other.time) return false;
  else if(spentEnergy1 != other.spentEnergy1) return false;  
  else if(spentEnergy2 != other.spentEnergy2) return false;
  else return true;

}

template <class T>
unsigned Run<T>::getNumberOfCandidates() const{
  
  return neutrinos.size();

}

template <class T>
T Run<T>::getRunningTime() const{
  
  return time;

}

template <class T>
T Run<T>::getSpentEnergy1()  const{
  
  return spentEnergy1;

}

template <class T>
T Run<T>::getSpentEnergy2() const{
  
  return spentEnergy2;

}

template <class T>
T Run<T>::getMeanSpentEnergy(T distance1, T distance2) const{
  
  return (spentEnergy1 * std::pow(distance2, 2) + spentEnergy2 * std::pow(distance1, 2))/distance1/distance2;

}

template <class T>
template <class ReturnType>
ReturnType Run<T>::getNeutrinoRate(T distance1, T distance2, T backgroundRate) const{
  
  T meanSpentEnergy = getMeanSpentEnergy(distance1, distance2);
  ReturnType numberOfNeutrinos = neutrinos.size() - backgroundRate * time;
  
  ReturnType zero{};
  if(meanSpentEnergy > zero && numberOfNeutrinos > zero) return numberOfNeutrinos/meanSpentEnergy;// with ReturnType = Scalar : operator(Scalar, T) returns a Saclar
  else return zero;

}

template <class T>
template<class BinType, class ValueType, class Iterator>
Histogram<BinType, ValueType> Run<T>::getNeutrinoSpectrum(Iterator firstBin, Iterator lastBin) const{

  Histogram<BinType, ValueType> histogram(firstBin, lastBin);
  for(const auto& neutrino : neutrinos) histogram.addCount(Point<T>{neutrino.getEnergy()});
  return histogram;
  
}

template <class T>
template<class BinType, class ValueType, class Container>
Histogram<BinType, ValueType> Run<T>::getNeutrinoSpectrum(const Container& bins) const{
  
  return getNeutrinoSpectrum<BinType, ValueType>(bins.begin(), bins.end());
  
}

template <class T>
template<class BinType, class ValueType, class Iterator>
Histogram<BinType, ValueType> Run<T>::getScaledNeutrinoSpectrum(T distance1, T distance2, T backgroundRate, Iterator firstBin, Iterator lastBin) const{
  
  auto histogram = getNeutrinoSpectrum<BinType, ValueType>(firstBin, lastBin);
  return histogram.scaleCountsTo(getNeutrinoRate<ValueType>(distance1, distance2, backgroundRate));
  
}

template <class T>
template<class BinType, class ValueType, class Container>
Histogram<BinType, ValueType> Run<T>::getScaledNeutrinoSpectrum(T distance1, T distance2, T backgroundRate, const Container& bins) const{
  
  return getScaledNeutrinoSpectrum<BinType, ValueType>(distance1, distance2, backgroundRate, bins.begin(), bins.end());
  
}

#endif