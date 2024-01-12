"""
1-11 24
the goal of this file is to check the following things after the fix deposited energy when facing 
compton scater and muon capture process.

1. first two geometric cuts, npe and time cut while using with and without photon data.


this file only focus the first two geometric cut

"""
import sys
import ROOT
import os

import numpy as np

from FlatMethod import *

K = str(sys.argv[1])
#save the file index from with photon data
OneHitPlInfor = f"newFlatResult/OneHitPerlay{K}.txt" 

#with photon 
#WOPhfile_names = [f"/net/cms26/cms26r0/schmitz/milliQanFlatSim/cosmic/barNoPhoton/output_{i}.root" for i in range(K, K+1)]

WithPhfile_names= [f"/net/cms26/cms26r0/schmitz/milliQanFlatSim/cosmic/barWithPhoton/output_{K}.root"]


#file that register which event pass one hit per layer
WithPhotonInfoFiles = f"newFlatResult/WPIndividual{K}.txt"

WithOutPhotonInfoFiles = f"newFlatResult/WPIndividual{K}.txt"

#resiter the file and index number if the NPE > 0 but Energy < 0 still exist
SpecialEvent = f"newFlatResult/special{K}.txt"

with open(WithPhotonInfoFiles, 'w') as PhotonFile, open(SpecialEvent,'w') as SP, open(OneHitPlInfor,'w') as OneHitEvent:
    # Create a histogram (outside the loop)
    maxLayers = 6  # Adjust this based on your expected max number of layers
    #h = ROOT.TH1F("h", "Number of Unique Layers Hit per Event", maxLayers, 0, maxLayers)

    # Variable to count events with exactly 4 hits, 1 per layer (outside the loop)
    events_with_4_unique_hits = 0
    events_AL_4_layer_got_hits = 0

    # with photon 
    events_with_4_unique_hits_WP = 0
    events_AL_4_layer_got_hits_WP = 0



    #events_with_4_unique_hits_p = 0
    #events_AL_4_layer_got_hits_p = 0

    events_cosVeto = 0
    events_beamVeto = 0

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

            AL_4_layer_got_hits,with_4_unique_hits=geometricCut_WithPhoton(layers, chan, nPE)

            if (AL_4_layer_got_hits):
                events_AL_4_layer_got_hits += 1

                if (with_4_unique_hits):
                    events_with_4_unique_hits += 1


            AL_4_layer_got_hits_WP,with_4_unique_hits_WP = geometricCut_WithPhoton(layers_PMT, chan_PMT, nPE_PMT)

            if (AL_4_layer_got_hits_WP):
                events_AL_4_layer_got_hits_WP += 1
                
                if(with_4_unique_hits_WP):
                    events_with_4_unique_hits_WP += 1
                    OneHitEvent.write(f"1+ Per Lay : {J} \n")

            # when the descripancy occur
            if (AL_4_layer_got_hits != AL_4_layer_got_hits_WP) or (with_4_unique_hits != with_4_unique_hits_WP):
                print("AL_4_layer_got_hits:" + str(AL_4_layer_got_hits)) #returm true at event 110 from file2
                print("AL_4_layer_got_hits_WP" + str(AL_4_layer_got_hits_WP))
                print("with_4_unique_hits" + str(with_4_unique_hits))
                print("with_4_unique_hits_WP" + str(with_4_unique_hits_WP))
                SP.write(f"descripancy : {J}  \n")
                
        
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

    
    PhotonFile.write(f"1+ Per Lay photon : {str(events_AL_4_layer_got_hits_WP)} \n")
    PhotonFile.write(f"1 Per Lay photon : {str(events_with_4_unique_hits_WP)} \n")


    #PhotonFile.write(f"cosVeto : {str(events_cosVeto)} \n")
    #PhotonFile.write(f"beamVeto : {str(events_beamVeto)} \n")
    # Print the count of events with exactly 4 hits, 1 per layer
    #print(f"Events with exactly 4 hits, 1 per layer: {events_with_4_unique_hits}")
    #print(f"Events with at least 1 hit per layer: {events_AL_4_layer_got_hits}")



 
