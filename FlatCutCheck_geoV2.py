"""
1-16 24
fix the issue of SpecialEvent so it do what it suppose to do.
I have more interest in study if the issue of summing NPE > 0 but summing Energy < 0 still exist than the issue
of a event has different results when using scint and pmt data for checking geometric cuts.


"""
import sys
import ROOT
import os

import numpy as np

from FlatMethod import *

K = str(sys.argv[1])
#save the file index from with photon data
OneHitPlInfor = f"newFlatResult/OneHitPerlay_new{K}.txt" 


WithPhfile_names= [f"/net/cms26/cms26r0/schmitz/milliQanFlatSim/cosmic/barWithPhoton/output{K}.root"]


#file that register which event pass one hit per layer
WithPhotonInfoFiles = f"newFlatResult/WPIndividual_new{K}.txt"

#save the file and index number if the NPE > 0 but Energy < 0 still exist
SpecialEvent = f"newFlatResult/special_new{K}.txt"

with open(WithPhotonInfoFiles, 'w') as PhotonFile, open(SpecialEvent,'w') as SP, open(OneHitPlInfor,'w') as OneHitEvent:

    maxLayers = 6  # Adjust this based on your expected max number of layers



    #count the without photon events with 0.6 NPE fix cut    
    events_with_4_unique_hits = 0
    events_AL_4_layer_got_hits = 0

    #count the without photon events with P cut 
    events_with_4_unique_hits_p = 0
    events_AL_4_layer_got_hits_p = 0


    # with photon 
    events_with_4_unique_hits_WP = 0
    events_AL_4_layer_got_hits_WP = 0


    # Prepare branch variables with photon 
    layers = ROOT.std.vector('int')()
    chan = ROOT.std.vector('int')()
    nPE = ROOT.std.vector('float')()

    #branch for without photon data
    layers_PMT = ROOT.std.vector('int')()
    chan_PMT = ROOT.std.vector('int')()
    nPE_PMT = ROOT.std.vector('float')()

    # Loop over With photon files
    for file_name in WithPhfile_names:
    

        if not os.path.exists(file_name):
            print(f"File not found: {file_name}")
            continue  # Skip to the next file
        
        print(file_name)
        # Open the ROOT file
        file = ROOT.TFile.Open(file_name, "READ")

        # Get the tree
        tree = file.Get("t")  # Replace with your tree name

        # Set the branch addresses
        tree.SetBranchAddress("layer", layers)
        tree.SetBranchAddress("chan", chan)
        tree.SetBranchAddress("nPE", nPE)

        tree.SetBranchAddress("pmt_nPE", nPE_PMT)
        tree.SetBranchAddress("pmt_chan", chan_PMT)
        tree.SetBranchAddress("pmt_layer", layers_PMT)


        J = 0 # use to check event index
        # Loop over events
        for event in tree:

            AL_4_layer_got_hits,with_4_unique_hits,cosPanel_hit,beamPanel_hit=geometricCut_noP(layers, chan, nPE)
            #cosPanel_hit,beamPanel_hit are not being used at here. 
            if (AL_4_layer_got_hits):
                events_AL_4_layer_got_hits += 1

                if (with_4_unique_hits):
                    events_with_4_unique_hits += 1


            AL_4_layer_got_hits_p,with_4_unique_hits_p,cosPanel_hit_p,beamPanel_hit_p = geometricCut_P(layers, chan, nPE)
            #cosPanel_hit_p,beamPanel_hit_p are not being used at here. 
            if (AL_4_layer_got_hits_p):
                events_AL_4_layer_got_hits_p += 1

                if (with_4_unique_hits_p):
                    events_with_4_unique_hits_p += 1



            AL_4_layer_got_hits_WP,with_4_unique_hits_WP, cosPanel_hit, beamPanel_hit = geometricCut_WithPhoton(layers_PMT, chan_PMT, nPE_PMT)
            #cosPanel_hit, beamPanel_hit are not using right now because withphoton data has limited amount of data. 

            if (AL_4_layer_got_hits_WP):
                events_with_4_unique_hits_p += 1
                
                if(with_4_unique_hits_WP):
                    events_with_4_unique_hits_WP += 1
                    OneHitEvent.write(f"1+ Per Lay : {J} \n")
    

            #check if the issue of umming NPE > 0 but summing Energy < 0 still exist
            SpecialIssue = EDPNPEdebug(nPE_PMT,nPE)
            if (SpecialIssue):
                SP.write(f"descripancy : {J}  \n")

            """
            # when the descripancy occur. I no longer have interest for checking this issue now.
            if (AL_4_layer_got_hits != AL_4_layer_got_hits_WP) or (with_4_unique_hits != with_4_unique_hits_WP):
                print("AL_4_layer_got_hits:" + str(AL_4_layer_got_hits)) #returm true at event 110 from file2
                print("AL_4_layer_got_hits_WP" + str(AL_4_layer_got_hits_WP))
                print("with_4_unique_hits" + str(with_4_unique_hits))
                print("with_4_unique_hits_WP" + str(with_4_unique_hits_WP))
                SP.write(f"descripancy : {J}  \n")
            """
            
                
        
            J += 1
        # Close the file
        file.Close()

    """
    # Print number of entries in each bin
    for bin in range(1, h.GetNbinsX() + 1):
        #print(f"Bin {bin}: {h.GetBinContent(bin)} entries")
        PhotonFile.write(f"Bin {bin}: {h.GetBinContent(bin)} entries \n")
    """
    

    PhotonFile.write(f"1+ Per Lay : {str(events_AL_4_layer_got_hits)} \n")
    PhotonFile.write(f"1 Per Lay : {str(events_with_4_unique_hits)} \n")

    PhotonFile.write(f"1+ Per Lay P cut : {str(events_AL_4_layer_got_hits_WP)} \n")
    PhotonFile.write(f"1 Per Lay P cut : {str(events_with_4_unique_hits_WP)} \n")
    
    PhotonFile.write(f"1+ Per Lay with photon : {str(events_AL_4_layer_got_hits_p)} \n")
    PhotonFile.write(f"1 Per Lay with photon : {str(events_with_4_unique_hits_p)} \n")

    



 
