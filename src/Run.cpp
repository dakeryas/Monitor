#include "Run.hpp"

std::ostream& operator<<(std::ostream& output, const Run& run){

  output<<std::setw(13)<<std::left<<"Neutrinos"<<": "<<run.getNumberOfCandidates()
    <<std::setw(14)<<std::left<<"\nLength"<<": "<<run.getRunningTime()
    <<std::setw(14)<<std::left<<"\nSpentEnergy1"<<": "<<run.getSpentEnergy1()
    <<std::setw(14)<<"\nSpentEnergy2"<<": "<<run.getSpentEnergy2();
  return output;
  
}

Run::Run():neutrinos(0),time(0),spentEnergy1(0),spentEnergy2(0){
  
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

bool Run::operator==(const Run& other){
  
  for(auto it = std::make_pair(neutrinos.begin(), other.neutrinos.begin()); it.first != neutrinos.end() && it.second != other.neutrinos.end(); ++it.first, ++it.second)
    if(it.first->getEnergy() != it.second->getEnergy()) return false;
    
  if(time != other.time) return false;
  else if(spentEnergy1 != other.spentEnergy1) return false;  
  else if(spentEnergy2 != other.spentEnergy2) return false;
  else return true;

}

unsigned Run::getNumberOfCandidates() const{
  
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
  
  double meanSpentEnergy = getMeanSpentEnergy(distance1, distance2);
  
  if(meanSpentEnergy > 0) return (neutrinos.size() - backgroundRate * time)/getMeanSpentEnergy(distance1, distance2);
  else return 0;

}

double Run::getNeutrinoRateError(double distance1, double distance2, double backgroundRate) const{

  double meanSpentEnergy = getMeanSpentEnergy(distance1, distance2);
  double numberOfNeutrinos = neutrinos.size() - backgroundRate * time;
  if(meanSpentEnergy > 0 && numberOfNeutrinos > 0) return sqrt(numberOfNeutrinos)/getMeanSpentEnergy(distance1, distance2);
  else return std::numeric_limits<double>::infinity();
  
}
