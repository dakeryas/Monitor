#ifndef RUN_H
#define RUN_H

#include <vector>
#include <iostream>
#include <cmath>
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
  unsigned getNumberOfNeutrinos() const;
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
  
};

std::ostream& operator<<(std::ostream& output, const Run& run){//for input masses in MeV sets the file into GeV

  output<<"Neutrinos = "<<run.getNumberOfNeutrinos()<<"\nTime = "<<run.getRunningTime()<<"\nSpentEnergy1 = "<<run.getSpentEnergy1()<<"\nSpentEnergy2 = "<<run.getSpentEnergy2();
  return output;
  
}

Run::Run():neutrinos(0),time(0),spentEnergy1(0),spentEnergy2(0){
  
}

template <class Iterator>
Run::Run(Iterator beginNeutrino, Iterator endNeutrino, double time, double power1, double power2):neutrinos(beginNeutrino,endNeutrino),time(time),spentEnergy1(power1*time),spentEnergy2(power2*time){
  
}

template <class Container>
Run::Run(const Container& neutrinos, double time, double power1, double power2):Run(neutrinos.begin(),neutrinos.end(),time, power1, power2){
  
}

Run operator+(Run run1, const Run& run2){
  
  return run1 += run2;

}

Run& Run::operator+=(const Run& other){
  
  neutrinos.insert(neutrinos.end(), other.neutrinos.begin(), other.neutrinos.end());
  time += other.time;
  spentEnergy1 += other.spentEnergy1;
  spentEnergy2 += other.spentEnergy2;
  
  return *this;

}

unsigned Run::getNumberOfNeutrinos() const{
  
  return neutrinos.size();

}

double Run::getRunningTime() const{
  
  return time;

}

double Run::getSpentEnergy1()  const{
  
  return spentEnergy1;

}

double Run::getSpentEnergy2() const{
  
  return spentEnergy2;

}

double Run::getMeanSpentEnergy(double distance1, double distance2) const{
  
  return (spentEnergy1 * pow(distance2, 2) + spentEnergy2 * pow(distance1, 2))/distance1/distance2;

}

double Run::getNeutrinoRate(double distance1, double distance2, double backgroundRate) const{
  
  return (neutrinos.size() - backgroundRate * time)/getMeanSpentEnergy(distance1, distance2);

}

double Run::getNeutrinoRateError(double distance1, double distance2, double backgroundRate) const{

  return sqrt(neutrinos.size() - backgroundRate * time)/getMeanSpentEnergy(distance1, distance2);
  
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

#endif