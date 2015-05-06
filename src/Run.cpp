#include "Run.hpp"

std::ostream& operator<<(std::ostream& output, const Run& run){//for input masses in MeV sets the file into GeV

  output<<"Neutrinos = "<<run.getNumberOfNeutrinos()<<"\nTime = "<<run.getRunningTime()<<"\nSpentEnergy1 = "<<run.getSpentEnergy1()<<"\nSpentEnergy2 = "<<run.getSpentEnergy2();
  return output;
  
}

Run::Run():neutrinos(0),time(0),spentEnergy1(0),spentEnergy2(0){
  
}

Run::Run(unsigned neutrinos, double time, double power1, double power2):neutrinos(neutrinos),time(time),spentEnergy1(power1*time),spentEnergy2(power2*time){
  
}

Run operator+(Run run1, const Run& run2){
  
  return run1 += run2;

}

Run& Run::operator+=(const Run& other){
  
  neutrinos += other.neutrinos;
  time += other.time;
  spentEnergy1 += other.spentEnergy1;
  spentEnergy2 += other.spentEnergy2;
  
  return *this;

}

unsigned Run::getNumberOfNeutrinos() const{
  
  return neutrinos;

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

double Run::getMeanSpentEnergy(double distance1, double distance2){
  
  return (spentEnergy1 * pow(distance2, 2) + spentEnergy2 * pow(distance1, 2))/distance1/distance2;

}

double Run::getNeutrinoRate(double distance1, double distance2, double backgroundRate){
  
  return (neutrinos - backgroundRate * time)/getMeanSpentEnergy(distance1, distance2);

}
