#include "Particle.hpp"

ClassImp(Particle)

std::ostream& operator<<(std::ostream& output, const Particle& p){//for input masses in MeV sets the file into GeV

  output<<"Energy = "<<p.energy<<"\n"<<p.position;
  return output;
  
}

Particle::Particle():Particle(0, Point()){
  
}

Particle::Particle(const double energy, const Point position):energy(energy),position(position){
  
}


Particle::Particle(const double energy, const double x, const double y, const double z):Particle(energy, Point(x,y,z)){

}

bool Particle::isAboveEnergyThreshold(const double energyThreshold){
  
  return energy > energyThreshold; 

}

Particle::~Particle(){
  
}
