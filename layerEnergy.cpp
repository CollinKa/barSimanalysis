/*
this script is created to find number of layer with E > 0 

*/



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
#include "/home/collin/CERN/forkMQSim/milliQanSim/include/mqROOTEvent.hh"
#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <iostream>
#include <string>

using namespace std;
R__LOAD_LIBRARY(/home/collin/CERN/forkMQSim/milliQanSim/build/libMilliQanCore.so)

//return 1 means event pass the cut
class CutTools {



public:



    void MakeChanHistogram(mqROOTEvent* myROOTEvent, TH1F* SumEDistribution0, TH1F* SumEDistribution1, TH1F* SumEDistribution2, TH1F* SumEDistribution3,TH1F* SumEDistribution4){

        int numScintHits=myROOTEvent->GetScintRHits()->size(); //number of scitillator get hit in a event
        std::set<int> layer;

        std::map<int, double> mapOfEnergy;//it provide the summing deposited 
        const int numberOfChannel = 64;
        const double defaultE = 0.0;
        for (int i = 0; i < numberOfChannel; ++i) {mapOfEnergy[i] = defaultE;}
        int hitN;
        int layerN;
        for (int h =0; h < numScintHits; h++)
        {
            hitN = myROOTEvent->GetScintRHits()->at(h)->GetCopyNo();
            double energy = myROOTEvent->GetScintRHits()->at(h)->GetEDep();

            //exclude the veto pannals
            if (hitN < 60 || (hitN > 90 && hitN < 665))
            {   
                int layerN = hitN / 216; //old mapping
                mapOfEnergy[layerN] += energy;

            }

        }

        double Esum4layer = 0.0;

        for (const auto& pair : mapOfEnergy)
        {
            int layer = pair.first; 
            double Etot = pair.second; //total deposit energy on a bar
            //if (Etot > 0.0)
            //{
                if (layer == 0){SumEDistribution0->Fill(Etot);}
                if (layer == 1){SumEDistribution1->Fill(Etot);}
                if (layer == 2){SumEDistribution2->Fill(Etot);}
                if (layer == 3){SumEDistribution3->Fill(Etot);}
                Esum4layer += Etot;

            //}
            
        }
        SumEDistribution4->Fill(Esum4layer);

    }



};

//only first two geometry cuts


void layerEnergy()
{
    
    int fileNumber = 1;
    

    //location of data file
    //TString folderName = Form("/home/collin/mqSimRun3rootfile/cosmicdir%d", fileNumber);
    TString folderName = Form("/home/collin/mqSimWithOldmac/cosmicdir%d", fileNumber);
    TString fileName = Form("%s/MilliQan.root", folderName.Data());



    string basePath5 = "/home/collin/mqSimWithOldmac/data/EHist";
    string rootFileName = basePath5 + to_string(fileNumber) + ".root";  
    TFile ChanHist(rootFileName.c_str(), "RECREATE");
    TH1F* SumEDistribution0 = new TH1F("E distribution0", "E(sum along layer 0 ) distribution", 100, 0, 100); //energy can reach up to 600MeV, but 0-100MeV is sufficient for see the trend
    TH1F* SumEDistribution1 = new TH1F("E distribution1", "E(sum along layer 1 ) distribution", 100, 0, 100); //energy can reach up to 600MeV, but 0-100MeV is sufficient for see the trend
    TH1F* SumEDistribution2 = new TH1F("E distribution2", "E(sum along layer 2 ) distribution", 100, 0, 100); //energy can reach up to 600MeV, but 0-100MeV is sufficient for see the trend
    TH1F* SumEDistribution3 = new TH1F("E distribution3", "E(sum along layer 3 ) distribution", 100, 0, 100); //energy can reach up to 600MeV, but 0-100MeV is sufficient for see the trend
    TH1F* SumEDistribution4 = new TH1F("E distribution4 layer", "E(sum along 4 layers ) distribution", 100, 0, 100);

    
    TChain ch("Events");
    ch.Add(fileName);
     
    mqROOTEvent* myROOTEvent = new mqROOTEvent();
    ch.SetBranchAddress("ROOTEvent", &myROOTEvent);
    Long64_t nentries=ch.GetEntries();

    //count how many event pass an individual cut
    int AL1HitPlayerCount = 0;

    int exa1HitPLayCount = 0;

    int AL1HitPLayNPECount = 0;

    int eventCount = nentries;


    for(int index = 0; index < nentries; index++)
    {
        ch.GetEntry(index);
        int numScintHits=myROOTEvent->GetScintRHits()->size();
        if (numScintHits > 0) {
            CutTools cut1;
            cut1.MakeChanHistogram(myROOTEvent,SumEDistribution0,SumEDistribution1,SumEDistribution2,SumEDistribution3,SumEDistribution4);    
        }         
  
    }

    cout << fileName << endl;

    SumEDistribution0->Write();
    SumEDistribution1->Write();
    SumEDistribution2->Write();
    SumEDistribution3->Write();
    SumEDistribution4->Write();

    return 0; 

}