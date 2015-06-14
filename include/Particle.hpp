#ifndef PARTICLE_H
#define PARTICLE_H

#include <iostream>

class Particle{

  double energy;
public:
  Particle();
  Particle(double energy);
  double getEnergy() const;
  void setEnergy(double energy);
  
};

std::ostream& operator<<(std::ostream& output, const Particle& particle);//for input masses in MeV sets the file into GeV

#endif