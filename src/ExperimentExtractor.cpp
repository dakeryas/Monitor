#include "ExperimentExtractor.hpp"

ExperimentExtractor::ExperimentExtractor(TTree* data, TTree* simu1, TTree* simu2):data(data),simu1(simu1),simu2(simu2){
  
  data->SetBranchAddress("RunNumber", &runData);
  data->SetBranchAddress("myPromptEvisID", &(currentEnergy));
  
  simu1->SetBranchAddress("run", &runSimu);
  simu1->SetBranchAddress("runlength", &runLength);
  simu1->SetBranchAddress("n_nu", &numberOfNeutrinosSimu1);
  simu1->SetBranchAddress("p_th", &power1);
  simu1->SetBranchAddress("f239Pu", &f239Pu_1);
  simu1->SetBranchAddress("f241Pu", &f241Pu_1);
  simu1->SetBranchAddress("f235U", &f235U_1);
  simu1->SetBranchAddress("f238U", &f238U_1);

  simu2->SetBranchAddress("n_nu", &numberOfNeutrinosSimu2);
  simu2->SetBranchAddress("p_th", &power2);
  simu2->SetBranchAddress("f239Pu", &f239Pu_2);
  simu2->SetBranchAddress("f241Pu", &f241Pu_2);
  simu2->SetBranchAddress("f235U", &f235U_2);
  simu2->SetBranchAddress("f238U", &f238U_2);

}
