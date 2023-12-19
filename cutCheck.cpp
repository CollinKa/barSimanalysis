/*
note : exactly one hit per layer seems stil rare

9-7-23 all cuts are checked
first 20 file
at least 1 hit per layer:90
exactly one hit per layer:0
cosmic veto:1018
beam panel veto:8370
exactly 1 hit per layer, 4 hits in a line:0
max hit NPE / min hit NPE < 10  :1282
largest calibrated hit time difference is within 15ns:4381


2min / 14 files
https://arxiv.org/pdf/2104.07151.pdf
find a new issue:
timeCheck: should get the time at first and last layer only
did I miss four in line?

count 3 in line (with / wo at least one hit per layer)(done)
add the event counting(done)

adding the cut consecutively(done)
add hits are all in a line( set chan ==1)& I believe I have this cut- check it!
fix time cut(done)
fix ThreeLONEHit（required by the loose cut）
for loose cut use threeIaLine() for along a line instead, it consider the case the of one of layer contain hits not in line with hits at other three layers


9-22 segmentation violation from timecheck when doing withPhoton sim(fixed)

9-22 To- do 
how to write to specific file?
1.create heat map of three in a line chanel distribution (4 layers channel distribution + view from top 16 channels)
2.under three in line concecutive cut, what is npe distribution among channels?
3.record the channel and file # when pass 3 in a line

9-23 withphoton result (panel cut at the end) finished
3 in line = 1-> not enough data
need to add npe cut on without photon(do deposit energy)
add time cut to without photon(need to modify the code)

afternoon
back to without photon
dont output stuff to outputFile
we only need the interest event
comment out some code for outputFile

9-26 adding code for coverting energy deposit to npe for without photon sim


9-27 there is one issue in panel cut, which is assuming all of deposited energy can be transfer in NPE.
1. change the panel cut by checking nPE not energy 
2. to make both with and without photom sim able to use this cut, the npe is calculated by multiply a converting ratio MeV to NPE. Please check EtoNpe()

adding 6 type of cut flows

for loose cut, extract interest event for 3(4) in line- done! check the previous result in interestEvent_threeInaLine.txt
plot bar NPE,channel,layer,intialtime distribution when a hit with  NPE > 1
plot panel NPE,channel,hit time distribution with NPE > 1


for strict cut, extract event that pass panels concecutive cuts


remove the exactly one hit in 3 or more layers in loose cut flow. and switch it into exactly one hit per layer.
keep the run3 cutflow
all new cutflows(except run3 cut flow) should starts from at least in 1 hit in 3 or 4 layers

10-2
change the file name into dataCut
adding corrected value for bar time distribution
adding corrected value for bar time distribution(for the bar)

The "correct" procedure can be found at https://milliqanelog.asc.ohio-state.edu:8080/MilliQanRun3Analysis/39

10-3 
add the "correct" procedure to timeCutWithoutP()
check 3 different cutflows


10-11 
recreate the individual cut result.

10-12
collect the file and event number for all events after applying cut concecutively
making histogram of channel distribution without applying any cut


10-13 
comment out some code to make it draw the channel distribution histogram only.(postpone)


10-14 
notice an issue for doing panel exlusion. it should be "chan < 67 || chan > 83" not chan <= 67 || chan >= 83


10- 19
(TBD)
adding cut exactly one channel got hit per layer
adding cut at least one channel got hit per layer

10-20
doing with photon analysis and collect NPE distribution across channels

10-28
N hit per layer is N bar got hit in layer. There is a need to change the first two cuts
I notice at least one bar hit per layer is the same as at least one hit in scitillator per layer
So I only need to change the ex one bar hit per layer

counting section for ex 1 bar hit is fix. From "int OneHitPLayResult = cut1.OneHitPLay(myROOTEvent)" to "int OneHitPLayResult =cut1.EX1BarHitPLay(myROOTEvent);"


11-16
adding the the location for previous sim file. The one for when the detector at the center of cavern


11-12 remove script for debugging


12-6 
run3 projection file 640 can be used to check if my simplified cutcheck can yeild the same result.

The deposited energy should be sum over bar by bar for the first two cuts. and then apply the energy cut.

The reason we need to sum the energy is to check whether a particle can deposit energy.
if a hit scater off from the scitillator, the Edep should have two energy, E and -E. Edep is calculated base on the 
kinetic energy. To get the deposit energy, we need to sum up the Edep.
The summing value is zero meaning this particle is reflect or not deposit energy.

there is need to check the following cut method after 1 hit per layer debug is finished.



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

            for (const auto& pair : mapOfEnergy)
            {
                int chanNum = pair.first; 
                double Etot = pair.second; //total deposit energy on a bar
                if (Etot > 0)
                {
                    int layerN = (chanNum-1)/16; //old mapping
                    layer.insert(layerN);
                    channel.insert(hitN);
                }
                
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
        }
        int layS = layer.size(); 

        if (layS == 4){return 1;}
        else {return 0;}
    }

    void MakeChanHistogram(mqROOTEvent* myROOTEvent, TH1F* SumEDistribution0, TH1F* SumEDistribution1, TH1F* SumEDistribution2, TH1F* SumEDistribution3,TH1F* SumEDistribution4,TH1F* SumEDistribution5){
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
            double Esum4layer = 0.0;
            for (int h =0; h < numScintHits; h++)
            {
                hitN = myROOTEvent->GetScintRHits()->at(h)->GetCopyNo();
                double energy = myROOTEvent->GetScintRHits()->at(h)->GetEDep();
                Esum4layer += energy;

                //exclude the veto pannals
                //if (hitN <= 64)
                //{   
                int layerN = (hitN-1)/16; //old mapping
                mapOfEnergy[layerN] += energy;

                //}

            }
            SumEDistribution4->Fill(Esum4layer);
            

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

                //}
                
            }
        
        //}


        //plot for all of kinds of event
        /*
        int numScintHits=myROOTEvent->GetScintRHits()->size(); //number of scitillator get hit in a event
        int hitN;
        double Sumenergy = 0.0;
        for (int h =0; h < numScintHits; h++)
        {
            hitN = myROOTEvent->GetScintRHits()->at(h)->GetCopyNo();
            double energy = myROOTEvent->GetScintRHits()->at(h)->GetEDep();
            
            //exclude the veto pannals
            if (hitN <= 64){Sumenergy += energy;} 
        }
        SumEDistribution5->Fill(Sumenergy);
        */
        




    }

};

