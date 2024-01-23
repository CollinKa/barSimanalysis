#####################################################################################################################################

########### A script to convert MilliQan simulation data to a flat, lightweight tree in the format of experimental data #############

###########                                         9/8/23, R.Schmitz                                                   #############

#####################################################################################################################################

###### Designed to be run over EDep-only data, minimal outputs ######

import array
import ROOT
import sys
import math
# Function to convert sim copy number to data copy number
def simToDataScint(simChannel):

    #first, if the hit is in a panel or slab, we manually map it
    if(simChannel == 73):
        return 68
    elif(simChannel == 74):
        return 70
    elif(simChannel == 75):
        return 69
    elif(simChannel == 81):
        return 72
    elif(simChannel == 82):
         return 74
    elif(simChannel == 83): 
        return 73                                                                                                                                                                                                                                                                 
    elif(simChannel == 67):
        return 71
    elif(simChannel == 68): 
        return 75

    #we save the layer number, then map the sim channel to the correct data number, then add the layer number back in
    layerNumber = math.floor(simChannel / 216)
    #layerNumber = simChannel / 216
    simChannel = simChannel % 216

    if simChannel <= 4:
        return (simChannel + 11)+layerNumber*16
    elif simChannel <= 12:
        return simChannel - 1 + layerNumber*16
    elif simChannel <= 16:
        return simChannel - 13 + layerNumber*16
    else:
        print("Error: simChannel out of range")
        return -1
#
cali =[9.7,8.4,5.0,3.6,8.2,8.2,6.8,4.6,8.7,7.1,6.4,7.5,8.7,11.8,9.7,8.1,8.6,9.1,6.4,7.5,9.8,8.8,11,6.6,8.6,11,7.3,8.3,2.6,6.2,7.0,7.2,8.7,7.7,6.8,7.8,8.8,8.5,3.5,8.2,7.6,6.8,9.5,8.5,6.3,6.7,7.6,7.9,9.0,8.3,8.3,6.9,6.0,8.0,5.5,5.5,6.6,8.1,8.4,8.7,9.2,7.6,5.9,8.2] 
    

# Function to populate the vectors in the flattened tree for ScintHits
def populate_vectors_scint(input_tree, scint_copyNo, scint_layer, scint_nPE, scint_time):
    # Clear the vectors
    scint_copyNo.clear()
    scint_layer.clear()
    scint_nPE.clear()
    scint_time.clear()

    # make an array of length 500 to store temporary nPE values
    temp_nPE = [0]*1000
    temp_time = [0]*1000

    # Populate the vectors with flattened data
    for j in range(input_tree.ScintRHits.size()):
        hit = input_tree.ScintRHits.at(j)
        #make temp variable to hold copy number, then convert it to data copy number
        tempCopyNo = hit.GetCopyNo()
        #tempCopyNo = simToDataScint(tempCopyNo)
        # add the hit to the temporary array, using the energy deposition as the nPE and converting it using a measure scale factor
        # also multiply by 7.5/11, the mean ratio of the measured nPE to the simulated nPE for the same geometry
        # this way we have three different ways to scale the nPE according to the geometry and correcting for the difference between simulation and data
        #simToDataScale = 0.682 #7.5/11
        transferChan = simToDataScint(tempCopyNo)
        if transferChan <=63 :
            #print(transferChan) 
            simToDataScale = cali[transferChan]/11
        else:
            # for slab
            simToDataScale = 7.5/11
        if(tempCopyNo == 67 or tempCopyNo == 68):
            nPEPerMeV = 110.2*simToDataScale #measured using bar cosmic dataset
        elif(tempCopyNo == 73 or tempCopyNo == 74 or tempCopyNo == 75 or tempCopyNo == 81 or tempCopyNo == 82 or tempCopyNo == 83):
            nPEPerMeV = 28.7*simToDataScale #measured using bar cosmic dataset
        else:
            nPEPerMeV = 331.2*simToDataScale #measured using bar cosmic dataset
        edep = hit.GetEDep()
        name = hit.GetParticleName()
        if(hit.GetEDep()>0 and name == 11):
            edep = edep-0.511
        if(hit.GetEDep()<0 and name == 11):
            edep = edep+0.511
        if(hit.GetEDep()<0 and name == -11):
            edep = edep-0.511
        if(hit.GetEDep()>0 and name == -11):
            edep = edep+0.511
        if(hit.GetEDep()>0 and name == 2112):
            edep = edep+4.946323
        if(hit.GetEDep()<0 and name == 2112):
            edep = edep-4.946323

        temp_nPE[tempCopyNo] = temp_nPE[tempCopyNo] + edep*nPEPerMeV

        # if the hit time in the channel is lower than the current time, replace the current time with the new time
        if(edep > 0 and (temp_time[tempCopyNo] == 0 or hit.GetHitTime() < temp_time[tempCopyNo])):
            temp_time[tempCopyNo] = hit.GetHitTime()

    # loop over the temporary arrays and add the values to the vectors
    for j in range(1000):
        # only save values where the nPE is greater than 0
        #if(temp_nPE[j] > 0):
        if(temp_nPE[j] != 0):
            scint_nPE.push_back(temp_nPE[j])
            scint_time.push_back(temp_time[j])
            dataChan = simToDataScint(j)
            scint_copyNo.push_back(int(dataChan))
            if(j == 67):
                scint_layer.push_back(-1)
            elif(j == 68):
                scint_layer.push_back(4)
            elif(j == 73 or j == 74 or j == 75):
                scint_layer.push_back(0)
            elif(j == 81 or j == 82 or j == 83):
                scint_layer.push_back(2)
            else:
                scint_layer.push_back(int(j/216))

