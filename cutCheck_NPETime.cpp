/*
1-9 
the goal of this file is used to find the NPE and time cut efficiency

Other than that I am also using this script to do the anlysis geometric cut for with vs without photon data
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
#include "/net/cms26/cms26r0/zheng/barSimulation/WithPhotonUpdateSim/milliQanSim/include/mqROOTEvent.hh"
#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <iostream>
#include <string>

using namespace std;
R__LOAD_LIBRARY(/net/cms26/cms26r0/zheng/barSimulation/WithPhotonUpdateSim/milliQanSim/build/libMilliQanCore.so)


//return 1 means event pass the cut
class CutTools {



public:


    /*
    int simChanTransfer(int chan)
    {
        if (chan == 73) {return 68;}
        if (chan == 74) {return 70;}
        if (chan == 75) {return 69;}
        if (chan == 81) {return 72;}
        if (chan == 82) {return 74;}
        if (chan == 83) {return 73;}
        if (chan == 67) {return 71;}
        if (chan == 68) {return 75;}
        int layerNumber = chan / 216;
        int simChannel = chan % 216;
        if (simChannel <= 4) {return (simChannel + 11)+layerNumber*16 ;}
        if (simChannel <= 12) {return simChannel - 1 + layerNumber*16;}
        if(simChannel <= 16) {return simChannel - 13 + layerNumber*16;}
        else {return -10;}

    }
    */



    //exactly 1 bar hit per layer: only 4 channels got hit & 4 layers got hits in a event
    int EX1BarHitPLay(mqROOTEvent* myROOTEvent){
        int numScintHits=myROOTEvent->GetScintRHits()->size();
        std::set<int> layer;
        std::set<int> channel;
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
            if (hitN <= 64)
            {
                mapOfEnergy[hitN] += energy;
            }

            
        }

        for (const auto& pair : mapOfEnergy)
        {
                int chanNum = pair.first; 
                double Etot = pair.second; //total deposit energy on a bar
                if (Etot > 0)
                {
                    int layerN = (chanNum-1)/16; //old mapping
                    layer.insert(layerN);
                    channel.insert(chanNum);
                }
                
        }

        int layS = layer.size(); 

        int NumberOfchannel = channel.size();

        if ((NumberOfchannel == 4) && (layS == 4)){return 1;}
        else {return 0;}
        
    }



    //AL1HitPLay:at least 1 hit in scitillator per layer(checked)
    int AL1HitPLay(mqROOTEvent* myROOTEvent) {
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
            if (hitN <= 64)
            {
                mapOfEnergy[hitN] += energy;
            }

            
        }

        for (const auto& pair : mapOfEnergy)
        {
                int chanNum = pair.first; 
                double Etot = pair.second; //total deposit energy on a bar
                if (Etot > 0)
                {
                    int layerN = (chanNum-1)/16; //old mapping
                    layer.insert(layerN);

                }
                
        }

        int layS = layer.size(); 

        if (layS == 4){return 1;}
        else {return 0;}
    }

    //create the second algorism to check 1 hit per layer
    int AL1HitPLayAG2(mqROOTEvent* myROOTEvent){
        int numScintHits=myROOTEvent->GetScintRHits()->size(); //number of scitillator get hit in a event
        std::map<int, double> mapOfEnergy;//it provide the summing deposited 
        const int numberOfLayer = 4;
        const double defaultE = 0.0;
        for (int i = 0; i < numberOfLayer; ++i) {mapOfEnergy[i] = defaultE;}
        int hitN;
        int layerN;
        bool HitAtL0 = false;
        bool HitAtL1 = false;
        bool HitAtL2 = false;
        bool HitAtL3 = false;

        for (int h =0; h < numScintHits; h++)
        {
            hitN = myROOTEvent->GetScintRHits()->at(h)->GetCopyNo();
            double energy = myROOTEvent->GetScintRHits()->at(h)->GetEDep();

            //exclude the veto pannals
            if (hitN <= 64)
            {   
                int layerN = (hitN-1)/16; //old mapping
                mapOfEnergy[layerN] += energy;
            }

        }

        for (const auto& pair : mapOfEnergy)
        {
            int layer = pair.first; 
            double Etot = pair.second; //total deposit energy on a bar
            if (Etot > 0.0)
            {
                if (layer == 0){HitAtL0 = true;}
                if (layer == 1){HitAtL1 = true;}
                if (layer == 2){HitAtL2 = true;}
                if (layer == 3){HitAtL3 = true;}
            }
        }

        if (HitAtL0 && HitAtL1 && HitAtL2 && HitAtL3) {return 1;}
        else {return 0;}
    }


    //try to use NPE cut 
    int AL1HitPLayNPE(mqROOTEvent* myROOTEvent) {
        int numPMTHits = myROOTEvent->GetPMTRHits()->size(); //number of scitillator get hit in a event
        std::set<int> layer;

        std::map<int, int> mapOfNPE;//it provide the summing deposited 
        const int numberOfChannel = 64;
        const int Dnpe = 0;
        for (int i = 0; i < numberOfChannel; ++i) {mapOfNPE[i] = Dnpe;}
        int hitN;
        int layerN;

        for (int h =0; h < numPMTHits; h++)
        {
            hitN = myROOTEvent->GetPMTRHits()->at(h)->GetPMTNumber();

            //exclude the veto pannals
            if (hitN <= 64)
            {
                mapOfNPE[hitN] ++;
            }

            
        }

        for (const auto& pair : mapOfNPE)
        {
            int chanNum = pair.first; 
            int NPE = pair.second; //total deposit energy on a bar
            if (NPE > 1)
            {
                int layerN = (chanNum-1)/16; //old mapping
                layer.insert(layerN);
            }
            
        }
        int layS = layer.size(); 

        if (layS == 4){return 1;}
        else {return 0;}
    }



    void MakeChanHistogram(mqROOTEvent* myROOTEvent, TH1F* SumEDistribution0, TH1F* SumEDistribution1, TH1F* SumEDistribution2, TH1F* SumEDistribution3,TH1F* SumEDistribution4,TH1F* layerDistribution){
        //int result = AL1HitPLay(myROOTEvent);
        //int result = AL1HitPLayAG2(myROOTEvent);

        //if (result == 1)
        //{
            int numScintHits=myROOTEvent->GetScintRHits()->size(); //number of scitillator get hit in a event
            std::set<int> layer;

            std::map<int, double> mapOfEnergy;//it provide the summing deposited 
            const int numberOfChannel = 64;
            const double defaultE = 0.0;
            for (int i = 0; i < numberOfChannel; ++i) {mapOfEnergy[i] = defaultE;}
            int hitN;
            int layerN;
            int layerHit = 0; //count the number layer got hit in a event
            double Esum4layer = 0.0;
            bool lay0Hit = false;
            bool lay1Hit = false;
            bool lay2Hit = false;
            bool lay3Hit = false;
            for (int h =0; h < numScintHits; h++)
            {
                hitN = myROOTEvent->GetScintRHits()->at(h)->GetCopyNo();
                double energy = myROOTEvent->GetScintRHits()->at(h)->GetEDep();
                Esum4layer += energy;

                //exclude the veto pannals
                if (hitN <= 64)
                {   
                    int layerN = (hitN-1)/16; //old mapping
                    mapOfEnergy[layerN] += energy;

                }

            }
            SumEDistribution4->Fill(Esum4layer);
            

            for (const auto& pair : mapOfEnergy)
            {
                int layer = pair.first; 
                double Etot = pair.second; //total deposit energy on a bar
                if (Etot > 0.0)
                {
                    if (layer == 0)
                    {
                        SumEDistribution0->Fill(Etot);
                        lay0Hit = true;
                    }
                    if (layer == 1)
                    {
                        SumEDistribution1->Fill(Etot);
                        lay1Hit = true;
                    }
                    if (layer == 2)
                    {
                        SumEDistribution2->Fill(Etot);
                        lay2Hit = true;
                    }
                    if (layer == 3)
                    {
                        SumEDistribution3->Fill(Etot);
                        lay3Hit = true;
                    }

                }
                
            }

            if (lay0Hit) {layerHit ++;}
            if (lay1Hit) {layerHit ++;}
            if (lay2Hit) {layerHit ++;}
            if (lay3Hit) {layerHit ++;}
            layerDistribution->Fill(layerHit);
        
        //}

    }

};

