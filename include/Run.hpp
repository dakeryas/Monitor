#ifndef RUN_H
#define RUN_H

#include <vector>
#include <iostream>
#include <cmath>
#include <limits>
#include "Particle.hpp"
#include "Histogram.hpp"

class Run{

  std::vector<Particle> neutrinos;//neutrinos detected during the run
  double time;// lenght of the run
  double spentEnergy1;//energy spent by reactor 1 during the run
  double spentEnergy2;//energy spent by reactor 2 during the run
  
public:  
  Run();
  template <class Iterator>
  Run(Iterator beginNeutrino, Iterator endNeutrino, double time, double power1, double power2);//the energy is filled as power*time
  template <class Container>
  Run(const Container& neutrinos, double time, double power1, double power2);//for iterable containters
  Run& operator+=(const Run& other);
  bool operator==(const Run& other) const;
  unsigned getNumberOfCandidates() const;
  double getRunningTime() const;
  double getSpentEnergy1() const;
  double getSpentEnergy2() const;
  double getMeanSpentEnergy(double distance1, double distance2) const;//get the mean spent energy for two reactors situated at distance1 and distance2 from the detector
  double getNeutrinoRate(double distance1, double distance2, double backgroundRate) const;//get the rate with respect to the total energy spent and substract the background noise
  double getNeutrinoRateError(double distance1, double distance2, double backgroundRate) const;
  template <class T, class K, class Iterator>
  Histogram<T,K> getNeutrinoSpectrum(Iterator firstBin, Iterator lastBin) const;
  template <class T, class K, class Container>
  Histogram<T,K> getNeutrinoSpectrum(const Container& bins) const;
  template <class T, class K, class Iterator>
  Histogram<T,K> getScaledNeutrinoSpectrum(double distance1, double distance2, double backgroundRate, Iterator firstBin, Iterator lastBin) const;
  template <class T, class K, class Container>
  Histogram<T,K> getScaledNeutrinoSpectrum(double distance1, double distance2, double backgroundRate, const Container& bins) const;
  
};

std::ostream& operator<<(std::ostream& output, const Run& run);
Run operator+(Run run1, const Run& run2);

template <class Iterator>
Run::Run(Iterator beginNeutrino, Iterator endNeutrino, double time, double power1, double power2):neutrinos(beginNeutrino,endNeutrino),time(time),spentEnergy1(power1*time),spentEnergy2(power2*time){
  
}

template <class Container>
Run::Run(const Container& neutrinos, double time, double power1, double power2):Run(neutrinos.begin(),neutrinos.end(),time, power1, power2){
  
}

template <class T, class K, class Iterator>
Histogram<T,K> Run::getNeutrinoSpectrum(Iterator firstBin, Iterator lastBin) const{

  Histogram<T,K> histogram(firstBin, lastBin);
  for(const auto& neutrino : neutrinos) histogram.addCount(Point<double>{neutrino.getEnergy()});
  return histogram;
  
}

template <class T, class K, class Container>
Histogram<T,K> Run::getNeutrinoSpectrum(const Container& bins) const{
  
  return getNeutrinoSpectrum<T,K>(bins.begin(), bins.end());
  
}

template <class T, class K, class Iterator>
Histogram<T,K> Run::getScaledNeutrinoSpectrum(double distance1, double distance2, double backgroundRate, Iterator firstBin, Iterator lastBin) const{
  
  auto histogram = getNeutrinoSpectrum<T,K>(firstBin, lastBin);
  return histogram.scaleCountsTo(getNeutrinoRate(distance1, distance2, backgroundRate));
  
}

template <class T, class K, class Container>
Histogram<T,K> Run::getScaledNeutrinoSpectrum(double distance1, double distance2, double backgroundRate, const Container& bins) const{
  
  return getScaledNeutrinoSpectrum<T,K>(distance1, distance2, backgroundRate, bins.begin(), bins.end());
  
}

#endif