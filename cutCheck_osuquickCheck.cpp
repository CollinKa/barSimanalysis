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
there is need to separete  this code into cpp(with class), header and main files
making npe distribution (npe vs chan) and NPE distribution(1D)

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
    //AL1HitPLay:at least 1 hit per layer(checked)
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

    //At least three layer got hit
    int ThreeLHit(mqROOTEvent* myROOTEvent) {
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
        if (layS >=3) {return 1;}
        else {return 0;}
    }

    //exactly 1 hit in 3 or more layers 
    
    int ThreeLONEHit(mqROOTEvent* myROOTEvent) {
        std::map<int,int> layerHitMap;
        int numScintHits=myROOTEvent->GetScintRHits()->size();
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
                layerHitMap[layerN]++;
            }

        }

        int hitcout = 0;
        for (const auto& pair : layerHitMap) {
            int layerN = pair.first; //channel
            int hits = pair.second;
            if (hits == 1) {hitcout ++;}

        }
        if (hitcout >= 3) {return 1;}
        else {return 0;}
            

    }



    //exactly one hit per layer
    int OneHitPLay(mqROOTEvent* myROOTEvent) {
        int numScintHits=myROOTEvent->GetScintRHits()->size();
        std::vector<int> layerListV;
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
                layerListV.push_back(layerN);
            }
                

        }

        // Convert the vector to a set
        std::set<int> layerListS(layerListV.begin(), layerListV.end());
        int layS = layerListS.size(); 
        int layV = layerListV.size();

        if ((layS == layV) && (layS == 4)){return 1;}
        else {return 0;}
            

    }

    //cosmic veto
    //a single hit can reach 1 NPE
    int CosVeto(mqROOTEvent* myROOTEvent) {
        int numScintHits=myROOTEvent->GetScintRHits()->size();
        int i = 0;
        for (int h =0; h < numScintHits; h++)
        {
            int hitN = myROOTEvent->GetScintRHits()->at(h)->GetCopyNo();
            double energy = myROOTEvent->GetScintRHits()->at(h)->GetEDep();
            //side panel corresponded copy number
            if ((hitN >=73 && hitN <= 83) && energy>0){
                double NPE = energy * EtoNpe(hitN);
                if (NPE > 1) {return 0;}
            }
        }            
        return 1;
    }

    //beam panel veto(verified)
    //if the sum of edp on front and back pannel >= 1 MeV, then reject it as an cosmic shower event.
    int BeamPveto(mqROOTEvent* myROOTEvent) {
        int numScintHits=myROOTEvent->GetScintRHits()->size();
        int i = 0;
        double npe =0;
        for (int h =0; h < numScintHits; h++)
        {
            int hitN = myROOTEvent->GetScintRHits()->at(h)->GetCopyNo();
            double energy = myROOTEvent->GetScintRHits()->at(h)->GetEDep();
            if ((hitN ==67 || hitN == 68) && energy >= 0){
                npe += energy * EtoNpe(hitN);
            }             
        }
        
        //suppose we treat 100 npe got detected as the event of cosmic shower.Based on the minimum energy deposit of 100 NPE got detected in the simulation, 
        //the minimum edp is 1MeV. 
        //please be aware that the Quntum efficiency doesn't scale to any calibration value. Because there is no calibration for slab detector
        if (npe < 100) {return 1;} //return 1 for not cosmic shower
            
        else {return 0;}
    }


    // exactly 1 hit per layer+ four in aline. This cut can done by combining 4 hits in line at below and ex1HitPLay() at above and checking if all the hits are in a line.
    // using 4*supermoduleNumber + barNumber to check hits along a line.
    // 4*supermoduleNumber + barNumber is unique for each layer. Let's call this number as num
    
    // hits are along a line.(checked)
    int alongALine(mqROOTEvent* myROOTEvent){
        std::set<int> numList;
        int numScintHits=myROOTEvent->GetScintRHits()->size();
        for (int h =0; h < numScintHits; h++){
            int hitN = myROOTEvent->GetScintRHits()->at(h)->GetCopyNo();
            double energy = myROOTEvent->GetScintRHits()->at(h)->GetEDep();
            if (energy > 0){
                int num = hitN%216;
                numList.insert(num);
            }
        }
        int numOfelement = numList.size();
        if (numOfelement == 1) {return 1;}
        else {return 0;}
    }


    //4 in a line & at leaset 1 hit per layer(not being used right now)
    int FourIaLine(mqROOTEvent* myROOTEvent){
        std::vector<std::vector<int>> Pairs;
        std::vector<int> ChanLayerPair;
        std::set<int> chanSet;
        int numScintHits=myROOTEvent->GetScintRHits()->size();
        for (int h =0; h < numScintHits; h++){
            int hitN = myROOTEvent->GetScintRHits()->at(h)->GetCopyNo();
            double energy = myROOTEvent->GetScintRHits()->at(h)->GetEDep();
            int ChanNum = hitN%216;
            if ((energy > 0) && (ChanNum < 67 || ChanNum > 83)){
                int ChanNum = hitN%216;
                int layerN = hitN/216;
                ChanLayerPair.push_back(ChanNum);
                ChanLayerPair.push_back(layerN);
                chanSet.insert(ChanNum);
                Pairs.push_back(ChanLayerPair);
            }
        }
        
        int i = Pairs.size();
        for (const int &chanUnique : chanSet){
            //int fourInlineCount = 0;
            std::set<int> layerSet;
            for (const std::vector<int>& pair : Pairs){
                int chan=pair[0];
                int layer=pair[1];
                if (chan == chanUnique){layerSet.insert(layer);}
                
            }
            if(layerSet.size() == 4) {return 1;}
        }
        return 0;
    }



    //three in a line and exctly one hit on those three in a line channel
    int threeIaLine(mqROOTEvent* myROOTEvent){

        int numScintHits=myROOTEvent->GetScintRHits()->size();
        if (numScintHits > 0) {
            //inside function threeIaLine: 
            std::vector<int>ChanlayerFirst; //record which chanel got hit in the first layer
            std::vector<int>ChanlayerSecond;
            std::vector<int>ChanlayerThird;
            std::vector<int>ChanlayerFourth;
            std::map<int,int> layerHits;
            numScintHits=myROOTEvent->GetScintRHits()->size();
            //cout << numScintHits << endl;
            for (int h =0; h < numScintHits; h++){
                int hitN = myROOTEvent->GetScintRHits()->at(h)->GetCopyNo();
                double energy = myROOTEvent->GetScintRHits()->at(h)->GetEDep();
                int ChanNum = hitN%216;
                if ((energy > 0) && (ChanNum < 67 || ChanNum > 83)){
                    
                    int layerN = hitN/216;
                    if (layerN==0) {ChanlayerFirst.push_back(ChanNum);}
                    if (layerN==1) {ChanlayerSecond.push_back(ChanNum);}
                    if (layerN==2) {ChanlayerThird.push_back(ChanNum);}
                    if (layerN==3) {ChanlayerFourth.push_back(ChanNum);}
                    layerHits[layerN]++;
                }

            }
            
            if (layerHits.size() >=3) {
                //cout << "start new event" << endl;
                std::vector<int>ChanCheck;
                for (const auto& pair : layerHits){
                    int layer = pair.first;

                    
                    int hits = pair.second;
                    int chan = -1;
                    if (hits == 1){
                        if (layer==0){chan = ChanlayerFirst[0];}
                        if (layer==1){chan = ChanlayerSecond[0];}
                        if (layer==2){chan = ChanlayerThird[0];}
                        if (layer==3){chan = ChanlayerFourth[0];}
                        //channel number starts from 1
                        if (chan>0) {ChanCheck.push_back(chan);}
                        
                    }
                }

                if (ChanCheck.size()>=3){
                    for (int i = 1; i <= 16; ++i) {
                        int threeInlineCount = 0;
                        for (int chan : ChanCheck){
                            if(chan == i) {threeInlineCount++;}
                        }
                        if (threeInlineCount >=3) {
                            //cout << "event pass" << endl;
                            return 1;
                        }
                    }
                }
                    
            }
            
            //cout << "event failed" << endl;
            
        }
        return 0;

    
    }
        
    
    //3 in a line(one hit per layer). Not being used now
    int threeIaLineOHitperLayer(mqROOTEvent* myROOTEvent){
        std::set<int> numList;
        std::vector<double> numVec;
        int numScintHits=myROOTEvent->GetScintRHits()->size();
        for (int h =0; h < numScintHits; h++){
            int hitN = myROOTEvent->GetScintRHits()->at(h)->GetCopyNo();
            double energy = myROOTEvent->GetScintRHits()->at(h)->GetEDep();
            if ((energy > 0) && (hitN < 67 || hitN > 83)){
                int num = hitN%216;
                numList.insert(num);
                numVec.push_back(num);
                
            }
        }
        int firstChan=numVec[0];
        int numberCount = 0;
        for (int i = 0; i < numVec.size(); i++) {
            if (numVec[i] == firstChan) {numberCount++;}
        }
        int result1 = 0;
        int result2 = 0;
        int result3 = 0;
        int numOfelement = numList.size();
        if (numOfelement == 2) {result1 = 1;}
        if ((numberCount == 1) || (numberCount == 3)) {result2 = 1;}
        if (numVec.size()==4) {result3 = 1;}

        int finalResult = result3*result2*result1;
        if (finalResult==1) {return 1;}
        else {return 0;}


    }

    //converting deposited energy into npe(for without photon sim only) the result is not precise!
    //this section of code comes from flatlight.py created by Ryan
    double EtoNpe(int chanNum){
        double simToDataScale = 0.68; //7.5/11
        double EquivalentNpeRatio = 0; //in unit npe/MeV
        if (chanNum == 67 || chanNum == 68){EquivalentNpeRatio = 110.2*simToDataScale;}
        if (chanNum == 73 ||  chanNum == 74 || chanNum == 75 || chanNum == 81 || chanNum == 82 || chanNum == 83){
            EquivalentNpeRatio = 28.7 * simToDataScale;
        }
        else {EquivalentNpeRatio = 331.2 * simToDataScale;}
        return EquivalentNpeRatio;
    }


    //npe CUT
    //determine if a photon is being detected
    //chanNum is the pmt number in simulation
    //Pd:possibility of being detected.
    //Pd = (source peak/spe peak ratio in calibration) /  (source peak/spe peak ratio in simulation)  
    // simRatio: ratio of source peak/spe peak
    //checked!
    int NPEdetect(int chanNum){
        double simRatio = 11.;
        double randNum = (rand()/RAND_MAX); //randorm number from 0 to 1


        //chan 0 in cavern
        if (chanNum == 13){
            double Pd = 9.7/simRatio;
            if (randNum <Pd) {return 1;}//being detected
            else {return 0;}//not being detected
        }

        //chan 1
        if (chanNum == 14){
            double Pd = 8.4/simRatio;
            if (randNum <Pd) {return 1;}//being detected
            else {return 0;}//not being detected
        }

        //chan2
        if (chanNum == 15){
            double Pd = 5.0/simRatio;
            if (randNum<Pd) {return 1;}//being detected
            else {return 0;}//not being detected
        }
        //chan3
        if (chanNum == 16){
            double Pd = 3.6/simRatio;
            if (randNum<Pd) {return 1;}//being detected
            else {return 0;}//not being detected
        }

        //chan 4
        if (chanNum == 5){
            double Pd = 8.2/simRatio;
            if (randNum<Pd) {return 1;}//being detected
            else {return 0;}//not being detected
        }

        //chan 5
        if (chanNum == 6){
            double Pd = 8.2/simRatio;
            if (randNum<Pd) {return 1;}//being detected
            else {return 0;}//not being detected
        }


        //chan 6
        if (chanNum == 7){
            double Pd = 6.8/simRatio;
            if (randNum<Pd) {return 1;}//being detected
            else {return 0;}//not being detected
        }


        //chan 7
        if (chanNum == 8){
            double Pd = 6.8/simRatio;
            if (randNum<Pd) {return 1;}//being detected
            else {return 0;}//not being detected
        }

        //chan 8
        if (chanNum == 9){
            double Pd = 8.7/simRatio;
            if (randNum<Pd) {return 1;}//being detected
            else {return 0;}//not being detected
        }
        //chan 9
        if (chanNum == 10){
            double Pd = 8.7/simRatio;
            if (randNum<Pd) {return 1;}//being detected
            else {return 0;}//not being detected
        }
        //chan 10
        if (chanNum == 11){
            double Pd = 6.4/simRatio;
            if (randNum<Pd) {return 1;}//being detected
            else {return 0;}//not being detected
        }
        //chan 11
        if (chanNum == 12){
            double Pd = 7.5/simRatio;
            if (randNum<Pd) {return 1;}//being detected
            else {return 0;}//not being detected
        }
        //chan 12
        if (chanNum == 1){
            double Pd = 8.7/simRatio;
            if (randNum<Pd) {return 1;}//being detected
            else {return 0;}//not being detected
        }
        //chan 13
        if (chanNum == 2){
            double Pd = 11.8/simRatio;
            if (randNum<Pd) {return 1;}//being detected
            else {return 0;}//not being detected
        }
        //chan 14
        if (chanNum == 3){
            double Pd = 9.7/simRatio;
            if (randNum<Pd) {return 1;}//being detected
            else {return 0;}//not being detected
        }
        //chan 15
        if (chanNum == 4){
            double Pd = 8.1/simRatio;
            if (randNum<Pd) {return 1;}//being detected
            else {return 0;}//not being detected
        }

        //layer 2(cal value unfinsihed)

        //chan 16
        if (chanNum == (13 + 216) ){
            double Pd = 8.6/simRatio;
            if (randNum<Pd) {return 1;}//being detected
            else {return 0;}//not being detected
        }
        //chan 17
        if (chanNum == (14 + 216)){
            double Pd = 9.1/simRatio;
            if (randNum<Pd) {return 1;}//being detected
            else {return 0;}//not being detected
        }
        //chan 18
        if (chanNum == (15 + 216)){
            double Pd = 6.4/simRatio;
            if (randNum<Pd) {return 1;}//being detected
            else {return 0;}//not being detected
        }
        //chan 19
        if (chanNum == (16 + 216)){
            double Pd = 7.5/simRatio;
            if (randNum<Pd) {return 1;}//being detected
            else {return 0;}//not being detected
        }
        //chan 20
        if (chanNum == (5+216)){
            double Pd = 9.8/simRatio;
            if (randNum<Pd) {return 1;}//being detected
            else {return 0;}//not being detected
        }
        //chan 21
        if (chanNum == (6+216)){
            double Pd = 8.8/simRatio;
            if (randNum<Pd) {return 1;}//being detected
            else {return 0;}//not being detected
        }
        //chan 22(channel has issue)
        if (chanNum == (7+216)){
            double Pd = simRatio/simRatio;
            if (randNum<Pd) {return 1;}//being detected
            else {return 0;}//not being detected
        }
        //chan 23
        if (chanNum == (8+216)){
            double Pd = 6.6/simRatio;
            if (randNum<Pd) {return 1;}//being detected
            else {return 0;}//not being detected
        }
        //chan 24
        if (chanNum == (9+216)){
            double Pd = 8.6/simRatio;
            if (randNum<Pd) {return 1;}//being detected
            else {return 0;}//not being detected
        }
        //chan 25
        if (chanNum == (10+216)){
            double Pd = simRatio/simRatio;
            if (randNum<Pd) {return 1;}//being detected
            else {return 0;}//not being detected
        }
        //chan 26
        if (chanNum == (11+216)){
            double Pd = 7.3/simRatio;
            if (randNum<Pd) {return 1;}//being detected
            else {return 0;}//not being detected
        }
        //chan 27
        if (chanNum == (12+216)){
            double Pd = 8.3/simRatio;
            if (randNum<Pd) {return 1;}//being detected
            else {return 0;}//not being detected
        }

        //chan 28
        if (chanNum == (1+216)){
            double Pd = 2.6/simRatio;
            if (randNum<Pd) {return 1;}//being detected
            else {return 0;}//not being detected
        }
        //chan 29
        if (chanNum == (2+216)){
            double Pd = 6.2/simRatio;
            if (randNum<Pd) {return 1;}//being detected
            else {return 0;}//not being detected
        }
        //chan 30
        if (chanNum == (3+216)){
            double Pd = 7.0/simRatio;
            if (randNum<Pd) {return 1;}//being detected
            else {return 0;}//not being detected
        }
        //chan 31
        if (chanNum == (4+216)){
            double Pd = 7.2/simRatio;
            if (randNum<Pd) {return 1;}//being detected
            else {return 0;}//not being detected
        }


        //chan 32
        if (chanNum == 9){
            double Pd = 8.7/simRatio;
            if (randNum<Pd) {return 1;}//being detected
            else {return 0;}//not being detected
        }

        //layer 2

        //chan 32
        if (chanNum == (13 + 216*2) ){
            double Pd = 8.7/simRatio;
            if (randNum<Pd) {return 1;}//being detected
            else {return 0;}//not being detected
        }
        //chan 33
        if (chanNum == (14 + 216*2)){
            double Pd = 7.7/simRatio;
            if (randNum<Pd) {return 1;}//being detected
            else {return 0;}//not being detected
        }
        //chan 34
        if (chanNum == (15 + 216*2)){
            double Pd = 6.8/simRatio;
            if (randNum<Pd) {return 1;}//being detected
            else {return 0;}//not being detected
        }
        //chan 35
        if (chanNum == (16 + 216*2)){
            double Pd = 7.8/simRatio;
            if (randNum<Pd) {return 1;}//being detected
            else {return 0;}//not being detected
        }
        //chan 36
        if (chanNum == (5+216*2)){
            double Pd = 8.8/simRatio;
            if (randNum<Pd) {return 1;}//being detected
            else {return 0;}//not being detected
        }
        //chan 37
        if (chanNum == (6+216*2)){
            double Pd = 8.5/simRatio;
            if (randNum<Pd) {return 1;}//being detected
            else {return 0;}//not being detected
        }
        //chan 38
        if (chanNum == (7+216*2)){
            double Pd = 3.5/simRatio;
            if (randNum<Pd) {return 1;}//being detected
            else {return 0;}//not being detected
        }
        //chan 39
        if (chanNum == (8+216*2)){
            double Pd = 8.2/simRatio;
            if (randNum<Pd) {return 1;}//being detected
            else {return 0;}//not being detected
        }
        //chan 40
        if (chanNum == (9+216*2)){
            double Pd = 7.6/simRatio;
            if (randNum<Pd) {return 1;}//being detected
            else {return 0;}//not being detected
        }
        //chan 41
        if (chanNum == (10+216*2)){
            double Pd = 6.8/simRatio;
            if (randNum<Pd) {return 1;}//being detected
            else {return 0;}//not being detected
        }
        //chan 42
        if (chanNum == (11+216*2)){
            double Pd = 9.5/simRatio;
            if (randNum<Pd) {return 1;}//being detected
            else {return 0;}//not being detected
        }
        //chan 43
        if (chanNum == (12+216*2)){
            double Pd = 8.5/simRatio;
            if (randNum<Pd) {return 1;}//being detected
            else {return 0;}//not being detected
        }

        //chan 44
        if (chanNum == (1+216*2)){
            double Pd = 6.3/simRatio;
            if (randNum<Pd) {return 1;}//being detected
            else {return 0;}//not being detected
        }
        //chan 45
        if (chanNum == (2+216*2)){
            double Pd = 6.7/simRatio;
            if (randNum<Pd) {return 1;}//being detected
            else {return 0;}//not being detected
        }
        //chan 46
        if (chanNum == (3+216*2)){
            double Pd = 7.6/simRatio;
            if (randNum<Pd) {return 1;}//being detected
            else {return 0;}//not being detected
        }
        //chan 47
        if (chanNum == (4+216*2)){
            double Pd = 7.9/simRatio;
            if (randNum<Pd) {return 1;}//being detected
            else {return 0;}//not being detected
        }

        //lay3


        //chan 48
        if (chanNum == (13 + 216*3) ){
            double Pd = 9.0/simRatio;
            if (randNum<Pd) {return 1;}//being detected
            else {return 0;}//not being detected
        }
        //chan 49
        if (chanNum == (14 + 216*3)){
            double Pd = 8.3/simRatio;
            if (randNum<Pd) {return 1;}//being detected
            else {return 0;}//not being detected
        }
        //chan 50
        if (chanNum == (15 + 216*3)){
            double Pd = 8.3/simRatio;
            if (randNum<Pd) {return 1;}//being detected
            else {return 0;}//not being detected
        }
        //chan 51
        if (chanNum == (16 + 216*3)){
            double Pd = 6.9/simRatio;
            if (randNum<Pd) {return 1;}//being detected
            else {return 0;}//not being detected
        }
        //chan 52
        if (chanNum == (5+216*3)){
            double Pd = 6.0/simRatio;
            if (randNum<Pd) {return 1;}//being detected
            else {return 0;}//not being detected
        }
        //chan 53
        if (chanNum == (6+216*3)){
            double Pd = 8.0/simRatio;
            if (randNum<Pd) {return 1;}//being detected
            else {return 0;}//not being detected
        }
        //chan 54
        if (chanNum == (7+216*3)){
            double Pd = 5.5/simRatio;
            if (randNum<Pd) {return 1;}//being detected
            else {return 0;}//not being detected
        }
        //chan 55
        if (chanNum == (8+216*3)){
            double Pd = 5.5/simRatio;
            if (randNum<Pd) {return 1;}//being detected
            else {return 0;}//not being detected
        }
        //chan 56
        if (chanNum == (9+216*3)){
            double Pd = 6.6/simRatio;
            if (randNum<Pd) {return 1;}//being detected
            else {return 0;}//not being detected
        }
        //chan 57
        if (chanNum == (10+216*3)){
            double Pd = 8.1/simRatio;
            if (randNum<Pd) {return 1;}//being detected
            else {return 0;}//not being detected
        }
        //chan 58
        if (chanNum == (11+216*3)){
            double Pd = 8.4/simRatio;
            if (randNum<Pd) {return 1;}//being detected
            else {return 0;}//not being detected
        }
        //chan 59
        if (chanNum == (12+216*3)){
            double Pd = 8.7/simRatio;
            if (randNum<Pd) {return 1;}//being detected
            else {return 0;}//not being detected
        }

        //chan 60
        if (chanNum == (1+216*3)){
            double Pd = 9.2/simRatio;
            if (randNum<Pd) {return 1;}//being detected
            else {return 0;}//not being detected
        }
        //chan 61
        if (chanNum == (2+216*3)){
            double Pd = 7.6/simRatio;
            if (randNum<Pd) {return 1;}//being detected
            else {return 0;}//not being detected
        }
        //chan 62
        if (chanNum == (3+216*3)){
            double Pd = 5.9/simRatio;
            if (randNum<Pd) {return 1;}//being detected
            else {return 0;}//not being detected
        }
        //chan 63
        if (chanNum == (4+216*3)){
            double Pd = 8.2/simRatio;
            if (randNum<Pd) {return 1;}//being detected
            else {return 0;}//not being detected
        }

        //for panel detector(no calibration data)
        else{
            double Pd = 7.5/simRatio; //7.5 is the mean value of source/spe ratio
            if (randNum<Pd) {return 1;}//being detected
            else {return 0;}//not being detected
        }
    }
    //withoutPhoton time cut
    int timeCutWithoutP (mqROOTEvent* myROOTEvent){
        int numScintHits = myROOTEvent->GetScintRHits()->size();
        if (numScintHits <= 0) {return 0;}
        std::map<int, vector<double>> mapOfTimes_Bar;
        vector<double> minTimeV;
        for (int h =0; h < numScintHits; h++) {
            int pmtNumber = myROOTEvent->GetScintRHits()->at(h)->GetCopyNo();
            double energy = myROOTEvent->GetScintRHits()->at(h)->GetEDep();
            double hitTime= myROOTEvent->GetScintRHits()->at(h)->GetHitTime();
            if (energy * EtoNpe(pmtNumber) > 1) {
                if(pmtNumber < 67 || pmtNumber > 83) {mapOfTimes_Bar[pmtNumber].push_back(hitTime);}
            }
        }
        if (mapOfTimes_Bar.size() < 2) {return 0;} //lack of data
        for (const auto& pair1 : mapOfTimes_Bar){
                vector<double>TimeList = pair1.second;
                int copyNum = pair1.first;
                auto minTime = min_element(TimeList.begin(), TimeList.end());
                double minTimeValue = *minTime;
                //time for the first and last hit of specific channel
                int layerNum = copyNum/216;
                // the time for particles travel straight across a bar dector and the air gap between two bars is roughly equal to 3.96274 ns
                //Here we are assuming particles travel at c/n in a medium with index of refraction n. The index of refraction for the bar is 1.58, and the gap is mainly air with index of refraction 1
                if (layerNum == 0) {minTimeV.push_back(minTimeValue);}
                if (layerNum == 1) {minTimeV.push_back(minTimeValue-3.96);}
                if (layerNum == 2) {minTimeV.push_back(minTimeValue-7.93);}
                if (layerNum == 3) {minTimeV.push_back(minTimeValue-11.89);}
        }

        auto minTimeR = min_element(minTimeV.begin(), minTimeV.end());
        auto maxTimeR = max_element(minTimeV.begin(), minTimeV.end());
        double RminTime = *minTimeR;
        double RmaxTime = *maxTimeR;

        double TimeDIFF = RmaxTime - RminTime;

        if (TimeDIFF <=15.03) {return 1;}
        else {return 0;}
    }


    //npe cut for converting the energy into npe. max hit NPE / min hit NPE < 10
    int EnergyMinMaxWithoutP(mqROOTEvent* myROOTEvent){
        std::map<int,double> chanNpeMap;
        int numScintHits = myROOTEvent->GetScintRHits()->size();
        if (numScintHits <= 0) {return 0;}
        for (int h =0; h < numScintHits; h++) {
            int pmtNumber = myROOTEvent->GetScintRHits()->at(h)->GetCopyNo();
            double energy = myROOTEvent->GetScintRHits()->at(h)->GetEDep();
            if (energy > 0) {
                double EquivalentNpe = energy * EtoNpe(pmtNumber);
                chanNpeMap[pmtNumber] += EquivalentNpe;
            }
        }

        // Find the most frequent and least frequent elements
        int mostFrequentElement = -1;
        int leastFrequentElement = -1;
        double highestFrequency = -1.0;
        double lowestFrequency = std::numeric_limits<double>::max();

        for (const auto& pair : chanNpeMap) {
            if (pair.second > highestFrequency) {
                mostFrequentElement = pair.first; //channel
                highestFrequency = pair.second; //npe
            }
            if (pair.second < lowestFrequency) {
                leastFrequentElement = pair.first;
                lowestFrequency = pair.second;
            }
        }
        if (leastFrequentElement==mostFrequentElement){
            //cout << "only one channel get hit." << endl;
            return 0;
        }//only one channel get hit.
        if (highestFrequency<(lowestFrequency*10)) {return 1;}
        else {return 0;}
    }

    

    // time difference between hits on first and last layers is within 15ns(not being used)
    int timeCheck(mqROOTEvent* myROOTEvent){
        std::vector<double> timeListFirstLayer;
        std::vector<double> timeListLastLayer;
        int pmtHits = myROOTEvent->GetPMTRHits()->size();
        for (int h =0; h < pmtHits; h++) {
            double hitTime= myROOTEvent->GetPMTRHits()->at(h)->GetFirstHitTime();
            int pmtNumber = myROOTEvent->GetPMTRHits()->at(h)->GetPMTNumber();
            //exclude panel hits
            if (pmtNumber < 67 || pmtNumber > 99) {
                int result = NPEdetect(pmtNumber);
                int layer = pmtNumber / 216;
                if (result == 1){
                    if (layer == 0) {timeListFirstLayer.push_back(hitTime);}
                    if (layer == 3) {timeListLastLayer.push_back(hitTime);}
                
                }
            }
            
            
        }
        if (timeListFirstLayer.size() == 0 || timeListLastLayer.size() == 0) {return 0;}
        auto minTimeLastLay = std::min_element(timeListLastLayer.begin(), timeListLastLayer.end());
        auto maxTimeLastLay = std::max_element(timeListLastLayer.begin(), timeListLastLayer.end());
        auto maxTimeFirstLay = std::max_element(timeListFirstLayer.begin(), timeListFirstLayer.end());
        auto minTimeFirstLay = std::min_element(timeListFirstLayer.begin(), timeListFirstLayer.end());
        int maxTimeLastLayNum = *maxTimeLastLay;
        int minTimeLastLayNum = *minTimeLastLay;
        int maxTimeFirstLayNum = *maxTimeFirstLay;
        int minTimeFirstLayNum = *minTimeFirstLay;
        //case for first layer got hit first
        if (maxTimeLastLayNum-minTimeFirstLayNum < 15.04){return 1;}
        //case for last layer got hit first
        if (maxTimeFirstLayNum-minTimeLastLayNum < 15.04){return 1;}
        else {return 0;}

    } 

    //exactly one channel got hit per layer
    //int ONEChanPerLay(mqROOTEvent* myROOTEvent):

    //at least one channel got hit per layer
    //int ALONEChanPerLay(mqROOTEvent* myROOTEvent):


    int simChanTransfer(int chan){
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

    //make the histogram for the channel distribution without any cut. But we only collect data with energy larger than 1 equivalent NPE
    void MakeChanHistogram(mqROOTEvent* myROOTEvent, TH1F* ChanDistribution){
        std::map<int,double> chanNpeMap;
        int numScintHits = myROOTEvent->GetScintRHits()->size();
        if (numScintHits <= 0) {return;}
        for (int h =0; h < numScintHits; h++) {
            int pmtNumber = myROOTEvent->GetScintRHits()->at(h)->GetCopyNo();
            double energy = myROOTEvent->GetScintRHits()->at(h)->GetEDep();

            if (energy * EtoNpe(pmtNumber) > 1) {
                int realChan = simChanTransfer(pmtNumber);
                ChanDistribution->Fill(realChan);

            }
        }
    }



    //make NPE distribution(channel based not hit based) for the with photon sim
    void MakeNPEHistogram(mqROOTEvent* myROOTEvent, TH1F* NPEDistribution, TH2F* NPEvsChanDistribution){
        std::map<int,double> chanNpeMap;
        int pmtHits = myROOTEvent->GetPMTRHits()->size();
        for (int h =0; h < pmtHits; h++) {
            int pmtNumber = myROOTEvent->GetPMTRHits()->at(h)->GetPMTNumber();
            //exclude panel hits
            if (pmtNumber < 67 || pmtNumber > 99) {
                int result = NPEdetect(pmtNumber);
                if (result == 1){
                    int realpmtNum = simChanTransfer(pmtNumber);
                    chanNpeMap[realpmtNum]+=1;
                }
            }
        }
        int PMTNumber = 0;
        int NPEValue = 0;

        for (const auto& pair : chanNpeMap) {
            PMTNumber = pair.first;
            NPEValue = pair.second;
            NPEDistribution->Fill(NPEValue);
            NPEvsChanDistribution->Fill(PMTNumber,NPEValue);
        }



    }


};