//only first two geometry cuts


void cutCheck()
{
    
    int fileNumber = 1;

    
    
    //count the result of applying cut individually
    string basePath4 = "/net/cms26/cms26r0/zheng/barSimulation/newRepoSwap/debug2/barSimanalysis/result/Individual";
    string outputPath4 = basePath4 + to_string(fileNumber) + ".txt";
    ofstream outputFile4(outputPath4);
    

    //location of data file
    TString folderName = Form("/net/cms27/cms27r0/schmitz/4SimMuon/cosmicdir%d", fileNumber);
    TString fileName = Form("%s/MilliQan.root", folderName.Data());

    //txt for saving interesting event(disable in current test)
    //string Filebase = "/net/cms26/cms26r0/zheng/barSimulation/newRepoSwap/debug2/barSimanalysis/result/hist";
    //string outPut = Filebase + to_string(fileNumber) + ".txt";
    //ofstream eventDetail(outPut);

    //string basePath5 = "/net/cms26/cms26r0/zheng/barSimulation/newRepoSwap/debug2/barSimanalysis/result/EHist";
    //string rootFileName = basePath5 + to_string(fileNumber) + ".root";  
    //TFile ChanHist(rootFileName.c_str(), "RECREATE");
    TH1F* SumEDistribution0 = new TH1F("E distribution0", "E(sum along layer 0 ) distribution", 120, -600, 600); //energy can reach up to 600MeV, but 0-100MeV is sufficient for see the trend
    TH1F* SumEDistribution1 = new TH1F("E distribution1", "E(sum along layer 1 ) distribution", 120, -600, 600); //energy can reach up to 600MeV, but 0-100MeV is sufficient for see the trend
    TH1F* SumEDistribution2 = new TH1F("E distribution2", "E(sum along layer 2 ) distribution", 120, -600, 600); //energy can reach up to 600MeV, but 0-100MeV is sufficient for see the trend
    TH1F* SumEDistribution3 = new TH1F("E distribution3", "E(sum along layer 3 ) distribution", 120, -600, 600); //energy can reach up to 600MeV, but 0-100MeV is sufficient for see the trend
    TH1F* SumEDistribution4 = new TH1F("E distribution4 layer", "E(sum along 4 layers) distribution", 120, -600, 600);
    TH1F* layerDistribution = new TH1F("layerDistribution", "layerDistribution", 5, 0, 5);
    
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
        //if (numScintHits == 0) {
            CutTools cut1;

            //start from the counting for strictShortCutFlow()
            //cut1.MakeChanHistogram(myROOTEvent,SumEDistribution0,SumEDistribution1,SumEDistribution2,SumEDistribution3,SumEDistribution4,layerDistribution);
            
            
            int Catleast4layOneHitResult = cut1.AL1HitPLay(myROOTEvent);
            //int Catleast4layOneHitResult = cut1.AL1HitPLayAG2(myROOTEvent);
            if (Catleast4layOneHitResult == 1) {
                AL1HitPlayerCount++;
                //eventDetail << "1+PerLay :" << index << "   " << numScintHits << endl;
            }
            
            

            //comment out for increase processing speed
            int AL1HitPLayNPE = cut1.AL1HitPLayNPE(myROOTEvent);
            if (AL1HitPLayNPE == 1) {AL1HitPLayNPECount ++;}  

            int OneHitPLayResult = cut1.EX1BarHitPLay(myROOTEvent);
            if (OneHitPLayResult == 1) {exa1HitPLayCount ++;}

                
        //}         
  
    }

    cout << fileName << endl;
    //result of applying single cut
    outputFile4 << "totoal events:" << eventCount << endl;
    outputFile4 << "Events with 1+ hit per layer :"<< AL1HitPlayerCount << endl;
    outputFile4 << "Events with 1+ hit per layer (NPE):"<< AL1HitPLayNPECount << endl;
    outputFile4 <<  "Events with exactly 1 hit per layer :"<< exa1HitPLayCount << endl;
    //SumEDistribution0->Write();
    //SumEDistribution1->Write();
    //SumEDistribution2->Write();
    //SumEDistribution3->Write();
    //SumEDistribution4->Write();
    //layerDistribution->Write();
    


    outputFile4.close();
    //eventDetail.close();
    cout << "nentries:" << nentries << endl;


    return 0; 

}