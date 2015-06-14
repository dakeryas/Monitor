#include "Particle.hpp"

std::ostream& operator<<(std::ostream& output, const Particle& particle){

  output<<"Energy = "<<particle.getEnergy();
  return output;
  
}

Particle::Particle():Particle(0){
  
}

Particle::Particle(double energy):energy(energy){
  
}

double Particle::getEnergy() const{
  
  return energy;

}

void Particle::setEnergy(double energy){
  
  this->energy = energy;

}

