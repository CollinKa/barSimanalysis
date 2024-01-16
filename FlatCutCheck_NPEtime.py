import ROOT
import os
import math
import random
import numpy as np
import sys
from FlatMethod import NPE_TimeCut_withPhoton
K = int(sys.argv[1])

OneHitPlInfor = f"/net/cms26/cms26r0/zheng/barSimulation/withPhotonAnalysis/newFlatResult/OneHitPerlay{K}.txt"
file_names = [f"/net/cms26/cms26r0/schmitz/milliQanFlatSim/cosmic/barWithPhoton/output_{i}.root" for i in range(K, K+1)]

# Initialize an empty list to store the indices
indices = []

# Read the file and extract indices
with open(OneHitPlInfor, 'r') as file:
    for line in file:
        # Split the line and extract the index part
        index_part = line.split(':')[-1].strip()

        # Append the index to the list
        indices.append(index_part)

#if indices == []:
#    quit()
print(indices)

# Prepare branch variables
layers = ROOT.std.vector('int')()
time = ROOT.std.vector('float')()
nPE = ROOT.std.vector('float')()
chan = ROOT.std.vector('int')()

TimeCutNo =0
NPECutNo = 0

# Loop over each file
for file_name in file_names:

    if not os.path.exists(file_name):
        print(f"File not found: {file_name}")
        continue  # Skip to the next file

    print(file_name)
    # Open the ROOT file
    file = ROOT.TFile.Open(file_name, "READ")

    # Get the tree
    tree = file.Get("t")  # Replace with your tree name

    tree.SetBranchAddress("pmt_chan", chan)
    tree.SetBranchAddress("pmt_layer", layers)
    tree.SetBranchAddress("pmt_time", time)
    tree.SetBranchAddress("pmt_nPE", nPE)
    J = 0 # use to check event index
    # Loop over th seleced events
    for event in indices:
        NPECut = False
        TimeCut = False

        tree.GetEntry(int(event))
        NPECut,TimeCut = NPE_TimeCut_withPhoton(chan,layers,nPE,time) 
        if TimeCut: TimeCutNo += 1
        if NPECut: NPECutNo += 1

lastTeoCutFile = f"newFlatResult/withPhotonNPETimecut{K}.txt"
if NPECutNo > 0:
    print("found one")
    with open(lastTeoCutFile,'w') as ls:
        ls.write(f"NPE cut :{NPECutNo}")
        if TimeCut > 0:
            ls.write(f"time cut :{TimeCutNo}")     
