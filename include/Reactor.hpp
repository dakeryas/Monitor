#ifndef REACTOR_H
#define REACTOR_H

#include "Fuel.hpp"

class Reactor{

  double power;//power in Mega Watts
  double distanceToDetector;//distance to the detector
  Fuel fuel; // to store the fuel composition

public:
  Reactor();
  Reactor(const double power, const double distanceToDetector, const Fuel& fuel);
  virtual ~Reactor();
  Reactor& operator+=(const Reactor& toAdd);//returns a reactor whose fuel is averaged according to the power and distance
  const double& getPower() const;
  const double& getDistanceToDetector() const;
  const Fuel& getFuel() const;
  void setPower(const double power);
  void setDistanceToDetector(const double distance);
  void setFuel(const Fuel& fuel);
  bool isOff();//checks if the power is non-zero
  
  ClassDef(Reactor, 1)
  
};

std::ostream& operator<<(std::ostream& output, const Reactor& r);//for input masses in MeV sets the file into GeV
Reactor operator+(Reactor r1, const Reactor& r2);

#endif