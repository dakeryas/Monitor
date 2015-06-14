#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "Reactor.hpp"
#include "Constants.hpp"
#include "ExperimentConverter.hpp"
#include "Binner.hpp"

using namespace constants;

void adaptUnits(double& runLenght, double& power1, double& power2){//convert values to days and GW
  
  runLenght *= time::secondToDay;
  power1 *= energy::MWattToGWatt;
  power2 *= energy::MWattToGWatt;
  
}

void neutrinoRetriever(TTree* data, TTree* simu1, TTree* simu2, double energyThreshold, const char* outname){
  
  int runData;
  double currentEnergy;
  data->SetBranchAddress("RunNumber", &runData);
  data->SetBranchAddress("myPromptEvisID", &(currentEnergy));
  
  int runSimu;
  double runLength, numberOfNeutrinosSimu1, power1, f239Pu_1, f241Pu_1, f235U_1, f238U_1;
  simu1->SetBranchAddress("run", &runSimu);
  simu1->SetBranchAddress("runlength", &runLength);
  simu1->SetBranchAddress("n_nu", &numberOfNeutrinosSimu1);
  simu1->SetBranchAddress("p_th", &power1);
  simu1->SetBranchAddress("f239Pu", &f239Pu_1);
  simu1->SetBranchAddress("f241Pu", &f241Pu_1);
  simu1->SetBranchAddress("f235U", &f235U_1);
  simu1->SetBranchAddress("f238U", &f238U_1);
  double numberOfNeutrinosSimu2, power2, f239Pu_2, f241Pu_2, f235U_2, f238U_2;
  simu2->SetBranchAddress("n_nu", &numberOfNeutrinosSimu2);
  simu2->SetBranchAddress("p_th", &power2);
  simu2->SetBranchAddress("f239Pu", &f239Pu_2);
  simu2->SetBranchAddress("f241Pu", &f241Pu_2);
  simu2->SetBranchAddress("f235U", &f235U_2);
  simu2->SetBranchAddress("f238U", &f238U_2);
  
  std::vector<Particle> neutrinos;
  Reactor reactor1, reactor2,  equivalentReactor;
  reactor1.setDistanceToDetector(distance::L1);
  reactor2.setDistanceToDetector(distance::L2);

  Experiment<double> experiment(distance::L1, distance::L2, backgroundRate::total);
  auto channels = Binner::createBinning(6, 0.28, 0.4);
  experiment.addChannels(channels.begin(), channels.end());
  
  unsigned i = 0;
  data->GetEntry(i);
  
  for(unsigned k = 0; k<simu1->GetEntries(); ++k){//simu1 and simu2 have the same number of entries

    simu1->GetEntry(k);
    simu2->GetEntry(k);
    
    adaptUnits(runLength, power1, power2);
    
    while(runData == runSimu && i < data->GetEntries()-1){
      
      if(currentEnergy > energyThreshold){
	
	neutrinos.emplace_back(Particle(currentEnergy));
	
      }
      data->GetEntry(++i);
    
    }
    
    reactor1.setPower(power1);
    reactor1.setFuel(Fuel(f235U_1,f238U_1,f239Pu_1,f241Pu_1));
    reactor2.setPower(power2);
    reactor2.setFuel(Fuel(f235U_2,f238U_2,f239Pu_2,f241Pu_2));
    equivalentReactor = reactor1 + reactor2;
    
    experiment.addRun(equivalentReactor.getFuel().getFrac("239Pu"), Run(neutrinos.begin(), neutrinos.end(), runLength, power1, power2));
    
    neutrinos.resize(0);

  }

  std::cout<<experiment<<std::endl;
  TFile outfile(outname, "recreate");
  ExperimentConverter::TGraph(experiment).Write("graph");
  
}

void monitor(const std::string& dirname, const std::string& outname){
  
  TFile dataFile((dirname+"/"+"FinalFitIBDTree_DCIII_Data.root").c_str());
  TFile simuFile1((dirname+"/"+"nu_rate_per_run_B1_3rdPub_f.root").c_str());
  TFile simuFile2((dirname+"/"+"nu_rate_per_run_B2_3rdPub_f.root").c_str()); 
  TTree* data = dynamic_cast<TTree*>(dataFile.Get("FinalFitIBDTree"));
  TTree* simu1 = dynamic_cast<TTree*>(simuFile1.Get("nu"));
  TTree* simu2 = dynamic_cast<TTree*>(simuFile2.Get("nu"));
  
  neutrinoRetriever(data, simu1, simu2, 0, outname.c_str());
  
}

int main(int argc, char* argv[]){
  
  if(argc == 3) monitor(argv[1], argv[2]);
  else std::cout<<"Error: you must provide a directory if the reference spectra and an out file name \n";
  return 0;
  
}