//only geometry cuts





void cutCheck_osuquickCheck()
{
    
    int fileNumber = 1;
    

    //location of data file
    TString folderName = Form("/net/cms26/cms26r0/zheng/barSimulation/barWithPhotonUpdate/BARcosmic%d", fileNumber);
    //TString folderName = Form("/net/cms26/cms26r0/zheng/barSimulation/barWithoutPhoton/BARcosmic%d", fileNumber);
    TString fileName = Form("%s/MilliQan.root", folderName.Data());

    string basePath5 = "/net/cms26/cms26r0/zheng/barSimulation/withPhotonAnalysis/OSU_quickCheck/NPEHist";
    string rootFileName = basePath5 + to_string(fileNumber) + ".root";  
    TFile ChanHist(rootFileName.c_str(), "RECREATE");
    TH1F* NPEDistribution = new TH1F("NPE  distribution", "NPE distribution", 500, 0, 1000);
    TH2F* ChanNPEDistribution = new TH2F("Chan NPE  distribution", "chan NPE distribution",80,0,80 ,500, 0, 1000);
    ChanNPEDistribution->GetXaxis()->SetTitle("chan");
    ChanNPEDistribution->GetYaxis()->SetTitle("NPE");
    NPEDistribution->GetXaxis()->SetTitle("NPE(channel based)");


    TChain ch("Events");
    ch.Add(fileName);
     
    mqROOTEvent* myROOTEvent = new mqROOTEvent();
    ch.SetBranchAddress("ROOTEvent", &myROOTEvent);
    Long64_t nentries=ch.GetEntries();



    for(int index = 0; index < nentries; index++){
        ch.GetEntry(index);
        int numScintHits=myROOTEvent->GetScintRHits()->size();
        if (numScintHits > 0) {
            CutTools cut1;
  
            cut1.MakeNPEHistogram(myROOTEvent,NPEDistribution,ChanNPEDistribution);

            
            //disable for the without photon sim 
            /*
            int NPEMinMaxResult = cut1.NPEMinMax(myROOTEvent);
            if (NPEMinMaxResult==1) {NPEMinMaxCount++;}
            int timeCheckResult = cut1.timeCheck(myROOTEvent);
            if (timeCheckResult ==1) {timeCheckCount++;}
            */ 


            //withPhoton sim
            /*
            int CNpeResult = CfourInlineResult * NPEMinMaxResult;
            if (CNpeResult == 1) {CNpeStrictCount ++;}
            int CTimeCutResult = CNpeResult * timeCheckResult;
            if (CTimeCutResult == 1) {CTimecutStrictCount ++;} 
 

            int CCosVetoStrictResult = CosVetoResult * CTimeCutResult;
            if (CCosVetoStrictResult==1) {CCosVetoStrictCount ++;}
            int CBeamVetoStrictRestult = CCosVetoStrictResult*BeamPvetoResult;
            if (CBeamVetoStrictRestult==1) {CBeamVetoStrictCount ++;}
            */
            
        }
    }
    NPEDistribution->Write();
    ChanNPEDistribution->Write();
    ChanHist.Close();

    return 0; //does notthing

}