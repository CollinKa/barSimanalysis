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
            if ((hitN <= 67 || hitN >= 83) && (energy > 0)){
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
            if ((hitN <= 67 || hitN >= 83) && (energy > 0)){
                //convert scitillator number into layer number
                layerN = hitN/216;
                layerList.insert(layerN);
            }
                

        }
        int layS = layerList.size();
        if (layS >=3) {return 1;}
        else {return 0;}
    }

    //exactly 1 hit in 3 or more layers (not being used)
    //need to rewrite, right now you are doing the exactly one hit among 3 or 4 layer
    //but not considering the case of the more than 1 hit in the last(one of four) layer.
    //use mapping <layer,number of hit in a layer> to fix it !
    //std::map<int,int> layHitMap;
    
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
            if ((hitN <= 67 || hitN >= 83) && (energy > 0)){
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
        if (hitcout == 3) {return 1;}
        else {return 0;}
            

    }



    //each layer that has hit only got one hit
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
            if ((hitN <= 67 || hitN >= 83) && (energy > 0)){
                //convert scitillator number into layer number
                layerN = hitN/216;
                layerListV.push_back(layerN);
            }
                

        }

        // Convert the vector to a set
        std::set<int> layerListS(layerListV.begin(), layerListV.end());
        int layS = layerListS.size(); 
        int layV = layerListV.size();

        if (layS == layV){return 1;}
        else {return 0;}
            

    }

    //cosmic veto(checked)
    int CosVeto(mqROOTEvent* myROOTEvent) {
        int numScintHits=myROOTEvent->GetScintRHits()->size();
        int i = 0;
        for (int h =0; h < numScintHits; h++)
        {
            int hitN = myROOTEvent->GetScintRHits()->at(h)->GetCopyNo();
            double energy = myROOTEvent->GetScintRHits()->at(h)->GetEDep();
            //side panel corresponded copy number
            if ((hitN >=73 && hitN <= 83) && energy>0){
                i ++;
                return 0;
            }
                

        }
        if (i == 0) {return 1;}
            
        else {return 0;}
    }

    //beam panel veto(verified)
    //if the sum of edp on front and back pannel >= 1 MeV, then reject it as an cosmic shower event.
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
            if ((energy > 0) && (ChanNum <= 67 || ChanNum >= 83)){
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
                if ((energy > 0) && (ChanNum <= 67 || ChanNum >= 83)){
                    
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
                    //cout << "layer:" << layer << endl;
                    
                    int hits = pair.second;
                    //cout << "hits:" << hits << endl;
                    int chan = -1;
                    if (hits == 1){
                        if (layer==0){chan = ChanlayerFirst[0];}
                        if (layer==1){chan = ChanlayerSecond[0];}
                        if (layer==2){chan = ChanlayerThird[0];}
                        if (layer==3){chan = ChanlayerFourth[0];}
                        if (chan>0) {ChanCheck.push_back(chan);}
                        //cout << "does it reach here" << endl;
                        
                    }
                }
                //cout << "ChanCheck size:" << ChanCheck.size() << endl;

                if (ChanCheck.size()>=3){
                    //cout << "start new loop" << endl;
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
            if ((energy > 0) && (hitN <= 67 || hitN >= 83)){
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


    //cut for max hit NPE / min hit NPE < 10
    int NPEMinMax(mqROOTEvent* myROOTEvent){
        std::map<int,int> chanHitMap;
        int pmtHits = myROOTEvent->GetPMTRHits()->size();
        for (int h =0; h < pmtHits; h++) {
            int pmtNumber = myROOTEvent->GetPMTRHits()->at(h)->GetPMTNumber();
            int result = NPEdetect(pmtNumber);
            if (result > 0) {
                chanHitMap[pmtNumber]++;
                //cout << "photon is being detected" << endl; //debug
                //cout << pmtNumber << endl;    //debug
            }
        }

        //debug
        //for (const auto& pair : chanHitMap) {
        //cout<< pair.first << "  " << pair.second << endl;
        //}

        // Find the most frequent and least frequent elements
        int mostFrequentElement = -1;
        int leastFrequentElement = -1;
        int highestFrequency = -1;
        int lowestFrequency = std::numeric_limits<int>::max();

        for (const auto& pair : chanHitMap) {
            if (pair.second > highestFrequency) {
                mostFrequentElement = pair.first; //channel
                highestFrequency = pair.second; //number of hit on specific channel
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

    // time difference between hits on first and last layers is within 15ns
    int timeCheck(mqROOTEvent* myROOTEvent){
        std::vector<double> timeListFirstLayer;
        std::vector<double> timeListLastLayer;
        int pmtHits = myROOTEvent->GetPMTRHits()->size();
        for (int h =0; h < pmtHits; h++) {
            double hitTime= myROOTEvent->GetPMTRHits()->at(h)->GetFirstHitTime();
            int pmtNumber = myROOTEvent->GetPMTRHits()->at(h)->GetPMTNumber();
            //exclude panel hits
            if (pmtNumber <= 67 || pmtNumber >= 83) {
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
};


void cutCheck()
{
    
    //TFile* file = new TFile(fileDir);
    //TTree* tree = (TTree*)file->Get("Events;");
    int fileNumber = 1;

    //location of output file
    //txt for counting number of events that pass the cuts
    string basePath  = "/net/cms26/cms26r0/zheng/barSimulation/withPhotonAnalysis/resultsWithPhoton/file";
    //string basePath  = "/net/cms26/cms26r0/zheng/barSimulation/withOutPhotonAnalysis/resultWithoutPhoton/file";
    string outputPath = basePath + to_string(fileNumber) + ".txt";
    ofstream outputFile(outputPath);
    
    //txt for saving interesting event
    string Filebase = "/net/cms26/cms26r0/zheng/barSimulation/withPhotonAnalysis/resultsWithPhoton/hist";
    string outPut = Filebase + to_string(fileNumber) + ".txt";

    ofstream eventDetail(outPut);



    

    //location of data file
    TString folderName = Form("/net/cms26/cms26r0/zheng/barSimulation/barWithPhotonUpdate/BARcosmic%d", fileNumber);
    //TString folderName = Form("/net/cms26/cms26r0/zheng/barSimulation/barWithoutPhoton/BARcosmic%d", fileNumber);
    TString fileName = Form("%s/MilliQan.root", folderName.Data());
    
    TChain ch("Events");
    ch.Add(fileName);
     
    mqROOTEvent* myROOTEvent = new mqROOTEvent();
    ch.SetBranchAddress("ROOTEvent", &myROOTEvent);
    Long64_t nentries=ch.GetEntries();

    int eventCout = 0;//count the number of event after cut
    double eventC = 0;

    int AL1HitPLayCount=0;
    int threeLayHitCount = 0;
    //we only interest about the result of applying the cut concecutively
    //int ex1HitPLayCount=0;
    //int CosVetoCount=0;
    // BeamPvetoCount=0;
    //exactly 1 hit per layer, 4 hits in a line
    //int exa1HitPLayFourCount=0;
    int NPEMinMaxCount=0;
    int timeCheckCount=0;

    //consecutive cut result
    int Cex1HitPLayCount = 0;
    int CCosVetoStrictCount = 0;
    int CBeamVetoStrictCount = 0;
    int CfourInlineCount = 0;
    int CAtleastthreeLayerHitcount = 0;
    int CCosVetolooseCount = 0;
    int CBeamPvetolooseCount = 0;
    int Cexa1HitPLayFourCount = 0;
    int numScintHits = 0;
    int CNpeStrictCount = 0;
    int CNpelooseCount = 0;
    int CTimecutStrictCount = 0;
    int CTimecutlooseCount = 0;


    int eventCount = nentries;

    for(int index = 0; index < nentries; index++){
        //tree->GetEntry(index);
        ch.GetEntry(index);
        //std:cout <<"index" <<index << std::endl;
        numScintHits=myROOTEvent->GetScintRHits()->size();
        //std::cout << numScintHits << std::endl;
        //eventCout += 1;
        //eventC += 1;
        //double ratio=eventC/nentries;
        //if (eventCout % updateInterval == 0) {
        //cout << ratio*100 << "%" << endl;
        //}
        if (numScintHits > 0) {
            CutTools cut1;
            

            //strict cut  four layers got hit & exactly one layer got hit
            
            int AL1HitPLayresult = cut1.AL1HitPLay(myROOTEvent);
            if (AL1HitPLayresult==1) {AL1HitPLayCount++;}
            
            //this counting is overlap with OneHitPLay(), combining result from
            //AL1HitPLayresult and OneHitPLay can give the result of exactly one hit per layer
            //int ex1HitPLayResult = cut1.ex1HitPLay(myROOTEvent); 
            //if (ex1HitPLayResult==1) {ex1HitPLayCount++;}
            
            
            //since we are not interest about the individual counting, I comment out the following if statement.
            //share cut
            int CosVetoResult = cut1.CosVeto(myROOTEvent);
            //if (CosVetoResult==1) {CosVetoCount++;}
            int BeamPvetoResult = cut1.BeamPveto(myROOTEvent);
            //if (BeamPvetoResult==1) {BeamPvetoCount++;}
            int alongALineResult = cut1.alongALine(myROOTEvent);
            //if (alongALineResult ==1) {HitalonglineCount++;}
            int OneHitPLayResult =cut1.OneHitPLay(myROOTEvent);
            //if (OneHitPLayResult == 1){OneHitPLayCount++;}
            //disable for the without photon sim 
            ///*
            int NPEMinMaxResult = cut1.NPEMinMax(myROOTEvent);
            if (NPEMinMaxResult==1) {NPEMinMaxCount++;}
            int timeCheckResult = cut1.timeCheck(myROOTEvent);
            if (timeCheckResult ==1) {timeCheckCount++;}
            //*/         


            //loose cut at least three layer got hit & at least 3 layer got hit but each layer got one hit only
            int threeLayerHitresult = cut1.ThreeLHit(myROOTEvent);
            if (threeLayerHitresult==1) {threeLayHitCount++;}

            


            // by applying the cut concecutively with cuts at above, the results at below are redundant.
            //int AtleastthreeLayerHitresult = cut1.ThreeLONEHit(myROOTEvent);
            //if (AtleastthreeLayerHitresult==1) {threelayOneHitCount++;}
            //int ThreeinAlineResult = cut1.threeIaLine(myROOTEvent);
            //if (ThreeinAlineResult==1) {ThreeinAlineCount++;}
        
            //apply the cuts consecutively
            //strict cut

            //exactly one hit in three layer
            int Cex1HitPLayResult = AL1HitPLayresult * OneHitPLayResult;
            if (Cex1HitPLayResult==1) {Cex1HitPLayCount ++;}
            //four in a line
            int CfourInlineResult = Cex1HitPLayResult*alongALineResult;
            if(CfourInlineResult==1) {CfourInlineCount ++;}

            //withPhoton sim
            ///*
            int CNpeResult = CfourInlineResult * NPEMinMaxResult;
            if (CNpeResult == 1) {CNpeStrictCount ++;}
            int CTimeCutResult = CNpeResult * timeCheckResult;
            if (CTimeCutResult == 1) {CTimecutStrictCount ++;} 
            //*/    

            int CCosVetoStrictResult = CosVetoResult * CTimeCutResult;
            if (CCosVetoStrictResult==1) {CCosVetoStrictCount ++;}
            int CBeamVetoStrictRestult = CCosVetoStrictResult*BeamPvetoResult;
            if (CBeamVetoStrictRestult==1) {CBeamVetoStrictCount ++;}
            

            //interest event fileNumber + index
            //eg cospaneal : index1
            //eg beampanel : index2
            //use summingResult_like code to combine the event number within a file

            //loose cut
            //exactly 1 hit in 3 or more layers
            int Catleast3layerOneHitResult = cut1.ThreeLONEHit(myROOTEvent);
            if (Catleast3layerOneHitResult==1) {CAtleastthreeLayerHitcount++;}
            //hits along a lines for at least three layers
            int CthreeIaLineResult = Catleast3layerOneHitResult * cut1.threeIaLine(myROOTEvent);
            if (CthreeIaLineResult == 1) {
                Cexa1HitPLayFourCount++;
                eventDetail << "CTHreeinaline :" <<  index << endl;
            }

            
            
            //withPhoton sim
            ///*
            int CNpelooseResult = CthreeIaLineResult * NPEMinMaxResult;
            if (CNpelooseResult == 1) {
                CNpelooseCount ++;
                eventDetail << "CNpeCut :" <<  index << endl;
            }
            int CTimeCutlooseResult = CNpelooseResult * timeCheckResult;
            if (CTimeCutlooseResult == 1) {
                CTimecutlooseCount ++;
                eventDetail << "TimeCut :" <<  index << endl;
            } 
            //*/  
            
            
            int CCosVetolooseResult = CosVetoResult * CTimeCutlooseResult;
            if (CCosVetolooseResult == 1) {CCosVetolooseCount++;}
            int CBeamPvetolooseResult = CCosVetolooseResult * BeamPvetoResult;
            if (CBeamPvetolooseResult == 1) {CBeamPvetolooseCount++;}
            


             
            
        }
    }
    
    
    outputFile << "totoal events:" << eventCount << endl;
    outputFile << "strict cut" << endl;

    //comment out the counting for applying individual cut on events
    //outputFile << "exactly one hit per layer:" << ex1HitPLayCount << endl;
    //outputFile << "cosmic veto:" << CosVetoCount << endl;
    //outputFile <<  "beam panel veto:" << BeamPvetoCount << endl;
    //outputFile << "exactly 1 hit per layer, 4 hits in a line:" << exa1HitPLayFourCount<< endl;
    //outputFile << "exactly 1 hit per layer, 3 hits in a line:" << ThreeinAlineCount << endl;
    //outputFile << "max hit NPE / min hit NPE < 10  :" << NPEMinMaxCount << endl;
    //outputFile << "largest calibrated hit time difference is within 15ns:" << timeCheckCount << endl;
    
    //strict cut
    //outputFile << "apply the cuts consecutively" << endl;
    outputFile << "at least 1 hit per layer:" <<  AL1HitPLayCount<< endl;
    outputFile << "exactly one hit per layer(C):" << Cex1HitPLayCount << endl;
    outputFile << "4 hits in a line (C):" <<  CfourInlineCount << endl;
    //withphoton sim

    outputFile << "NPE max/min < 10:" << CNpeStrictCount << endl;
    outputFile << "T < 15.04 ns:" <<  CTimecutStrictCount << endl;

    outputFile << "cosmic veto(C):" << CCosVetoStrictCount << endl;
    outputFile << "beam panel veto(C):" << CBeamVetoStrictCount << endl;


    //loose cut
    outputFile << "loose cut" << endl;
    outputFile << "1+ hits in 3 or more layers :" << threeLayHitCount << endl;
    outputFile << "exactly 1 hit in 3 of four layers :" << CAtleastthreeLayerHitcount << endl;
    outputFile << "hits are all in line in 3 layers:" << Cexa1HitPLayFourCount << endl;
    //withphoton sim

    outputFile << "NPE max/min < 10:" << CNpelooseCount <<endl;
    outputFile << "T < 15.04 ns:"  << CTimecutlooseCount << endl;
    
    outputFile << "cosmic panel Veto:" << CCosVetolooseCount << endl;
    outputFile << "beam panel Veto:" << CBeamPvetolooseCount << endl;

    


    outputFile.close();
    return 0; //does notthing

}

