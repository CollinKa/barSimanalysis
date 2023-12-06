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
TString fileDir = "/net/cms26/cms26r0/zheng/barSimulation/barWithPhotonUpdate/BARcosmic21/MilliQan.root";

//return 1 means event pass the cut
class CutTools {
public:
    //exactly 1 bar hit per layer: only 4 channels got hit & 4 layers got hits in a event
    int EX1BarHitPLay(mqROOTEvent* myROOTEvent){
        int numScintHits=myROOTEvent->GetScintRHits()->size();
        std::vector<int> layerListV;
        std::set<int> channel;
        int hitN;
        int layerN;

        for (int h =0; h < numScintHits; h++)
        {
            hitN = myROOTEvent->GetScintRHits()->at(h)->GetCopyNo();
            double energy = myROOTEvent->GetScintRHits()->at(h)->GetEDep();
            
            //cout << numScintHits  << "   "<< hitN <<  "    " << energy << endl;
            //exclude the veto pannals
            if ((hitN < 67 || hitN > 83) && (energy > 0)){
                //convert scitillator number into layer number
                layerN = hitN/216;
                layerListV.push_back(layerN);
                channel.insert(hitN);
            }
        }

        // Convert the vector to a set
        std::set<int> layerListS(layerListV.begin(), layerListV.end());
        int layS = layerListS.size(); 

        int NumberOfchannel = channel.size();

        if ((NumberOfchannel == 4) && (layS == 4)){return 1;}
        else {return 0;}
        
    }


    int EX1BarHitPLayNOE(mqROOTEvent* myROOTEvent){
        int numScintHits=myROOTEvent->GetScintRHits()->size();
        std::vector<int> layerListV;
        std::set<int> channel;
        int hitN;
        int layerN;

        for (int h =0; h < numScintHits; h++)
        {
            hitN = myROOTEvent->GetScintRHits()->at(h)->GetCopyNo();
            double energy = myROOTEvent->GetScintRHits()->at(h)->GetEDep();
            
            //cout << numScintHits  << "   "<< hitN <<  "    " << energy << endl;
            //exclude the veto pannals
            if ((hitN < 67 || hitN > 83) && (energy > 0 || energy < 0)){
                //convert scitillator number into layer number
                layerN = hitN/216;
                layerListV.push_back(layerN);
                channel.insert(hitN);
            }
        }

        // Convert the vector to a set
        std::set<int> layerListS(layerListV.begin(), layerListV.end());
        int layS = layerListS.size(); 

        int NumberOfchannel = channel.size();

        if ((NumberOfchannel == 4) && (layS == 4)){return 1;}
        else {return 0;}
        
    }

    //AL1HitPLay:at least 1 hit in scitillator per layer(checked)
    int AL1HitPLay(mqROOTEvent* myROOTEvent) {
        int numScintHits=myROOTEvent->GetScintRHits()->size(); //number of scitillator get hit in a event
        std::set<int> layerList;
        int hitN;
        int layerN;

        for (int h =0; h < numScintHits; h++)
        {
            hitN = myROOTEvent->GetScintRHits()->at(h)->GetCopyNo();
            double energy = myROOTEvent->GetScintRHits()->at(h)->GetEDep();

            //exclude the veto pannals
            if ((hitN < 67 || hitN > 83) && (energy > 0)){
                //convert scitillator number into layer number
                layerN = hitN/216;
                layerList.insert(layerN);
            }
                

        }
        int layS = layerList.size();
        if (layS == 4) {return 1;}
        else {return 0;}
    }



};

//only geometry cuts





void cutCheck()
{
    
    int fileNumber = 2;
    


    //count the result of applying cut individually
    //string basePath4 = "/net/cms26/cms26r0/zheng/barSimulation/withOutPhotonAnalysis/resultWithoutPhoton/Individual";
    string basePath4 = "/net/cms26/cms26r0/zheng/barSimulation/newRepoSwap/result/result";
    //string basePath4 = "/net/cms26/cms26r0/zheng/barSimulation/newRepoSwap/debug/testfolder/Individual";
    string outputPath4 = basePath4 + to_string(fileNumber) + ".txt";
    ofstream outputFile4(outputPath4);

    

    //txt for saving interesting event(disable in current test)
    //string Filebase = "/net/cms26/cms26r0/zheng/barSimulation/withPhotonAnalysis/resultsWithPhoton/hist";
    //string Filebase = "/net/cms26/cms26r0/zheng/barSimulation/withOutPhotonAnalysis/resultWithoutPhoton/hist";
    string Filebase = "/net/cms26/cms26r0/zheng/barSimulation/newRepoSwap/result/hist";
    //string Filebase = "/net/cms26/cms26r0/zheng/barSimulation/newRepoSwap/debug/testfolder/hist";
    string outPut = Filebase + to_string(fileNumber) + ".txt";
    ofstream eventDetail(outPut);
    

    //location of data file
    TString folderName = Form("/net/cms26/cms26r0/zheng/barSimulation/newRepoSwap/rootFiles/BARcosmic%d", fileNumber);
    //TString folderName = Form("/net/cms26/cms26r0/zheng/barSimulation/barWithoutPhoton/BARcosmic%d", fileNumber);
    //TString folderName = Form("/net/cms27/cms27r0/schmitz/4SimMuon/cosmicdir%d", fileNumber);
    TString fileName = Form("%s/MilliQan.root", folderName.Data());
    
    TChain ch("Events");
    ch.Add(fileName);
     
    mqROOTEvent* myROOTEvent = new mqROOTEvent();
    ch.SetBranchAddress("ROOTEvent", &myROOTEvent);
    Long64_t nentries=ch.GetEntries();

    //count how many event pass an individual cut
    int AL1HitPlayerCount = 0;

    int AL1HitPlayerCountNoE = 0;
    
    int exa1HitPLayCount = 0;


    int eventCount = nentries;
    eventDetail << "totoal events:" << eventCount << endl;

    for(int index = 0; index < nentries; index++){
        ch.GetEntry(index);
        int numScintHits=myROOTEvent->GetScintRHits()->size();
        if (numScintHits > 0) {
            CutTools cut1;
      
            //start from the counting for strictShortCutFlow()

            int Catleast4layOneHitResult = cut1.AL1HitPLay(myROOTEvent);
            if (Catleast4layOneHitResult == 1) {
                AL1HitPlayerCount++;
                eventDetail << "1+PerLay :" << index << "   " << numScintHits << endl;
            }

            int Catleast4layOneHitResultNoE = cut1.EX1BarHitPLayNOE(myROOTEvent);
            if (Catleast4layOneHitResultNoE == 1) {
                AL1HitPlayerCountNoE++;
                eventDetail << "1+PerLay_No_E:" << index << "   " << numScintHits << endl;
            }
            
            //turn it off for reduce process time recently
            //int OneHitPLayResult = cut1.EX1BarHitPLay(myROOTEvent);
            //if (OneHitPLayResult == 1) {exa1HitPLayCount ++;}            
        }
    }

    cout << fileName << endl;
    //result of applying single cut
    outputFile4 << "totoal events:" << eventCount << endl;
    outputFile4 << "Events with 1+ hit per layers :"<< AL1HitPlayerCount << endl;
    outputFile4 << "Events with 1+ (No E)hit per layers :"<< AL1HitPlayerCountNoE << endl;
    
    //outputFile4 <<  "Events with exactly 1 hit per layer :"<< exa1HitPLayCount << endl;

    
    eventDetail.close();
    outputFile4.close();
    

    

    return 0; //does notthing

}