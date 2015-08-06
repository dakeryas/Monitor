#include "boost/filesystem.hpp"
#include "boost/program_options.hpp"
#include "TFile.h"
#include "ExperimentExtractor.hpp"
#include "Converter.hpp"
#include "Binner.hpp"
#include "Simulation.hpp"

namespace bpo = boost::program_options;

void neutrinoRetriever(TTree* data, TTree* simu1, TTree* simu2, const char* outname, const std::vector<Histogram<double, double>>& referenceSpectra){
  
  Binner<double> binner({Axis<double>(5, 0.44, 0.66), Axis<double>(1, 0.085, 0.091), Axis<double>(5, 0.22, 0.4), Axis<double>(2, 0.03, 0.08)});
  
  ExperimentExtractor experimentExtractor(data, simu1, simu2);//use the simulations to create Fuel bins for the data
  auto experiment = experimentExtractor.extractExperiment<double>(constants::distance::L1, constants::distance::L2, constants::backgroundRate::total, binner.generateBinning());
  experiment.slim();
  std::cout<<experiment<<std::endl;
  std::cout<<"Integrated Experiment:\n"<<experiment.integrateChannels({0,1,3})<<std::endl;
  
  Simulation<double, double> simulation(constants::distance::average, constants::mixing::th13, constants::squaredMass::delta31, referenceSpectra.begin(), referenceSpectra.end());
  simulation.simulateToMatch(experiment);
  simulation.shiftResultingSpectra(constants::mass::proton - constants::mass::neutron  + constants::mass::electron);//convert the neutrino's energy to the positron's energy + electron's annihilation mass
//   std::cout<<simulation<<std::endl;

  TFile outfile(outname, "recreate");
  auto rate = Converter::toTH1(experiment.getRateHistogram<double,Scalar<double>>());
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
  
//   auto normaliser = simulation.getResulingSpectrum(referenceConfiguration);
  unsigned index{};
//   for(auto pair : simulation.getResults()){
// 
//     pair.second /= normaliser;
//     auto spectrum = Converter::toTH1(pair.second);
//     spectrum->SetName(("spectrum_simu_"+std::to_string(index)).c_str());
//     spectrum->SetTitle(Converter::toString(pair.first.getCenter()).c_str());
//     if(index + 1 != 10) spectrum->SetLineColor(index + 1);
//     else spectrum->SetLineColor(kOrange +1);
//     spectrum->SetLineWidth(2);
//     spectrum->Write();
// 
//     ++index;
//     
//   }
  
  Histogram<double,Scalar<double>> energyHistogram;
  binner.setAxes(4, 0., 8);
  auto energyChannels = binner.generateBinning();
  auto normaliser = experiment.getScaledNeutrinoSpectrum<double, Scalar<double>>(referenceConfiguration, energyChannels);
  index = 0;
  for(const auto& pair : experiment){
   
    energyHistogram = pair.second.getScaledNeutrinoSpectrum<double,Scalar<double>>(experiment.getDistance1(), experiment.getDistance2(), experiment.getBackgroundRate(),energyChannels);
//     energyHistogram = pair.second.getNeutrinoSpectrum<double,Scalar<double>>(energyChannels);
//     energyHistogram /= normaliser;
    auto spectrum = Converter::toTH1(energyHistogram);
    spectrum->SetName(("spectrum_data_"+std::to_string(index)).c_str());
    spectrum->SetTitle(Converter::toString(pair.first.getCenter()).c_str());
    if(index + 1 != 10) spectrum->SetLineColor(index + 1);
    else spectrum->SetLineColor(kOrange +1);
    spectrum->SetLineWidth(2);
    spectrum->Write();
  
    ++index;
  
  }
  
}

