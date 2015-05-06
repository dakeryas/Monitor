#ifndef RUN_H
#define RUN_H

#include <iostream>
#include <cmath>

class Run{

  unsigned neutrinos;// number of neutrinos in the run
  double time;// lenght of the run
  double spentEnergy1;//energy spent by reactor 1 during the run
  double spentEnergy2;//energy spent by reactor 2 during the run
  
public:  
  Run();
  Run(unsigned neutrinos, double time, double power1, double power2);//the energy is filled as power*time
  Run& operator+=(const Run& other);
  unsigned getNumberOfNeutrinos() const;
  double getRunningTime() const;
  double getSpentEnergy1() const;
  double getSpentEnergy2() const;
  double getMeanSpentEnergy(double distance1, double distance2);//get the mean spent energy for two reactors situated at distance1 and distance2 from the detector
  double getNeutrinoRate(double distance1, double distance2, double backgroundRate);//get the rate with respect to the total energy spent and substract the background noise
  
};

std::ostream& operator<<(std::ostream& output, const Run& run);
Run operator+(Run run1, const Run& run2);

#endif