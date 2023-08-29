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

void NPEandEnergyPlot() {
    
    /*
    TChain ch("Events");
    ch.Add(fileDir);
    
    TH2F* MeasuredNPET =new TH2F("NPEE", "npe & deposited energy", 100, 0, 70, 100,0, 70000);
    TCanvas* c1 = new TCanvas("c1", "c1", 0, 400, 600, 300);
    c1->SetFillColor(0);
    ch.Draw("NbOfPMTHits:Sum$(ScintRHits->EDep_MeV)>>MeasuredNPET", "" ,"NbOfPMTHits>0");
    MeasuredNPET->Draw();
    MeasuredNPET->Write();
    c1->Write();
    
    */
    TFile outputFile("NPEEnergychan68and67.root", "RECREATE");
    TChain ch("Events");
    int numberOfFolders = 200;
    for (int folderIndex = 1; folderIndex <= numberOfFolders; ++folderIndex) {
        TString folderName = Form("/net/cms26/cms26r0/zheng/barSimulation/barWithPhotonUpdate/BARcosmic%d", folderIndex);
        TString fileName = Form("%s/MilliQan.root", folderName.Data());
        ch.Add(fileName);
    } 


    //front panel histogram
    TH2F* h0 =new TH2F("back panel", "npe & deposited energy", 100, 0, 200, 100,0, 4000);
    //back panel histogram
    TH2F* h1 =new TH2F("front panel", "npe & deposited energy", 100, 0, 200, 100,0, 4000);
    //front and back histogram
    TH2F* h2 =new TH2F("front&back panel", "npe & deposited energy", 100, 0, 200, 100,0, 4000);
    TCanvas* c0 = new TCanvas("c0", "c1", 0, 400, 600, 300);
    TCanvas* c1 = new TCanvas("c1", "c1", 0, 400, 600, 300);
    TCanvas* c2 = new TCanvas("c2", "c1", 0, 400, 600, 300);
    c1->SetFillColor(0);
    c2->SetFillColor(0);
    c0->SetFillColor(0);


    mqROOTEvent* myROOTEvent = new mqROOTEvent();
    ch.SetBranchAddress("ROOTEvent", &myROOTEvent);


    Long64_t nentries=ch.GetEntries();
    for(int index = 0; index < nentries; index++){
        ch.GetEntry(index);

        //NPE
        int pmtHits = myROOTEvent->GetPMTRHits()->size(); //pmt hit in a event
        int npecout = 0; //front and back NPE count
        int FrontNpeCount = 0;
        int BackNpeCount = 0;

        for (int h =0; h < pmtHits; h++) {
            int pmtNumber = myROOTEvent->GetPMTRHits()->at(h)->GetPMTNumber();
            //front and back NPE count
            if ((pmtNumber >=96 &&  pmtNumber <=99)){npecout++;}
            //front NPE count
            if ((pmtNumber ==97) || (pmtNumber ==99)){FrontNpeCount++;}
            //back NPE count
            if ((pmtNumber ==96) || (pmtNumber ==98)){BackNpeCount++;}
        }

        //deposited energy
        double Edp =0; //energy deposit on back and front panel
        double FrontEdp = 0; //energy deposit on front panel
        double BackEdp = 0; //energy deposit on back panel

        int numScintHits=myROOTEvent->GetScintRHits()->size();
        for (int h =0; h < numScintHits; h++) {
            double energy = myROOTEvent->GetScintRHits()->at(h)->GetEDep();
             
            double e_KeV = energy * 1e3; //change the unit into KeV from MeV
            int hitN = myROOTEvent->GetScintRHits()->at(h)->GetCopyNo();
            //front panel
            if (( hitN == 67) && (energy > 0) ) {
                 FrontEdp += e_KeV;
            }
            //back panel
            if (( hitN == 68) && (energy > 0) ) {
                 BackEdp += e_KeV;
            }

            //front and back panel
            if ((hitN == 68 || hitN == 67) && (energy > 0) ) {
                Edp += e_KeV;
            }
            
        }
        
        //front and back 
        if ((Edp>0) && (npecout > 0)){
            h2->Fill(npecout,Edp);
            //cout<<npecout << " " << Edp << endl;
        }

        //front
        if ((FrontEdp>0) && (FrontNpeCount > 0)){
            h0->Fill(FrontNpeCount,FrontEdp);
            //cout<< "front" <<FrontNpeCount << " " << FrontEdp << endl;
        }

        //back
        if ((BackEdp>0) && (BackNpeCount > 0)){
            h1->Fill(BackNpeCount,BackEdp);
            //cout<< "Back" <<BackNpeCount << " " << BackEdp << endl;
        }

        


    }

    h2->GetYaxis()->SetTitle("Edp(KeV)");
    h2->GetXaxis()->SetTitle("pmtHits");
    h0->GetYaxis()->SetTitle("Edp(KeV)");
    h0->GetXaxis()->SetTitle("pmtHits");
    h1->GetYaxis()->SetTitle("Edp(KeV)");
    h1->GetXaxis()->SetTitle("pmtHits");
    h0->Write();
    h1->Write();
    h2->Write();
    //h2->Draw();
    //c1->Write();
    outputFile.Close();

}