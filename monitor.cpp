#include <vector>
#include <TFile.h>
#include <TTree.h>
#include <TH1D.h>
#include "Particle.hpp"
#include "Reactor.hpp"
#include "Constants.hpp"

using namespace std;
using namespace constants;

void neutrinoRetriever(TTree* data, TTree* simu1, TTree* simu2, const map<string, TH1D>& referenceSpectra, const double energyThreshold, const char* outname){
  
  int runData;
  Particle currentNeutrino;
  data->SetBranchAddress("RunNumber", &runData);
  data->SetBranchAddress("myPromptEvisID", &(currentNeutrino.energy));
  data->SetBranchAddress("myPromptRecoBAMAX", &(currentNeutrino.position.x));
  data->SetBranchAddress("myPromptRecoBAMAY", &(currentNeutrino.position.y));
  data->SetBranchAddress("myPromptRecoBAMAZ", &(currentNeutrino.position.z));
  
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
  
  vector<Particle> neutrinos;
  double neutrinoRateData, neutrinoRateData1, neutrinoRateData2, neutrinoRateSimu, neutrinoRateSimu1, neutrinoRateSimu2;
  TH1D emittedSpectrum1;
  TH1D emittedSpectrum2;
  Reactor reactor1, reactor2,  equivalentReactor;
  reactor1.setDistanceToDetector(distance::L1);
  reactor2.setDistanceToDetector(distance::L2);
  TFile outfile(outname, "recreate");
  TTree* merged = new TTree("monitor", "");
  merged->Branch("run", &runSimu);
  merged->Branch("neutrinos", &neutrinos);
  merged->Branch("neutrinoRateData", &neutrinoRateData);
  merged->Branch("neutrinoRateData1", &neutrinoRateData1);
  merged->Branch("neutrinoRateData2", &neutrinoRateData2);
  merged->Branch("neutrinoRateSimu", &neutrinoRateSimu);
  merged->Branch("neutrinoRateSimu1", &neutrinoRateSimu1);
  merged->Branch("neutrinoRateSimu2", &neutrinoRateSimu2);
  merged->Branch("emittedSpectrum1", &emittedSpectrum1);
  merged->Branch("emittedSpectrum2", &emittedSpectrum2);
  merged->Branch("reactor1", &reactor1);
  merged->Branch("reactor2", &reactor2);
  merged->Branch("reactorEquivalent", &equivalentReactor);

  unsigned i = 0;
  data->GetEntry(i);
  unsigned numberOfNeutrinosAbove;
  double weight1, weight2;
  
  for(unsigned k = 0; k<simu1->GetEntries(); ++k){//simu1 and simu2 have the same number of entries

    simu1->GetEntry(k);
    simu2->GetEntry(k);
    
    weight1 = power1*pow(distance::L2,2);
    weight2 = power2*pow(distance::L1,2);
    
    numberOfNeutrinosAbove = 0;
    while(runData == runSimu && i < data->GetEntries()-1){
      
      if(currentNeutrino.isAboveEnergyThreshold(energyThreshold)){
	
	neutrinos.push_back(currentNeutrino);
	++numberOfNeutrinosAbove;
	
      }
      data->GetEntry(++i);
    
    }
    neutrinoRateData = distance::squaredProduct*numberOfNeutrinosAbove/runLength;
    neutrinoRateSimu = efficiency::global*distance::squaredProduct*(numberOfNeutrinosSimu1 + numberOfNeutrinosSimu2)/runLength;
    if(weight1 > 0){
      
      neutrinoRateData1 = numberOfNeutrinosAbove*numberOfNeutrinosSimu1/(numberOfNeutrinosSimu1+numberOfNeutrinosSimu2)/weight1/runLength;
      neutrinoRateSimu1 = efficiency::global*numberOfNeutrinosSimu1/weight1/runLength;
      
    }
    else{
      
      neutrinoRateData1 = 0;
      neutrinoRateSimu1 = 0;
      
    }
    if(weight2 > 0){
      
      neutrinoRateData2 = numberOfNeutrinosAbove*numberOfNeutrinosSimu2/(numberOfNeutrinosSimu1+numberOfNeutrinosSimu2)/weight2/runLength;
      neutrinoRateSimu2 = efficiency::global*numberOfNeutrinosSimu2/weight2/runLength;
      
    }
    else{
      
      neutrinoRateData2 = 0;
      neutrinoRateSimu2 = 0;
      
    }
    emittedSpectrum1 = f235U_1*referenceSpectra.at("235U")+f238U_1*referenceSpectra.at("238U")+f239Pu_1*referenceSpectra.at("239Pu")+f241Pu_1*referenceSpectra.at("241Pu");
    emittedSpectrum1.Scale(1/weight1/runLength);
    emittedSpectrum2 = f235U_2*referenceSpectra.at("235U")+f238U_2*referenceSpectra.at("238U")+f239Pu_2*referenceSpectra.at("239Pu")+f241Pu_2*referenceSpectra.at("241Pu");
    emittedSpectrum2.Scale(1/weight2/runLength);
    reactor1.setPower(power1);
    reactor1.setFuel(Fuel(f235U_1,f238U_1,f239Pu_1,f241Pu_1));
    reactor2.setPower(power2);
    reactor2.setFuel(Fuel(f235U_2,f238U_2,f239Pu_2,f241Pu_2));
    equivalentReactor = reactor1 + reactor2;
    
    if(weight1 + weight2 > 0){ //if one core at least is 'on'
      
      neutrinoRateData /= weight1 + weight2;
      neutrinoRateSimu /= weight1 + weight2;
    
    }
    else{
      
      neutrinoRateData = 0;
      neutrinoRateSimu = 0;
      
    }
    merged->Fill();
    neutrinos.resize(0);

  }

  merged->Write();
  delete merged;
  
}

void monitor(const string& dirname, const string& outname){
  
  TFile dataFile((dirname+"/"+"FinalFitIBDTree_DCIII_Data.root").c_str());
  TFile simuFile1((dirname+"/"+"nu_rate_per_run_B1_3rdPub_f.root").c_str());
  TFile simuFile2((dirname+"/"+"nu_rate_per_run_B2_3rdPub_f.root").c_str()); 
  TTree* data = dynamic_cast<TTree*>(dataFile.Get("FinalFitIBDTree"));
  TTree* simu1 = dynamic_cast<TTree*>(simuFile1.Get("nu"));
  TTree* simu2 = dynamic_cast<TTree*>(simuFile2.Get("nu"));
  
  TFile spectraFile((dirname+"/"+"spectra.root").c_str());
  map<string, TH1D> referenceSpectra;
  referenceSpectra["235U"] = *dynamic_cast<TH1D*>(spectraFile.Get("spectrum_U235"));
  referenceSpectra["238U"] = *dynamic_cast<TH1D*>(spectraFile.Get("spectrum_U238"));
  referenceSpectra["239Pu"] = *dynamic_cast<TH1D*>(spectraFile.Get("spectrum_Pu239"));
  referenceSpectra["241Pu"] = *dynamic_cast<TH1D*>(spectraFile.Get("spectrum_Pu241"));

  neutrinoRetriever(data, simu1, simu2, referenceSpectra, 0, (outname+".root").c_str());
  
}

int main(int argc, char* argv[]){
  
  if(argc == 3) monitor(argv[1], argv[2]);
  else cout<<"Error: you must provide a directory if the reference spectra and an out file name"<<endl;
  return 0;
  
}
