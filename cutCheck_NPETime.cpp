/*
1-9 
the goal of this file is used to find the NPE and time cut efficiency

Other than that I am also using this script to do the anlysis geometric cut for with vs without photon data

Change mapping to the one for new repo (TBD)

find the cut efficiency of 1 hit per layer with probability. need to create two extra cuts.


1-10
bug:
Processing cutCheck_NPETime.cpp...
terminate called after throwing an instance of 'std::out_of_range'
  what():  vector::_M_range_check

the bug comes from NPE and time cut

done

all photon on data should use NPEdetect()

which one should use P? wihtout photon
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
#include <ctime>
#include <cstdlib>
#include <cmath>

using namespace std;
R__LOAD_LIBRARY(/net/cms26/cms26r0/zheng/barSimulation/WithPhotonUpdateSim/milliQanSim/build/libMilliQanCore.so)


//return 1 means event pass the cut
class CutTools {



public:


    
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

    //exactly 1 bar hit per layer: only 4 channels got hit & 4 layers got hits in a event
    //with P && without photon data 
    //checked
    int EX1BarHitPLayP(mqROOTEvent* myROOTEvent){
        int numScintHits=myROOTEvent->GetScintRHits()->size();
        std::set<int> layer;
        std::set<int> channel;
        std::map<int, double> mapOfNPE;//it provide the summing deposited 
        const int numberOfChannel = 64;
        const double defaultNPE = 0.0;
        for (int i = 0; i < numberOfChannel; ++i) {mapOfNPE[i] = defaultNPE;}
        int hitN;
        int layerN;

        // Seed the random number generator with the current time
        std::srand(static_cast<unsigned>(std::time(nullptr)));

        // Generate a random number between 0 and 1
        double random_number = static_cast<double>(std::rand()) / RAND_MAX;

        for (int h =0; h < numScintHits; h++)
        {
            hitN = simChanTransfer(myROOTEvent->GetScintRHits()->at(h)->GetCopyNo());
            double energy = myROOTEvent->GetScintRHits()->at(h)->GetEDep();
            double npe = energy* EtoNpe(myROOTEvent->GetScintRHits()->at(h)->GetCopyNo());

            //exclude the veto pannals
            if (hitN <= 64)
            {
                mapOfNPE[hitN] += npe;
            }

            
        }

        for (const auto& pair : mapOfNPE)
        {
                int chanNum = pair.first; 
                double NPE = pair.second; //total deposit energy on a bar
                if ((NPE > 0) && (random_number < (1 - exp(-NPE)) ))
                {   
                    //cout << chanNum << endl; //debug
                    int layerN = (chanNum)/16;
                    layer.insert(layerN);
                    channel.insert(chanNum);
                }
                
        }

        int layS = layer.size(); 
        //cout << layS << endl; //debug

        int NumberOfchannel = channel.size();
        //cout << NumberOfchannel << endl; //debug
        if ((NumberOfchannel == 4) && (layS == 4)){return 1;}
        else {return 0;}
        
    }
    //exactly 1 bar hit per layer: only 4 channels got hit & 4 layers got hits in a event
    //without P && without photon data 
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
            hitN = simChanTransfer(myROOTEvent->GetScintRHits()->at(h)->GetCopyNo());
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
                    //cout << chanNum << endl; //debug
                    int layerN = (chanNum)/16;
                    layer.insert(layerN);
                    channel.insert(chanNum);
                }
                
        }

        int layS = layer.size(); 
        //cout << layS << endl; //debug

        int NumberOfchannel = channel.size();
        //cout << NumberOfchannel << endl; //debug
        if ((NumberOfchannel == 4) && (layS == 4)){return 1;}
        else {return 0;}
        
    }


    // photon on && wihtout P cut
    int EX1BarHitPLay_photonOn(mqROOTEvent* myROOTEvent){
        int pmtHits = myROOTEvent->GetPMTRHits()->size();
        std::set<int> layer;
        std::set<int> channel;
        std::map<int, double> mapOfNPE;//it provide the summing deposited 
        const int numberOfChannel = 64;
        const double defaultNPE = 0.0;
        for (int i = 0; i < numberOfChannel; ++i) {mapOfNPE[i] = defaultNPE;}
        int hitN;
        int layerN;

        for (int h =0; h < pmtHits; h++)
        {
            hitN = simChanTransfer(myROOTEvent->GetPMTRHits()->at(h)->GetPMTNumber());
            
            //exclude the veto pannals
            if (hitN <= 64)
            {
                mapOfNPE[hitN] += 1.0;
            }
 
        }

        for (const auto& pair : mapOfNPE)
        {
                int chanNum = pair.first; 
                double NPE = pair.second; 
                if (NPE > 0)
                {   
                    //cout << chanNum << endl; //debug
                    int layerN = (chanNum)/16;
                    layer.insert(layerN);
                    channel.insert(chanNum);
                }
                
        }

        int layS = layer.size(); 
        //cout << layS << endl; //debug

        int NumberOfchannel = channel.size();
        //cout << NumberOfchannel << endl; //debug
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
            hitN = simChanTransfer(myROOTEvent->GetScintRHits()->at(h)->GetCopyNo());
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
                    
                    int layerN = (chanNum)/16; //old mapping
                    //cout << layerN << endl; //debug
                    layer.insert(layerN);

                }
                
        }

        int layS = layer.size(); 
        //cout << layS << endl; //debug

        if (layS == 4) {return 1;}
        else {return 0;}

        
    }


     //AL1HitPLay:at least 1 hit in scitillator per layer with P cut
    int AL1HitPLay_P(mqROOTEvent* myROOTEvent) {
        int numScintHits=myROOTEvent->GetScintRHits()->size();
        std::set<int> layer;
        std::map<int, double> mapOfNPE;//it provide the summing deposited 
        const int numberOfChannel = 64;
        const double defaultNPE = 0.0;
        for (int i = 0; i < numberOfChannel; ++i) {mapOfNPE[i] = defaultNPE;}
        int hitN;
        int layerN;

        // Seed the random number generator with the current time
        std::srand(static_cast<unsigned>(std::time(nullptr)));

        // Generate a random number between 0 and 1
        double random_number = static_cast<double>(std::rand()) / RAND_MAX;

        for (int h =0; h < numScintHits; h++)
        {
            hitN = simChanTransfer(myROOTEvent->GetScintRHits()->at(h)->GetCopyNo());
            double energy = myROOTEvent->GetScintRHits()->at(h)->GetEDep();
            double npe = energy* EtoNpe(myROOTEvent->GetScintRHits()->at(h)->GetCopyNo());

            //exclude the veto pannals
            if (hitN <= 64)
            {
                mapOfNPE[hitN] += npe;
            }

            
        }

        for (const auto& pair : mapOfNPE)
        {
                int chanNum = pair.first; 
                double NPE = pair.second; //total deposit energy on a bar
                if ((NPE > 0) && (random_number < (1 - exp(-NPE)) ))
                {   
                    //cout << chanNum << endl; //debug
                    int layerN = (chanNum)/16;
                    layer.insert(layerN);
                    
                }
                
        }

        int layS = layer.size(); 
        //cout << layS << endl; //debug

        
        //cout << NumberOfchannel << endl; //debug
        if (layS == 4){return 1;}
        else {return 0;}
    }


    // photon on data && without P cut cut
    int AL1HitPLay_PhotonOn(mqROOTEvent* myROOTEvent) {
        int pmtHits = myROOTEvent->GetPMTRHits()->size();
        std::set<int> layer;

        std::map<int, double> mapOfNPE;//it provide the summing deposited 
        const int numberOfChannel = 64;
        const double defaultNPE = 0.0;
        for (int i = 0; i < numberOfChannel; ++i) {mapOfNPE[i] = defaultNPE;}
        int hitN;
        int layerN;

        for (int h =0; h < pmtHits; h++)
        {
            hitN = simChanTransfer(myROOTEvent->GetPMTRHits()->at(h)->GetPMTNumber());
            
            //exclude the veto pannals
            if (hitN <= 64)
            {
                mapOfNPE[hitN] += 1.0;
            }
 
        }

        for (const auto& pair : mapOfNPE)
        {
                int chanNum = pair.first; 
                double NPE = pair.second; 
                if (NPE > 0)
                {   
                    //cout << chanNum << endl; //debug
                    int layerN = (chanNum)/16;
                    layer.insert(layerN);

                }
                
        }

        int layS = layer.size(); 
        //cout << layS << endl; //debug


        //cout << NumberOfchannel << endl; //debug
        if (layS == 4){return 1;}
        else {return 0;}
    }



    //convert from energy
    int NPEcut(mqROOTEvent* myROOTEvent){
        int numScintHits=myROOTEvent->GetScintRHits()->size();
        std::set<int> layer;
        std::set<int> channel;
        std::map<int, double> chanNpeMap;//it provide the summing deposited 
        const int numberOfChannel = 64;
        const double defaultE = 0.0;
        for (int i = 0; i <= numberOfChannel; ++i) {chanNpeMap[i] = defaultE;}
        int hitN;
        int layerN;

        cout << "reach here" << endl;

        for (int h =0; h < numScintHits; h++)
        {
            hitN = simChanTransfer(myROOTEvent->GetScintRHits()->at(h)->GetCopyNo());
            double energy = myROOTEvent->GetScintRHits()->at(h)->GetEDep();

            //exclude the veto pannals
            if (hitN <= 64) { chanNpeMap[hitN] += energy* EtoNpe(hitN);}
            if (hitN == 64) {cout << "got it" << endl;}            
        }
        

        // Find the most frequent and least frequent elements
        int mostFrequentElement = -1;
        int leastFrequentElement = -1;
        double highestFrequency = -1.0;
        double lowestFrequency = std::numeric_limits<double>::max();

        for (const auto& pair : chanNpeMap) {
            if (pair.second > highestFrequency && pair.second > 0.0) {
                mostFrequentElement = pair.first; //channel
                highestFrequency = pair.second; //npe
            }
            if (pair.second < lowestFrequency && pair.second > 0.0) {
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
    
    int NPEcut_photonON(mqROOTEvent* myROOTEvent){
        int pmtHits = myROOTEvent->GetPMTRHits()->size();
        std::set<int> layer;
        std::set<int> channel;
        std::map<int, double> chanNpeMap;//it provide the summing deposited 
        const int numberOfChannel = 64;
        const double defaultNPE = 0.0;
        for (int i = 0; i <= numberOfChannel; ++i) {chanNpeMap[i] = defaultNPE;}
        int hitN;
        int layerN;
        

        for (int h =0; h < pmtHits; h++)
        {
            hitN = simChanTransfer(myROOTEvent->GetPMTRHits()->at(h)->GetPMTNumber());
            //exclude the veto pannals
            if (hitN <= 64) { chanNpeMap[hitN] += 1.0;}            
        }
        
        

        // Find the most frequent and least frequent elements
        int mostFrequentElement = -1;
        int leastFrequentElement = -1;
        double highestFrequency = -1.0;
        double lowestFrequency = std::numeric_limits<double>::max();



        for (const auto& pair : chanNpeMap) {


            if (pair.second > highestFrequency && pair.second > 0.0) {
                mostFrequentElement = pair.first; //channel
                highestFrequency = pair.second; //npe
            }
            if (pair.second < lowestFrequency && pair.second > 0.0) {
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

    // time difference between hits on first and last layers is within 15ns(not being used, bug might exist)
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


void cutCheck_NPETime()
{
    
    int fileNumber = 2;

    
    
    //count the result of applying cut individually
    string basePath4 = "/net/cms26/cms26r0/zheng/barSimulation/withPhotonAnalysis/result2024Newdebug/Individual";
    string outputPath4 = basePath4 + to_string(fileNumber) + ".txt";
    ofstream outputFile4(outputPath4);
     
    //find the with photon data that pass geometric cut
    string basepath =  "/net/cms26/cms26r0/zheng/barSimulation/withPhotonAnalysis/result2024Newdebug/WithphotonGeometric";
    string outputPath = basepath + to_string(fileNumber) + ".txt";    
    ofstream outputFile(outputPath);

    //location of data file
    TString folderName = Form("/net/cms26/cms26r0/zheng/barSimulation/barWithPhotonUpdate/BARcosmic%d", fileNumber);
    TString fileName = Form("%s/MilliQan.root", folderName.Data());

    

    //txt for saving interesting event(disable in current test)
    //string Filebase = "/net/cms26/cms26r0/zheng/barSimulation/newRepoSwap/debug2/barSimanalysis/result/hist";
    //string outPut = Filebase + to_string(fileNumber) + ".txt";
    //ofstream eventDetail(outPut);

    //string basePath5 = "/net/cms26/cms26r0/zheng/barSimulation/newRepoSwap/debug2/barSimanalysis/result/EHist";
    //string rootFileName = basePath5 + to_string(fileNumber) + ".root";  
    //TFile ChanHist(rootFileName.c_str(), "RECREATE");
    //TH1F* SumEDistribution0 = new TH1F("E distribution0", "E(sum along layer 0 ) distribution", 120, -600, 600); //energy can reach up to 600MeV, but 0-100MeV is sufficient for see the trend
    //TH1F* SumEDistribution1 = new TH1F("E distribution1", "E(sum along layer 1 ) distribution", 120, -600, 600); //energy can reach up to 600MeV, but 0-100MeV is sufficient for see the trend
    //TH1F* SumEDistribution2 = new TH1F("E distribution2", "E(sum along layer 2 ) distribution", 120, -600, 600); //energy can reach up to 600MeV, but 0-100MeV is sufficient for see the trend
    //TH1F* SumEDistribution3 = new TH1F("E distribution3", "E(sum along layer 3 ) distribution", 120, -600, 600); //energy can reach up to 600MeV, but 0-100MeV is sufficient for see the trend
    //TH1F* SumEDistribution4 = new TH1F("E distribution4 layer", "E(sum along 4 layers) distribution", 120, -600, 600);
    //TH1F* layerDistribution = new TH1F("layerDistribution", "layerDistribution", 5, 0, 5);
    
    TChain ch("Events");
    ch.Add(fileName);
     
    mqROOTEvent* myROOTEvent = new mqROOTEvent();
    ch.SetBranchAddress("ROOTEvent", &myROOTEvent);
    Long64_t nentries=ch.GetEntries();

    //count how many event pass an individual cut
    int AL1HitPlayerCount = 0;

    int exa1HitPLayCount = 0;

    int AL1HitPLayNPECount = 0;

    int EX1HitPLayNPECount = 0;

    int NPE_photon = 0;

    int time_photon = 0;

    int AL1HitPLayPCount = 0;

    int EX1HitPLayPCount = 0;

    int eventCount = nentries;


    for(int index = 0; index < nentries; index++)
    {
        ch.GetEntry(index);
        int numScintHits=myROOTEvent->GetScintRHits()->size();
        if (numScintHits > 0) {
            
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
            

            int OneHitPLayResult = cut1.EX1BarHitPLay(myROOTEvent);
            if (OneHitPLayResult == 1) {exa1HitPLayCount ++;}


            


            // investivation of result from with phton data 


            int AL1HitPLayNPE = cut1.AL1HitPLay_PhotonOn(myROOTEvent);
            if (AL1HitPLayNPE == 1) {
                AL1HitPLayNPECount ++;
                outputFile <<"Photon on 1 + hit per layer  : " << index << endl;
            }  


            int EX1HitPLayNPE = cut1.EX1BarHitPLay_photonOn(myROOTEvent);
            if (EX1HitPLayNPE == 1) {
                EX1HitPLayNPECount ++;
                outputFile << "Photon on EX1 hit per layer : " << index << endl;
            } 

            

            // investivation of result from without photon data && p cut

            int AL1HitPLayP = cut1.AL1HitPLay_P(myROOTEvent);
            if (AL1HitPLayP == 1) {AL1HitPLayPCount ++;}  


            int EX1HitPLayP = cut1.EX1BarHitPLayP(myROOTEvent);
            if (EX1HitPLayP == 1) {EX1HitPLayPCount ++;} 


            
            // the geometric cut that doesn't come with P is less aggressive 
            //
            
            int NPEcut = cut1.NPEcut_photonON(myROOTEvent);
            
            int timecut = cut1.timeCheck(myROOTEvent);

            
            if (OneHitPLayResult == 1) {
                // NPE (with photon) cut
                

                if (NPEcut == 1)
                {
                    NPE_photon += 1;
                    if (timecut == 1)
                    {
                        time_photon += 1;
                    }
                    
                
                }

            }

            


 

                
        }         
  
    }

    cout << fileName << endl;
    //result of applying single cut
    outputFile4 << "totoal events:" << eventCount << endl;
    outputFile4 << "Events with 1+ hit per layer :"<< AL1HitPlayerCount << endl;
    
    outputFile4 <<  "Events with exactly 1 hit per layer :"<< exa1HitPLayCount << endl;
    
    outputFile4 << "Events with 1+ hit per layer (NPE):"<< AL1HitPLayNPECount << endl;

    
    outputFile4 <<  "Events with exactly 1 hit per layer (NPE) : "   << EX1HitPLayNPECount << endl;
    
    
    outputFile4 <<  "Events with 1+ hit per layer (probability):"   << AL1HitPLayPCount << endl;
    outputFile4 <<  "Events with exactly 1 hit per layer (probability):"   << EX1HitPLayPCount << endl;

    outputFile4 << "NPE cut (with photon but no P)" << NPE_photon << endl;
    outputFile4 << "time cut (with photon but no P)" << time_photon << endl;





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
