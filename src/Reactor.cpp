#include "Reactor.hpp"

std::ostream& operator<<(std::ostream& output, const Reactor& r){
  
  output<<"Power = "<<r.getPower()<<"\tDistance to detector = "<<r.getDistanceToDetector()<<"\nFuel composition :\n"<<r.getFuel();
  return output;
  
}

Reactor operator+(Reactor r1, const Reactor& r2){
  
  r1 += r2;
  return r1;

}

Reactor::Reactor():Reactor(0, 0, Fuel()){

}

Reactor::Reactor(double power, double distanceToDetector, const Fuel& fuel):power(power),distanceToDetector(distanceToDetector),fuel(fuel){

}

Reactor::~Reactor(){

}

Reactor& Reactor::operator+=(const Reactor& toAdd){
  
  fuel = getWeighedAverage(fuel, toAdd.fuel, power, toAdd.power, distanceToDetector, toAdd.distanceToDetector);
  distanceToDetector = sqrt(distanceToDetector*toAdd.distanceToDetector);
  power = power * toAdd.distanceToDetector/distanceToDetector + toAdd.power * distanceToDetector/toAdd.distanceToDetector;
  
  return *this;

}

const double& Reactor::getPower() const{
  
  return power;

}

const double& Reactor::getDistanceToDetector() const{
  
  return distanceToDetector;
  
}

const Fuel& Reactor::getFuel() const{
  
  return fuel;

}

void Reactor::setPower(double power){
  
  this->power = power;

}

void Reactor::setDistanceToDetector(double distance){
  
  this->distanceToDetector = distance;

}

void Reactor::setFuel(const Fuel& fuel){
  
  this->fuel = fuel;

}

bool Reactor::isOff(){
  
  return !(power > 0);

}


