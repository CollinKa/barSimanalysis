
#include "TCanvas.h"
#include "TTree.h"
#include "TStyle.h"
#include "TFile.h"
#include <iostream>
#include <fstream>
#include "TMath.h"
#include "TChain.h"
//#include "/homes/zheng/milliQanSim/include/mqROOTEvent.hh"
//#include "TGraph.h"
//#include "TVector.h"
//#include "TVectorD.h"
//#include "TVectorF.h"
//#include "TH1.h"
//#include "TH1F.h"
//#include "TString.h"
//#include "TChain.h"
//#include "TMultiGraph.h"
#include "/net/cms26/cms26r0/zheng/barSimulation/WithPhotonUpdateSim/milliQanSim/include/mqROOTEvent.hh"
//#include <iostream>
//#include <vector>
//#include <set>
//#include <map>
#include <iostream>
#include <string>
//
using namespace std;
R__LOAD_LIBRARY(/net/cms26/cms26r0/zheng/barSimulation/WithPhotonUpdateSim/milliQanSim/build/libMilliQanCore.so)








void debugCheck()
{       
        int fileNumber = 2;
        string Filebase = "/net/cms26/cms26r0/zheng/barSimulation/newRepoSwap/debug/testfolder/debugHist";
        string outPut = Filebase + to_string(fileNumber) + ".txt";
        ofstream eventDetail(outPut);






        
        //location of data file
        //TString folderName = Form("/net/cms26/cms26r0/zheng/barSimulation/newRepoSwap/rootFiles/BARcosmic%d", fileNumber);
        TString folderName = Form("/net/cms26/cms26r0/zheng/barSimulation/barWithoutPhoton/BARcosmic%d", fileNumber);

        TString fileName = Form("%s/MilliQan.root", folderName.Data());
        TChain ch("Events");
        ch.Add(fileName);
        mqROOTEvent* myROOTEvent = new mqROOTEvent();
        ch.SetBranchAddress("ROOTEvent", &myROOTEvent);
        Long64_t nentries = ch.GetEntries();
        std::cout <<  nentries << std::endl;
        int numberOfevnets= 0;
        for(int index = 0; index < nentries; index++){
                ch.GetEntry(index);
                int numScintHits=myROOTEvent->GetScintRHits()->size();
                if (numScintHits > 0) 
                {
                        
                        std::set<int> channel;
                        int hitN;
                        int layerN;
                        for (int h =0; h < numScintHits; h++)
                        {
                                hitN = myROOTEvent->GetScintRHits()->at(h)->GetCopyNo();
                                double energy = myROOTEvent->GetScintRHits()->at(h)->GetEDep();
                                //if ((hitN < 67 || ((hitN > 83)&&(hitN<800)) ) && (energy > 0))
                                if ((hitN < 67 || hitN > 83) && (energy > 0))
                                {
                                        layerN = hitN/216;
                                        //cout << layerN << endl;
                                        channel.insert(hitN);
                                }

                        }
                        int NumberOfchannel = channel.size();
                        if (NumberOfchannel == 4){
                                numberOfevnets++;
                                eventDetail << "1+PerLay :" << index << "   " << numScintHits  <<endl;
                        }
                        

                        
                }
        }
        cout << numberOfevnets << endl;
}

