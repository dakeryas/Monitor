#ifndef PARTICLE_H
#define PARTICLE_H

#include "Point.hpp"

class Particle{

public:
  double energy;
  Point position; // to store the whole vertex position(only one per event at the moment)
  
  Particle();
  Particle(const double energy, const Point position);
  Particle(const double energy, const double x, const double y, const double z);
  virtual ~Particle();
  bool isAboveEnergyThreshold(const double energyThreshold);
  
  ClassDef(Particle, 1)
  
};

std::ostream& operator<<(std::ostream& output, const Particle& p);//for input masses in MeV sets the file into GeV

#endif