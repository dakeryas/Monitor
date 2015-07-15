#include "TFile.h"
#include "ExperimentExtractor.hpp"
#include "Converter.hpp"
#include "Binner.hpp"
#include "Simulation.hpp"

void neutrinoRetriever(TTree* data, TTree* simu1, TTree* simu2, const char* outname, const std::vector<Histogram<double, double>>& referenceSpectra){
  
  ExperimentExtractor experimentExtractor(data, simu1, simu2);
  
  Binner<double> binner({Axis<double>(5, 0.44, 0.66), Axis<double>(1, 0.085, 0.091), Axis<double>(5, 0.22, 0.4), Axis<double>(2, 0.03, 0.08)});
  auto experiment = experimentExtractor.extractExperiment<double>(constants::distance::L1, constants::distance::L2, constants::backgroundRate::total, binner.generateBinning());
  experiment.slim();
  std::cout<<experiment<<std::endl;
  
  Simulation<double, double> simulation(constants::distance::average, constants::mixing::th13, constants::squaredMass::delta31, referenceSpectra.begin(), referenceSpectra.end());
  simulation.simulateToMatch(experiment);
  simulation.shiftResultingSpectra(constants::mass::proton - constants::mass::neutron  + constants::mass::electron);//convert the neutrino's energy to the positron's energy + electron's annihilation mass
//   std::cout<<simulation<<std::endl;
  
  TFile outfile(outname, "recreate");
  auto rate = Converter::toTH1(experiment.getRateHistogram<double,double>().integrateDimensions({0,1,3}));
  if(rate){
  
    if(rate->GetDimension() == 1){
      
      rate->SetLineColor(kBlue);
      rate->SetLineWidth(2);
      rate->Fit("pol1", "NEXO");
      
    }
    else rate->SetFillColor(kBlue);
    
    rate->Write("rate");
    
  }
  
  Point<double> referenceConfiguration{0.575, 0.0875, 0.274, 0.0425};//U5,U8,PU9,PU41
  
  TGraphErrors spectrum;
  auto normaliser = simulation.getResulingSpectrum(referenceConfiguration);
  unsigned index{};
  for(auto pair : simulation.getResults()){

    pair.second /= normaliser;
    spectrum = Converter::toTGraph(pair.second);
    spectrum.SetName(("spectrum_simu_"+std::to_string(index)).c_str());
    spectrum.SetTitle(Converter::toString(pair.first.getCenter()).c_str());
    spectrum.SetLineColor(index + 1);
    spectrum.SetLineWidth(2);
    spectrum.Write();

    ++index;
    
  }
  
  Histogram<double,double> energyHistogram;
  binner.setAxes(4, 0., 8);
  auto energyChannels = binner.generateBinning();
  normaliser = experiment.getNeutrinoSpectrum<double,double>(referenceConfiguration, energyChannels);
  index = 0;
  for(const auto& pair : experiment){
   
    energyHistogram = pair.second.getScaledNeutrinoSpectrum<double,double>(experiment.getDistance1(), experiment.getDistance2(), experiment.getBackgroundRate(),energyChannels);
    energyHistogram /= normaliser;
    spectrum = Converter::toTGraph(energyHistogram);
    spectrum.SetName(("spectrum_data_"+std::to_string(index)).c_str());
    spectrum.SetTitle(Converter::toString(pair.first.getCenter()).c_str());
    spectrum.SetLineColor(index + 1);
    spectrum.SetLineWidth(2);
    spectrum.Write();
  
    ++index;
  
  }
  
}

void monitor(const std::string& dirname, const std::string& outname){
  
  Tracer::setGlobalVerbosity(Tracer::Error);//set the static variable
  
  TFile dataFile((dirname+"/"+"FinalFitIBDTree_DCIII_Data.root").c_str());
  TFile simuFile1((dirname+"/"+"nu_rate_per_run_B1_3rdPub_f.root").c_str());
  TFile simuFile2((dirname+"/"+"nu_rate_per_run_B2_3rdPub_f.root").c_str());
  
  TFile referenceSpectraFile((dirname+"/"+"reference.root").c_str());
  std::vector<Histogram<double, double>> referenceSpectra(4);
  referenceSpectra[0] = Converter::toHistogram<double,double>(*dynamic_cast<TH1D*>(referenceSpectraFile.Get("U235")));
  referenceSpectra[1] = Converter::toHistogram<double,double>(*dynamic_cast<TH1D*>(referenceSpectraFile.Get("U238")));
  referenceSpectra[2] = Converter::toHistogram<double,double>(*dynamic_cast<TH1D*>(referenceSpectraFile.Get("Pu239")));
  referenceSpectra[3] = Converter::toHistogram<double,double>(*dynamic_cast<TH1D*>(referenceSpectraFile.Get("Pu241")));
  
  TTree* data = dynamic_cast<TTree*>(dataFile.Get("FinalFitIBDTree"));
  TTree* simu1 = dynamic_cast<TTree*>(simuFile1.Get("nu"));
  TTree* simu2 = dynamic_cast<TTree*>(simuFile2.Get("nu"));
  
  neutrinoRetriever(data, simu1, simu2, outname.c_str(), referenceSpectra);
  
}

int main(int argc, char* argv[]){
  
  if(argc == 3) monitor(argv[1], argv[2]);
  else std::cout<<"Error: you must provide a directory for the reference spectra and an out file name \n";
  return 0;
  
}
