/*
note : exactly one hit per layer seems stil rare

9-7-23 all cuts are checked


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

    //ex1HitPLay:exactly one hit per layer(checked), and each layer got hit
    int ex1HitPLay(mqROOTEvent* myROOTEvent) {
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
            cout << "only one channel get hit." << endl;
            return 0;
        }//only one channel get hit.
        if (highestFrequency<(lowestFrequency*10)) {return 1;}
        else {return 0;}

    }

    //largest calibrated hit time difference is within 15ns
    //checked
    int timeCheck(mqROOTEvent* myROOTEvent){
        std::vector<double> timeList;
        int pmtHits = myROOTEvent->GetPMTRHits()->size();
        for (int h =0; h < pmtHits; h++) {
            double hitTime= myROOTEvent->GetPMTRHits()->at(h)->GetFirstHitTime();
            int pmtNumber = myROOTEvent->GetPMTRHits()->at(h)->GetPMTNumber();
            int result = NPEdetect(pmtNumber);
            if (result == 1){
                timeList.push_back(hitTime);
            }
            
        }
        int Size = timeList.size();
        if (Size >=2 ){
            
            auto maxTime = std::max_element(timeList.begin(), timeList.end());
            auto minTime = std::min_element(timeList.begin(), timeList.end());

            if (maxTime-minTime < 15){return 1;}
            else {return 0;}
        }
        else{
            cout << "lack of data" << endl;
            return 0;
        }//unable to detemine if an event qualifies for the timeCheck due to the lack of data.
    }     
};


void cutCheck()
{
    
    //TFile* file = new TFile(fileDir);
    //TTree* tree = (TTree*)file->Get("Events;");
    
    TChain ch("Events");
    int numberOfFolders = 10;
    for (int folderIndex = 1; folderIndex <= numberOfFolders; ++folderIndex) {
        TString folderName = Form("/net/cms26/cms26r0/zheng/barSimulation/barWithPhotonUpdate/BARcosmic%d", folderIndex);
        TString fileName = Form("%s/MilliQan.root", folderName.Data());
        ch.Add(fileName);
    } 
    mqROOTEvent* myROOTEvent = new mqROOTEvent();
    //tree->SetBranchAddress("ROOTEvent", &myROOTEvent);
    //Long64_t nentries=tree->GetEntries();
    ch.SetBranchAddress("ROOTEvent", &myROOTEvent);
    Long64_t nentries=ch.GetEntries();

    int eventCout = 0;//count the number of event after cut

    int AL1HitPLayCount=0;
    int ex1HitPLayCount=0;
    int CosVetoCount=0;
    int BeamPvetoCount=0;
    //exactly 1 hit per layer, 4 hits in a line
    int exa1HitPLayFourCount=0;
    int NPEMinMaxCount=0;
    int timeCheckCount=0;


    for(int index = 0; index < nentries; index++){
        //tree->GetEntry(index);
        ch.GetEntry(index);
        //std:cout <<"index" <<index << std::endl;
        int numScintHits=myROOTEvent->GetScintRHits()->size();
        //std::cout << numScintHits << std::endl;
        if (numScintHits > 0) {
            CutTools cut1;

            int AL1HitPLayresult = cut1.AL1HitPLay(myROOTEvent);
            if (AL1HitPLayresult==1) {AL1HitPLayCount++;}
            int ex1HitPLayResult = cut1.ex1HitPLay(myROOTEvent);
            if (ex1HitPLayResult==1) {ex1HitPLayCount++;}
            int CosVetoResult = cut1.CosVeto(myROOTEvent);
            if (CosVetoResult==1) {CosVetoCount++;}
            int BeamPvetoResult = cut1.BeamPveto(myROOTEvent);
            if (BeamPvetoResult==1) {BeamPvetoCount++;}
            
            int alongALineResult = cut1.alongALine(myROOTEvent);
            int exa1HitPLayFourResult = alongALineResult*ex1HitPLayResult;
            if (exa1HitPLayFourResult ==1) {exa1HitPLayFourCount++;}

            int NPEMinMaxResult = cut1.NPEMinMax(myROOTEvent);
            if (NPEMinMaxResult==1) {NPEMinMaxCount++;}
            int timeCheckResult = cut1.timeCheck(myROOTEvent);
            if (timeCheckResult ==1) {timeCheckCount++;}
             
            
        }
    }
    cout << "at least 1 hit per layer:" <<  AL1HitPLayCount<< endl;
    cout << "exactly one hit per layer:" << ex1HitPLayCount << endl;
    cout << "cosmic veto:" << CosVetoCount << endl;
    cout <<  "beam panel veto:" << BeamPvetoCount << endl;
    cout << "exactly 1 hit per layer, 4 hits in a line:" << exa1HitPLayFourCount<< endl;
    cout << "max hit NPE / min hit NPE < 10  :" << NPEMinMaxCount << endl;
    cout << "largest calibrated hit time difference is within 15ns:" << timeCheckCount << endl;

    return eventCout; //does not do anything

}

