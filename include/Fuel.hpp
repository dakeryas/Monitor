#ifndef FUEL_H
#define FUEL_H

#include <map>
#include <cmath>
#include <iostream>
#include <Rtypes.h>

class Fuel{
  
  std::map<std::string, double> fraction;

public:
  Fuel();
  Fuel(const double numberOfFissions235U, const double numberOfFissions238U, const double numberOfFissions239Pu, const double numberOfFissions241Pu);//pass the total number of fissions that occured during the run
  virtual ~Fuel();
  Fuel& getWeighedAverage(const Fuel& toAdd, const double power1, const double power2, const double L1, const double L2);//weighs "this" with toAdd according to the power and distances, and returns *this
  double getFrac(const std::string& fracName) const;
  void setFrac(const std::string& fracName, const double fracValue);
  bool isEmpty();
  void Print(std::ostream& output) const;
  
  ClassDef(Fuel, 1)
  
};

Fuel getWeighedAverage(Fuel fuel1, const Fuel& fuel2, const double power1, const double power2, const double L1, const double L2); //calls fuel1.getWeighedAverage(fuel2...)
std::ostream& operator<<(std::ostream& output, const Fuel& fuel);//fills "output" Print()

#endif