# Function to create the branches in the new tree for ScintHits
def create_branches_scint(output_tree, scint_copyNo, scint_layer, scint_nPE, scint_time):
    # Create the branch for the flattened data
    output_tree.Branch("chan", scint_copyNo)
    output_tree.Branch("layer", scint_layer)
    output_tree.Branch("nPE", scint_nPE)
    output_tree.Branch("time", scint_time)

def create_branches_event(output_tree, event, runNumber):
    output_tree.Branch("event", event, "event/I")
    output_tree.Branch("runNumber", runNumber, "runNumber/I")

def populate_vectors_event(input_tree, event, runNumber):
    evt = input_tree.ROOTEvent
    event[0] = evt.GetEventID()
    runNumber[0] = 1

##################################################################################################################
# Main script
# Load the custom dictionary
if ROOT.gSystem.Load("/net/cms26/cms26r0/zheng/barSimulation/WithPhotonUpdateSim/milliQanSim/build/libMilliQanCore.so") < 0:
    raise Exception("Failed to load custom dictionary.")

# Open the input ROOT file specified in the first argument
#filename = "/net/cms26/cms26r0/zheng/barSimulation/barWithPhotonUpdate/BARcosmic" + sys.argv[1] + "/MilliQan.root"
filename = sys.argv[1] + sys.argv[2] + "/MilliQan.root"
outname = "output_" + sys.argv[2] + ".root"

input_file = ROOT.TFile(filename, "READ")

# Get the input ROOT tree
input_tree = input_file.Get("Events")

# Create an output ROOT file
# it should be FILENAME_flat.root
output_file = ROOT.TFile(outname, "RECREATE")
output_tree = ROOT.TTree("t", "A flat, lightweight tree for MilliQan sim data")

# Scint variables to hold flattened data
scint_copyNo = ROOT.std.vector('int')()
scint_layer = ROOT.std.vector('int')()
scint_nPE = ROOT.std.vector('float')()
scint_time = ROOT.std.vector('float')()

# Variables to hold event-level data
event = array.array('i', [0])  # 'i' is for integer type
runNumber = array.array('i', [0])

# Create the branches in the new tree
# want only the following branches with the following names:
# event - Will be the same as the input tree event
# runNumber - Will use Run 1 for now

# nPE - Will be determined using a sum of scint_EDep and a scaling factor
# time - Will use a minimum time from scint_firstHitTime
# layer - will store layer number, 0-indexed, with end panels as -1 and 4
# chan - will store copyNo from Scint hit

# of these, event-level parameters are event and runNumber
# scintillator-level parameters are nPE, time, layer, chan

# Create the branches in the new tree
create_branches_event(output_tree, event, runNumber)
create_branches_scint(output_tree, scint_copyNo, scint_layer, scint_nPE, scint_time)

# Loop over entries in the input tree
n_entries = input_tree.GetEntries()
for i in range(n_entries):
    input_tree.GetEntry(i)

    # Populate the vectors with flattened data
    populate_vectors_event(input_tree, event, runNumber)
    populate_vectors_scint(input_tree, scint_copyNo, scint_layer, scint_nPE, scint_time)
    
    # Fill the new tree with the flattened data
    output_tree.Fill()

# Write and close the output file
output_file.Write()
output_file.Close()

print("Finished file " + sys.argv[2])
