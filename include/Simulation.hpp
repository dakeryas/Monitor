#ifndef SIMULATION_H
#define SIMULATION_H

#include "Experiment.hpp"
#include "Constants.hpp"

template <class T, class K>
class Simulation{//class meant to hold runs in the corresponding configuration bin

  double averageDistance;//average distance from the (equivalent) reactor to the detector
  double theta13;//to apply the neutrino oscillation to the neutrino spectrum
  double delta31;//mass to apply the oscillation
  std::vector<Histogram<T,K>> referenceSpectra;//must follow the order of the bin edges in Experiment
  std::map<Bin<T>, Histogram<T,K>> results;//for each configuration/bin in the experiment, compute the simulate spectrum
  
public:
  template <class Iterator>  
  Simulation(double averageDistance, double theta13, double delta31, Iterator beginReferenceSpectra, Iterator endReferenceSpectra);
  Simulation(double averageDistance, double theta13, double delta31, std::initializer_list<Histogram<T,K>> referenceSpectra);
  double getAverageDistance() const;
  double getTheta13() const;
  double getDelta31() const;
  unsigned getAdmissbleConfigurationSize() const;
  const std::map<Bin<T>, Histogram<T,K>>& getResults() const;
  void setAverageDistance(double averageDistance);
  void setTheta13(double theta13);
  void setDelta13(double delta31);
  template <class Iterator>
  void setReferenceSpectra(Iterator beginReferenceSpectra, Iterator endReferenceSpectra);
  void setReferenceSpectra(std::initializer_list<Histogram<T,K>> referenceSpectra);
  void build(const Experiment<T>& experiment);//build the reference results according to the configrations of the experiment
  void applyOscillation();
  void applyCrossSection();
  void scaleCountsTo(const Experiment<T>& experiment);//normalise each spectrum in results to the rate obtained for the corresponding configration in the experiment
  void simulateToMatch(const Experiment<T>& experiment);//apply the oscillation, the cross section, and normalise to the data rates
  void shiftResultingSpectra(const T& shift);//shift all histograms in 'results' by 'shift'
  
};

template <class T, class K>
std::ostream& operator<<(std::ostream& output, const Simulation<T,K>& simulation){
  
  output.precision(4);
  
  for(const auto& pair : simulation.getResults())
    output<<pair.first<<"\n"<<pair.second<<"\n";
  
  return output;
  
}

template <class T, class K>
template <class Iterator> 
Simulation<T,K>::Simulation(double averageDistance, double theta13, double delta31, Iterator beginReferenceSpectra, Iterator endReferenceSpectra):averageDistance(averageDistance),theta13(theta13),delta31(delta31),referenceSpectra(beginReferenceSpectra,endReferenceSpectra){

}

template <class T, class K>
Simulation<T,K>::Simulation(double averageDistance, double theta13, double delta31, std::initializer_list<Histogram<T,K>> referenceSpectra):Simulation(averageDistance,theta13,delta31,referenceSpectra.begin(),referenceSpectra.end()){

}

template <class T, class K>
double Simulation<T,K>::getAverageDistance() const{
  
  return averageDistance;

}

template <class T, class K>
double Simulation<T,K>::getTheta13() const{
  
  return theta13;

}

template <class T, class K>
double Simulation<T,K>::getDelta31() const{
  
  return delta31;

}

template <class T, class K>
unsigned Simulation<T,K>::getAdmissbleConfigurationSize() const{

  return referenceSpectra.size();
  
}

template <class T, class K>
const std::map<Bin<T>, Histogram<T,K>>& Simulation<T,K>::getResults() const{

  return results;
  
}

template <class T, class K>
void Simulation<T,K>::setAverageDistance(double averageDistance){
  
  this->averageDistance = averageDistance;

}

template <class T, class K>
void Simulation<T,K>::setTheta13(double theta13){

  this->theta13 = theta13;
  
}

template <class T, class K>
void Simulation<T,K>::setDelta13(double delta31){
  
  this->delta31 = delta31;

}

template <class T, class K>
template <class Iterator>
void Simulation<T,K>::setReferenceSpectra(Iterator beginReferenceSpectra, Iterator endReferenceSpectra){
  
  referenceSpectra.assign(beginReferenceSpectra, endReferenceSpectra);
  
}

template <class T, class K>
void Simulation<T,K>::setReferenceSpectra(std::initializer_list<Histogram<T,K>> referenceSpectra){
  
  setReferenceSpectra(referenceSpectra.begin(),referenceSpectra.end());
  
}

template <class T, class K>
void Simulation<T,K>::build(const Experiment< T >& experiment){
  
  for(const auto& pairBin : experiment)
    results[pairBin.first] = weigh(pairBin.first.getCenter(), referenceSpectra.begin(), referenceSpectra.end());//weigh reference spectra with the composition

}

template <class T, class K>
void Simulation<T,K>::applyOscillation(){
  
  for(auto& pairHist : results)//all histograms of the results
    for(auto& pairBin : pairHist.second)//apply the oscillation to all bins of each histograms
      pairBin.second *= constants::oscillation(pairBin.first.getEdge(0).getCenter(), averageDistance, theta13, delta31);//binContent * crossSection(binCenter)

}

template <class T, class K>
void Simulation<T,K>::applyCrossSection(){
  
  for(auto& pairHist : results)
    for(auto& pairBin : pairHist.second)
      pairBin.second *= constants::crossSection(pairBin.first.getEdge(0).getCenter());//binContent * crossSection(binCenter)
  
}

template <class T, class K>
void Simulation<T,K>::scaleCountsTo(const Experiment<T>& experiment){

  for(const auto& pairBin : experiment)
    results[pairBin.first].scaleCountsTo(pairBin.second.getNeutrinoRate(experiment.getDistance1(), experiment.getDistance2(), experiment.getBackgroundRate()));
  
}

template <class T, class K>
void Simulation<T,K>::simulateToMatch(const Experiment<T>& experiment){
  
  build(experiment);
  applyOscillation();
  applyCrossSection();
  scaleCountsTo(experiment);
  
}

template <class T, class K>
void Simulation<T,K>::shiftResultingSpectra(const T& shift){
  
  for(auto& pairHist : results)
    pairHist.second.shiftChannels(Point<T>(shift));

}

#endif