import ROOT
import os
import math
import random

startFile=1
endFile=100
# Create a list of file names
file_names = [f"/net/cms26/cms26r0/schmitz/milliQanFlatSim/cosmic/barNoPhoton/lightOffline/output_{i}.root" for i in range(startFile, endFile)]

# Create a histogram (outside the loop)
maxLayers = 6  # Adjust this based on your expected max number of layers
h = ROOT.TH1F("h", "Number of Unique Layers Hit per Event", maxLayers, 0, maxLayers)

# Variable to count events with exactly 4 hits, 1 per layer (outside the loop)
events_with_4_unique_hits = 0
events_with_no_panel_hit = 0
events_with_no_endcap_hit = 0
# Prepare branch variables

#change this into to PMT branch and then check negative value
# i want ot check if summing NPE(PMT) has zero or negative data

layers = ROOT.std.vector('int')()
chan = ROOT.std.vector('int')()
nPE = ROOT.std.vector('float')()
# Loop over each file
fileNum = startFile - 1
for file_name in file_names:

    if not os.path.exists(file_name):
        print(f"File not found: {file_name}")
        continue  # Skip to the next file
    
    print(file_name)
    fileNum = fileNum + 1
    # Open the ROOT file
    file = ROOT.TFile.Open(file_name, "READ")

    # Get the tree
    tree = file.Get("t")  # Replace with your tree name

    # Set the branch addresses
    tree.SetBranchAddress("layer", layers)
    tree.SetBranchAddress("chan", chan)
    tree.SetBranchAddress("nPE", nPE)
    # Loop over events
    eventID=0
    for event in tree:
        unique_layers = set()
        npelist = []
        detectlist = []
        detectorlist = []
        barlayerlist = []
        endcapnpelist = []
        endcaplist = []
        panelnpelist = []
        panellist = []
        alllist = []
        alllistnpe = []
        hits=0
        panelhit=0
        endcaphit=0
        eventID = eventID+1
        for i in range(len(layers)):
            detect = 1-math.exp(-1*nPE[i])
            detector = random.random()
            alllist.append(chan[i])
            alllistnpe.append(nPE[i])
            if chan[i] <= 65 and (nPE[i]>0.5):
            #if chan[i] <= 65 and (detector < detect):
                unique_layers.add(layers[i])
                npelist.append(nPE[i])
                barlayerlist.append(layers[i])
                detectlist.append(detect)
                detectorlist.append(detector)
                hits = hits + 1
            else:
                #if (chan[i] == 71 or chan[i] == 75) and detector < detect:
                if (chan[i] == 71 or chan[i] == 75) and (nPE[i]>0.5):
                    endcaphit = endcaphit + 1
                    endcapnpelist.append(nPE[i])
                    endcaplist.append(chan[i])
                else:
                    #if chan[i] > 66 and detector < detect:
                    if chan[i] > 66 and (nPE[i]>0.5):
                        panelhit = panelhit + 1
                        panelnpelist.append(nPE[i])
                        panellist.append(chan[i])
        h.Fill(len(unique_layers))

        # Check for exactly 4 hits with 1 hit in each layer
        if len(unique_layers) == 4 and hits == 4:
            events_with_4_unique_hits += 1
            #print("we got one!")
            #print("File {}, event {}".format(fileNum,eventID))
            #print("all channel hit list, npe")
            #print(alllist)
            #print(alllistnpe)
            #print("barlayerlist, npe, prob to detect, detect rng, unique layer list")
            #print(barlayerlist)
            #print(npelist)
            #print(detectlist)
            #print(detectorlist)
            #print(unique_layers)
            #print("panellist, npe")
            #print(panellist)
            #print(panelnpelist)
            #print("endcaplist, npe")
            #print(endcaplist)
            #print(endcapnpelist)
            if panelhit == 0:
                events_with_no_panel_hit += 1
                if endcaphit == 0:
                    events_with_no_endcap_hit += 1
    # Close the file
    file.Close()

# Print number of entries in each bin
for bin in range(1, h.GetNbinsX() + 1):
    print(f"Bin {bin}: {h.GetBinContent(bin)} entries")

# Print the count of events with exactly 4 hits, 1 per layer
print(f"Events with exactly 4 hits, 1 per layer: {events_with_4_unique_hits}")
print(f"Events with exactly 4 hits, 1 per layer, no panel hits: {events_with_no_panel_hit}")
print(f"Events with exactly 4 hits, 1 per layer, no panel or endcap hits: {events_with_no_endcap_hit}")