void monitor(const boost::filesystem::path& dataPath, const boost::filesystem::path& referenceSpectraPath, const std::vector<boost::filesystem::path>& simulationPaths, const boost::filesystem::path& outputPath, Verbose verbose){
  
  Tracer::setGlobalVerbosity(verbose);//set the static variable
  
  TFile dataFile(dataPath.c_str());
  TFile simuFile1(simulationPaths.front().c_str());
  TFile simuFile2(simulationPaths.back().c_str());
  
  TFile referenceSpectraFile(referenceSpectraPath.c_str());
  std::vector<Histogram<double, double>> referenceSpectra(4);
  referenceSpectra[0] = Converter::toHistogram<double,double>(*dynamic_cast<TH1D*>(referenceSpectraFile.Get("U235")));
  referenceSpectra[1] = Converter::toHistogram<double,double>(*dynamic_cast<TH1D*>(referenceSpectraFile.Get("U238")));
  referenceSpectra[2] = Converter::toHistogram<double,double>(*dynamic_cast<TH1D*>(referenceSpectraFile.Get("Pu239")));
  referenceSpectra[3] = Converter::toHistogram<double,double>(*dynamic_cast<TH1D*>(referenceSpectraFile.Get("Pu241")));
  
  TTree* data = dynamic_cast<TTree*>(dataFile.Get("FinalFitIBDTree"));
  TTree* simu1 = dynamic_cast<TTree*>(simuFile1.Get("nu"));
  TTree* simu2 = dynamic_cast<TTree*>(simuFile2.Get("nu"));
  
  neutrinoRetriever(data, simu1, simu2, outputPath.c_str(), referenceSpectra);
  
}

int main(int argc, char* argv[]){
  
  boost::filesystem::path dataPath, referenceSpectraPath, outputPath;
  std::vector<boost::filesystem::path> simulationPaths;
  Verbose verbose;
  
  bpo::options_description optionDescription("Monitor usage");
  optionDescription.add_options()
  ("help,h", "Display this help message")
  ("data,d", bpo::value<boost::filesystem::path>(&dataPath)->required(), "Data tree")
  ("reference,r", bpo::value<boost::filesystem::path>(&referenceSpectraPath)->required(), "Reference spectra file")
  ("simulations,s", bpo::value<std::vector<boost::filesystem::path>>(&simulationPaths)->required()->multitoken(), "Simulation trees")
  ("output,o", bpo::value<boost::filesystem::path>(&outputPath)->required(), "Output file where to save the rate and shape evolution")
  ("verbose,v", bpo::value<Verbose>(&verbose)->default_value(Verbose::Error),"Verbose level (Quiet, Error, Warning, Debug)");;

  bpo::positional_options_description positionalOptions;//to use arguments without "--"
  positionalOptions.add("data", -1);
  
  bpo::variables_map arguments;
  try{
    
    bpo::store(bpo::command_line_parser(argc, argv).options(optionDescription).positional(positionalOptions).run(), arguments);
    
    if(arguments.count("help")){
      
      std::cout<<optionDescription<<std::endl;
      return 0;
      
    }
      
    bpo::notify(arguments);//the arguments are ready to be used
    
  }
  catch(bpo::error& e){
    
    std::cout<<e.what()<<std::endl;
    return 1;
    
  }
  
  if(!boost::filesystem::is_regular_file(dataPath)) std::cout<<"Error: '"<<dataPath<<"' is not a regular file"<<std::endl;
  else if(!boost::filesystem::is_regular_file(referenceSpectraPath)) std::cout<<"Error: '"<<referenceSpectraPath<<"' is not a regular file"<<std::endl;
  else if(simulationPaths.size() != 2) std::cout<<"Error : wrong number of simulation files (2 needed)"<<std::endl;
  else{
    
    for (const auto& file : simulationPaths) if(!boost::filesystem::is_regular_file(file)){
      
      std::cout<<"Error: '"<<file<<"' is not a regular file"<<std::endl;
      return 1;
      
    }
     
    monitor(dataPath, referenceSpectraPath, simulationPaths, outputPath, verbose);
    
  }
  
}
