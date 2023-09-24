#include <sstream>
#include "TCanvas.h"
#include "TTree.h"
#include "TStyle.h"
#include "TFile.h"
#include <iostream>
#include <fstream>
#include "TMath.h"
#include "TChain.h"
//#include "/homes/zheng/milliQanSim/include/mqROOTEvent.hh"
#include "TGraph.h"
#include "TVector.h"
#include "TVectorD.h"
#include "TVectorF.h"
#include "TH1.h"
#include "TH1F.h"
#include "TString.h"
#include "TChain.h"
#include "TMultiGraph.h"
#include "/net/cms26/cms26r0/zheng/barSimulation/WithPhotonUpdateSim/milliQanSim/include/mqROOTEvent.hh"
#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <iostream>
#include <string>
using namespace std;
R__LOAD_LIBRARY(/net/cms26/cms26r0/zheng/barSimulation/WithPhotonUpdateSim/milliQanSim/build/libMilliQanCore.so)

//scintillator analysis
TH1F* layer0 =new TH1F("first layer", "channel hit", 17, 0, 17);
TH1F* layer1 =new TH1F("second layer", "channel hit", 17, 0, 17);
TH1F* layer2 =new TH1F("third layer", "channel hit", 17, 0, 17);
TH1F* layer3 =new TH1F("fourth layer", "channel hit", 17, 0, 17);


void createHistogram(int fileNumber,int eventNumber)
{
    TString folderName = Form("/net/cms26/cms26r0/zheng/barSimulation/barWithoutPhoton/BARcosmic%d", fileNumber);
    TString fileName = Form("%s/MilliQan.root", folderName.Data());
    TChain ch("Events");
    ch.Add(fileName);
    mqROOTEvent* myROOTEvent = new mqROOTEvent();
    ch.SetBranchAddress("ROOTEvent", &myROOTEvent);
    Long64_t nentries=ch.GetEntries();
    ch.GetEntry(eventNumber);

    

    int numScintHits=myROOTEvent->GetScintRHits()->size();
    if (numScintHits > 0){
        for (int h =0; h < numScintHits; h++)
        {   
            std::set<int> layerList;
            int layerN;
            int chanN;
            int hitN = myROOTEvent->GetScintRHits()->at(h)->GetCopyNo();
            double energy = myROOTEvent->GetScintRHits()->at(h)->GetEDep();

            //exclude the veto pannals
            if ((hitN <= 67 || hitN >= 83) && (energy > 0)){
                //convert scitillator number into layer number
                layerN = hitN/216;
                chanN = hitN%216;
                layerList.insert(layerN);

                if (layerN == 0){layer0->Fill(chanN);}
                if (layerN == 1){layer1->Fill(chanN);}
                if (layerN == 2){layer2->Fill(chanN);}
                if (layerN == 3){layer3->Fill(chanN);}
            }
                

        }

        
    }
}



void plot()
{   
    TFile outputFile("threeInLineChanHit.root", "RECREATE");
    std::ifstream inputFile("interestEvent_threeInaLine.txt");
    int file = 1;
    std::string line;
    while (std::getline(inputFile, line)) {

        std::string event; 
        int fileNumber, eventNumber;
        std::istringstream iss(line);
        if (iss >> event >> fileNumber >> eventNumber) {

            //std::cout << "event: " << event << std::endl;
            //std::cout << "fileNumber: " << fileNumber << std::endl;
            //std::cout << "eventNumber: " << eventNumber << std::endl;
            if (event == "CTHreeinaline") {createHistogram(fileNumber,eventNumber);}
        }

    }
    //inputFile.Close();
    layer0->Write();
    layer1->Write();
    layer2->Write();
    layer3->Write();
    //outputFile.Close();


}