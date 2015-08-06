#ifndef EXPERIMENTEXTRACTOR_H
#define EXPERIMENTEXTRACTOR_H

#include "TTree.h"
#include "Experiment.hpp"
#include "Reactor.hpp"
#include "Constants.hpp"

class ExperimentExtractor{
  TTree* data;
  TTree* simu1;
  TTree* simu2;
//for data tree  
  int runData;
  double currentEnergy;
//for simulation trees  
  int runSimu;
  double runLength, numberOfNeutrinosSimu1, power1, f239Pu_1, f241Pu_1, f235U_1, f238U_1;
  double numberOfNeutrinosSimu2, power2, f239Pu_2, f241Pu_2, f235U_2, f238U_2;

public:
  ExperimentExtractor() = delete;
  ExperimentExtractor(TTree* data, TTree* simu1, TTree* simu2);
  ~ExperimentExtractor() = default;//do not release the pointers you do not own
  template <class T, class Iterator>
  Experiment<T> extractExperiment(double distance1, double distance2, double backgroundRate, Iterator beginChannel, Iterator endChannel);
  template <class T, class Container>
  Experiment<T> extractExperiment(double distance1, double distance2, double backgroundRate, const Container& channels);
  
};

template <class T, class Iterator>
Experiment<T> ExperimentExtractor::extractExperiment(double distance1, double distance2, double backgroundRate, Iterator beginChannel, Iterator endChannel){
  
  std::vector<Particle> neutrinos;
  Reactor reactor1, reactor2;
  reactor1.setDistanceToDetector(distance1);
  reactor2.setDistanceToDetector(distance2);
  Fuel equivalentFuel;

  Experiment<T> experiment(distance1, distance2, backgroundRate);
  experiment.addChannels(beginChannel, endChannel);
  
  unsigned i = 0;
  data->GetEntry(i);
  
  for(unsigned k = 0; k<simu1->GetEntries(); ++k){//simu1 and simu2 have the same number of entries

    simu1->GetEntry(k);
    simu2->GetEntry(k);
    
    constants::adaptUnits(runLength, power1, power2);
    
    while(runData == runSimu && i < data->GetEntries()-1){
      
      neutrinos.emplace_back(Particle(currentEnergy));
      data->GetEntry(++i);
    
    }
    
    reactor1.setPower(power1);
    reactor1.setFuel(Fuel(f235U_1,f238U_1,f239Pu_1,f241Pu_1));
    reactor2.setPower(power2);
    reactor2.setFuel(Fuel(f235U_2,f238U_2,f239Pu_2,f241Pu_2));
    equivalentFuel = (reactor1 + reactor2).getFuel();

    experiment.addRun(
      Point<double>{equivalentFuel.getFrac("235U"), equivalentFuel.getFrac("238U"), equivalentFuel.getFrac("239Pu"), equivalentFuel.getFrac("241Pu")}, 
      Run<double>(neutrinos, runLength, power1, power2)
    );
    
    neutrinos.resize(0);

  }

  return experiment;
  
}

template <class T, class Container>
Experiment<T> ExperimentExtractor::extractExperiment(double distance1, double distance2, double backgroundRate, const Container& channels){
 
  return extractExperiment<T>(distance1, distance2, backgroundRate, channels.begin(), channels.end());
  
}
#endif