//only first two geometry cuts


void cutCheck()
{
    
    int fileNumber = 640;

    
    
    //count the result of applying cut individually
    string basePath4 = "/home/collin/mqSimRun3rootfile/data/Individual";
    string outputPath4 = basePath4 + to_string(fileNumber) + ".txt";
    ofstream outputFile4(outputPath4);
    

    //location of data file
    TString folderName = Form("/home/collin/mqSimRun3rootfile/cosmicdir%d", fileNumber);
    TString fileName = Form("%s/MilliQan.root", folderName.Data());

    //txt for saving interesting event(disable in current test)
    string Filebase = "/home/collin/mqSimRun3rootfile/data/hist";
    string outPut = Filebase + to_string(fileNumber) + ".txt";
    ofstream eventDetail(outPut);

    string basePath5 = "/home/collin/mqSimRun3rootfile/data/EHist";
    string rootFileName = basePath5 + to_string(fileNumber) + ".root";  
    TFile ChanHist(rootFileName.c_str(), "RECREATE");
    TH1F* SumEDistribution0 = new TH1F("E distribution0", "E(sum along layer 0 ) distribution", 120, -600, 600); //energy can reach up to 600MeV, but 0-100MeV is sufficient for see the trend
    TH1F* SumEDistribution1 = new TH1F("E distribution1", "E(sum along layer 1 ) distribution", 120, -600, 600); //energy can reach up to 600MeV, but 0-100MeV is sufficient for see the trend
    TH1F* SumEDistribution2 = new TH1F("E distribution2", "E(sum along layer 2 ) distribution", 120, -600, 600); //energy can reach up to 600MeV, but 0-100MeV is sufficient for see the trend
    TH1F* SumEDistribution3 = new TH1F("E distribution3", "E(sum along layer 3 ) distribution", 120, -600, 600); //energy can reach up to 600MeV, but 0-100MeV is sufficient for see the trend
    TH1F* SumEDistribution4 = new TH1F("E distribution4 layer", "E(sum along 4 layers) distribution", 120, -600, 600);
    TH1F* SumEDistribution5 = new TH1F("E distribution4 layers (not AL1hitpL)", "E(sum along 4 layers ) distribution", 600, -600, 600);
    
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
        if (numScintHits == 0) {
            CutTools cut1;

            //start from the counting for strictShortCutFlow()
            cut1.MakeChanHistogram(myROOTEvent,SumEDistribution0,SumEDistribution1,SumEDistribution2,SumEDistribution3,SumEDistribution4,SumEDistribution5);
            /*
            int Catleast4layOneHitResult = cut1.AL1HitPLay(myROOTEvent);
            if (Catleast4layOneHitResult == 1) {
                AL1HitPlayerCount++;
                eventDetail << "1+PerLay :" << index << "   " << numScintHits << endl;
            }
            */
            

            //comment out for increase processing speed
            //int AL1HitPLayNPE = cut1.AL1HitPLayNPE(myROOTEvent);
            //if (AL1HitPLayNPE == 1) {AL1HitPLayNPECount ++;}  

            //int OneHitPLayResult = cut1.EX1BarHitPLay(myROOTEvent);
            //if (OneHitPLayResult == 1) {exa1HitPLayCount ++;}

                
        }         
  
    }

    cout << fileName << endl;
    //result of applying single cut
    outputFile4 << "totoal events:" << eventCount << endl;
    outputFile4 << "Events with 1+ hit per layer :"<< AL1HitPlayerCount << endl;
    outputFile4 << "Events with 1+ hit per layer (NPE):"<< AL1HitPLayNPECount << endl;
    outputFile4 <<  "Events with exactly 1 hit per layer :"<< exa1HitPLayCount << endl;
    SumEDistribution0->Write();
    SumEDistribution1->Write();
    SumEDistribution2->Write();
    SumEDistribution3->Write();
    SumEDistribution4->Write();
    SumEDistribution5->Write();
    


    outputFile4.close();
    eventDetail.close();
    cout << "nentries:" << nentries << endl;


    return 0; 

}
