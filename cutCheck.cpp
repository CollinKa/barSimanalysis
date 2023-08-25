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

using namespace std;
R__LOAD_LIBRARY(/net/cms26/cms26r0/zheng/barSimulation/WithPhotonUpdateSim/milliQanSim/build/libMilliQanCore.so)
TString fileDir = "/net/cms26/cms26r0/zheng/barSimulation/barWithPhotonUpdate/BARcosmic1/MilliQan.root";

class CutTools {
public:
    int AL1HitPLay(mqROOTEvent* myROOTEvent){
        int numScintHits=myROOTEvent->GetScintRHits()->size();
        int hitN = myROOTEvent->GetScintRHits()->at(0)->GetCopyNo();
        //std::cout << numScintHits << std::endl;
        //std::cout << hitN << std::endl;
        return hitN;
    }

};


void cutCheck()
{
    
    TFile* file = new TFile(fileDir);
    TTree* tree = (TTree*)file->Get("Events;");
    mqROOTEvent* myROOTEvent = new mqROOTEvent();
    tree->SetBranchAddress("ROOTEvent", &myROOTEvent);
    
    Long64_t nentries=tree->GetEntries();
    for(int index = 0; index < nentries; index++){
        tree->GetEntry(index);
        std:cout <<"index" <<index << std::endl;
        int numScintHits=myROOTEvent->GetScintRHits()->size();
        //std::cout << numScintHits << std::endl;
        if (numScintHits > 0) {
            CutTools cut1;
            int result = cut1.AL1HitPLay(myROOTEvent);
            if (result > 0){
                
                std::cout <<"result" <<result << std::endl;
            }
            
        }
    }

}

