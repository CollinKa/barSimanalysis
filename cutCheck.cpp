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
TString fileDir = "/net/cms26/cms26r0/zheng/barSimulation/barWithPhotonUpdate/BARcosmic2/MilliQan.root";

//return 1 means event pass the cut
class CutTools {
public:
    //AL1HitPLay:at least 1 hit per layer(checked)
    int AL1HitPLay(mqROOTEvent* myROOTEvent) {
        int numScintHits=myROOTEvent->GetScintRHits()->size(); //number of scitillator get hit in a event
        std::set<int> layerList;
        int hitN;
        int layerN;

        for (int h =0; h < numScintHits; h++)
        {
            hitN = myROOTEvent->GetScintRHits()->at(h)->GetCopyNo();

            //exclude the veto pannals
            if (hitN <= 67 || hitN >= 83){
                //convert scitillator number into layer number
                layerN = hitN/216;
                layerList.insert(layerN);
            }
                

        }
        int layS = layerList.size();
        if (layS == 4) {return 1;}
        else {return 0;}
    }

    //ex1HitPLay:exactly one hit per layer(checked)
    int ex1HitPLay(mqROOTEvent* myROOTEvent) {
        int numScintHits=myROOTEvent->GetScintRHits()->size();
        std::vector<int> layerListV;
        int hitN;
        int layerN;

        for (int h =0; h < numScintHits; h++)
        {
            hitN = myROOTEvent->GetScintRHits()->at(h)->GetCopyNo();

            //exclude the veto pannals
            if (hitN <= 67 || hitN >= 83){
                //convert scitillator number into layer number
                layerN = hitN/216;
                layerListV.push_back(layerN);
            }
                

        }

        // Convert the vector to a set
        std::set<int> layerListS(layerListV.begin(), layerListV.end());
        int layS = layerListS.size(); 
        int layV = layerListV.size();

        if (layS == 4 && layV==4){return 1;}
        else {return 0;}
            

    }

    //cosmic veto(checked)
    int CosVeto(mqROOTEvent* myROOTEvent) {
        int numScintHits=myROOTEvent->GetScintRHits()->size();
        int i = 0;
        for (int h =0; h < numScintHits; h++)
        {
            int hitN = myROOTEvent->GetScintRHits()->at(h)->GetCopyNo();

            //side panel corresponded copy number
            if (hitN >=73 && hitN <= 83){
                i ++;
                return 0;
            }
                

        }
        if (i == 0) {return 1;}
            
        else {return 0;}
    }

    //beam panel veto(not verified)
    //if the sum of edp on front and back pannel >= 1 MeV, then reject it
    int BeamPveto(mqROOTEvent* myROOTEvent) {
        int numScintHits=myROOTEvent->GetScintRHits()->size();
        int i = 0;
        double edp =0;
        for (int h =0; h < numScintHits; h++)
        {
            int hitN = myROOTEvent->GetScintRHits()->at(h)->GetCopyNo();
            double energy = myROOTEvent->GetScintRHits()->at(h)->GetEDep();
            if ((hitN ==67 || hitN == 68) && energy >= 0){
                edp += energy;
            }             
        }
        
        //suppose we treat 100 npe got detected as the event of cosmic shower.Based on the minimum energy deposit of 100 NPE got detected in the simulation, 
        //the minimum edp is 1MeV. 
        //please be aware that the Quntum efficiency doesn't scale to any calibration value. Because there is no calibration for slab detector
        if (edp < 1) {return 1;} //return 1 for not cosmic shower
            
        else {return 0;}
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
        //std:cout <<"index" <<index << std::endl;
        int numScintHits=myROOTEvent->GetScintRHits()->size();
        //std::cout << numScintHits << std::endl;
        if (numScintHits > 0) {
            CutTools cut1;
            /*
            int result = cut1.AL1HitPLay(myROOTEvent);
            if (result > 0){
                
                std::cout <<"result" <<result << std::endl;
            }
            */
            int result = cut1.BeamPveto(myROOTEvent);
            std::cout << result << std::endl;
            
            
        }
    }

}

