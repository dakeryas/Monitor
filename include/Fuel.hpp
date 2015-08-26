#ifndef FUEL_H
#define FUEL_H

#include <map>
#include <cmath>
#include <iostream>

class Fuel{
  
  std::map<std::string, double> fraction;

public:
  Fuel();
  Fuel(double numberOfFissions235U, double numberOfFissions238U, double numberOfFissions239Pu, double numberOfFissions241Pu);//pass the total number of fissions that occured during the run
  virtual ~Fuel();
  Fuel& getWeighedAverage(const Fuel& toAdd, double power1, double power2, double L1, double L2);//weighs "this" with toAdd according to the power and distances, and returns *this
  double getFrac(const std::string& fracName) const;
  void setFrac(const std::string& fracName, double fracValue);
  bool isEmpty();
  void Print(std::ostream& output) const;
  
};

Fuel getWeighedAverage(Fuel fuel1, const Fuel& fuel2, double power1, double power2, double L1, double L2); //calls fuel1.getWeighedAverage(fuel2...)
std::ostream& operator<<(std::ostream& output, const Fuel& fuel);//fills "output" Print()

#endif
