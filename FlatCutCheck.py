"""
1-11 24
the goal of this file is to check the following things after the fix deposited energy when facing 
compton scater and muon capture process.

1. first two geometric cuts, npe and time cut while using with and without photon data.




"""

import ROOT
import os

import numpy as np

from FlatMethod import *

K = 1

OneHitPlInfor = f"newFlatResult/OneHitPerlay{K}.txt"

#with photon 
#WOPhfile_names = [f"/net/cms26/cms26r0/schmitz/milliQanFlatSim/cosmic/barNoPhoton/output_{i}.root" for i in range(K, K+1)]

WithPhfile_names= [f"/net/cms26/cms26r0/schmitz/milliQanFlatSim/cosmic/barWithPhoton/output_{i}.root" for i in range(K, K+1)]


#file that register which event pass one hit per layer
WithPhotonInfoFiles = f"newFlatResult/WPIndividual{K}.txt"

WithOutPhotonInfoFiles = f"newFlatResult/WPIndividual{K}.txt"

#resiter the file and index number if the NPE > 0 but Energy < 0 still exist
SpecialEvent = f"newFlatResult/special{K}.txt"

with open(WithPhotonInfoFiles, 'w') as PhotonFile, open(OneHitPlInfor, 'w') as OneHitEvent, open(SpecialEvent,'w') as SP:
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

            # when the descripancy occur
            if (AL_4_layer_got_hits != AL_4_layer_got_hits_WP) or (with_4_unique_hits != with_4_unique_hits_WP):
                SP.write(f"descripancy : {J}")
       
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



#old script

"""
CosPanelHit = False
BeamPanelHit = False
unique_layers = set()

barlayerlist = []
hits=0


for i in range(len(layers)):
    detect = 1-math.exp(-1*nPE[i])
    detector = random.random()
    if chan[i] <= 65 and nPE[i]>0:
    #if chan[i] <= 65 and (detector < detect):
        unique_layers.add(layers[i])
        barlayerlist.append(layers[i])

        hits = hits + 1
    
    #panel cuts investigation
    else:
        #cosmic panel
        if (chan[i] != 71 or chan[i] != 75) and nPE[i]>0.5:
            CosPanelHit = True
            
        

        #beam panel
        if (chan[i] == 71 or chan[i] == 75) and nPE[i]>0.5:
            BeamPanelHit = True


h.Fill(len(unique_layers))

# check for at leat 1 hits per layer
if len(unique_layers) == 4:
    events_AL_4_layer_got_hits += 1
    #filedebug.write(f"1+PerLay : {str(J)} \n")


# Check for exactly 4 hits with 1 hit in each layer
if len(unique_layers) == 4 and hits == 4:
    events_with_4_unique_hits += 1
    #print("index:" + str(J))
    
    #print(file_name)
    #print(barlayerlist)
    #print(nPE)
    #print(detectlist)
    #print(detectorlist)
    #print(unique_layers)

    OneHitEvent.write(f"1 Per Lay : {str(J)} \n") #J is the event index

    #panel Hit count
    if not (CosPanelHit):
        events_cosVeto += 1
        if not (BeamPanelHit):
            events_beamVeto += 1
"""