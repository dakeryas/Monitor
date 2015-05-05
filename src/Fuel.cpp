#include "Fuel.hpp"

ClassImp(Fuel)

using namespace std;

ostream& operator<<(ostream& output, const Fuel& fuel){
  
  fuel.Print(output);
  return output;
  
}

Fuel getWeighedAverage(Fuel fuel1, const Fuel& fuel2, double power1, double power2, double L1, double L2){
  
  fuel1.getWeighedAverage(fuel2, power1, power2, L1, L2);
  return fuel1;

}

Fuel::Fuel():Fuel(0,0,0,0){
  
}

Fuel::Fuel(double numberOfFissions235U, double numberOfFissions238U, double numberOfFissions239Pu, double numberOfFissions241Pu){
  
  double totalNumberOfFissions = numberOfFissions235U + numberOfFissions238U + numberOfFissions239Pu + numberOfFissions241Pu;
  if(totalNumberOfFissions > 0){
   
    fraction["235U"] = numberOfFissions235U / totalNumberOfFissions;
    fraction["238U"] = numberOfFissions238U / totalNumberOfFissions;
    fraction["239Pu"] = numberOfFissions239Pu / totalNumberOfFissions;
    fraction["241Pu"] = numberOfFissions241Pu / totalNumberOfFissions;
    
  }
  else{
    
    fraction["235U"] = 0;
    fraction["238U"] = 0;
    fraction["239Pu"] = 0;
    fraction["241Pu"] = 0;
    
  }
  
}

Fuel::~Fuel(){

}

Fuel& Fuel::getWeighedAverage(const Fuel& toAdd, double power1, double power2, double L1, double L2){

  double weight1 = power1*pow(L2,2);
  double weight2 = power2*pow(L1,2);
  
  for(auto it = std::make_pair(fraction.begin(), toAdd.fraction.begin()); it.first != fraction.end() && it.second != toAdd.fraction.end(); ++it.first, ++it.second){
    
    it.first->second = it.first->second * weight1 + it.second->second * weight2;
    if(weight1 + weight2 > 0){ //if one core at least is 'on'
      
      it.first->second /= weight1+ weight2;
      
    }
    
  }
  
  return *this;
  
}

double Fuel::getFrac(const string& fracName) const{
  
  try{
    
    return fraction.at(fracName);
    
  }
  catch(...){
    
    std::cout<<fracName<<" is not an element of the fuel."<<endl;
    return 0;
    
  }

}

void Fuel::setFrac(const string& fracName, double fracValue){
  
  try{
    
    fraction.at(fracName) = fracValue;
    
  }
  catch(...){
    
    cout<<fracName<<" is not an element of the fuel."<<endl;
    
  }

}

bool Fuel::isEmpty(){
  
  double totalFractionSum = 0;
  for(auto it = fraction.begin(); it != fraction.end(); ++it) totalFractionSum += it->second;
  return !(totalFractionSum >0);

}

void Fuel::Print(ostream& output) const{
  
  for(auto it = fraction.begin(); it != fraction.end(); ++it) output<<it->first<<" = "<<it->second<<"\t";

}
