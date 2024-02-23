import sys
import ROOT
import os

import numpy as np

from FlatMethod import *



#file index for finding 

interstFile = [26, 33, 45, 54, 94, 95, 158, 163, 166, 167, 190, 195, 217, 218, 221, 225, 236, 246, 264, 286, 316, 346, 385, 391, 395, 400, 416, 417, 420, 439, 442, 464, 512, 516, 524, 527, 534, 571, 577, 628, 661, 666, 675, 676, 698, 743, 764, 765, 767, 801, 822, 854, 855, 866, 869, 872, 873, 886, 976, 979, 980, 1000, 1014, 1022, 1026, 1028, 1082, 1086, 1104, 1106, 1123, 1127, 1168, 1171, 1178, 1189, 1190, 1206, 1220, 1268, 1289]

#interstFile = [10]
Datafile_names = []
#K = str(sys.argv[1])

for index in interstFile:
    Datafile_names.append(f"/net/cms26/cms26r0/schmitz/milliQanFlatSim/cosmic/barNoPhoton/output_{index}.root")


#Datafile_names= [f"/net/cms26/cms26r0/schmitz/milliQanFlatSim/cosmic/barNoPhoton/output_{K}.root"]

#Cutinfo = f"newFlatResult/withOutPhotonCutresult{K}.txt"


#with open(Cutinfo, 'w') as info:


#count the without photon events with 0.6 NPE fix cut    

"""
events_AL_4_layer_got_hits = 0
events_with_4_unique_hits = 0

events_CosVeto = 0
events_BeamVeto = 0

events_NPE = 0
events_time = 0



#count the without photon events with P cut 
events_with_4_unique_hits_p = 0
events_AL_4_layer_got_hits_p = 0

events_CosVeto_p = 0
events_BeamVeto_p = 0

events_NPE_p = 0
events_time_p = 0

"""

# Prepare branch variables with photon 
layers = ROOT.std.vector('int')()
chan = ROOT.std.vector('int')()
nPE = ROOT.std.vector('float')()
time = ROOT.std.vector('float')()


histL0 = ROOT.TH1F("l0correct time", "lay0 time;ns;number", 4000, -2000, 2000)
histL1 = ROOT.TH1F("l1correct time", "lay1 time;ns;number", 4000, -2000, 2000)
histL2 = ROOT.TH1F("l2correct time", "lay2 time;ns;number", 4000, -2000, 2000)
histL3 = ROOT.TH1F("l3correct time", "lay3 time;ns;number", 4000, -2000, 2000)






# Loop over each file
for file_name in Datafile_names:

    if not os.path.exists(file_name):
        print(f"File not found: {file_name}")
        continue  # Skip to the next file

    print(file_name)
    # Open the ROOT file
    file = ROOT.TFile.Open(file_name, "READ")

    # Get the tree
    tree = file.Get("t")

    tree.SetBranchAddress("layer", layers)
    tree.SetBranchAddress("chan", chan)
    tree.SetBranchAddress("nPE", nPE)
    tree.SetBranchAddress("time", time)
    numberOfEvent = tree.GetEntries()      
    for index,event in enumerate(tree):
        #if os.path.exists(file_name):  #this if statement do nothing 
        tree.GetEntry(index) 
        AL_4_layer_got_hits,with_4_unique_hits,cosPanel_hit,beamPanel_hit=geometricCut_noP(layers, chan, nPE)
        #NPECut,TimeCut = NPE_TimeCut_withoutPhoton(chan,layers,nPE,time)
        

        #for time1,npe1 in zip(time,nPE):
        #    if npe1 > 0.5:
        #        print(time1)
        #print("next event")
        #print("index: " + str(index))   
        if (AL_4_layer_got_hits):
            #events_AL_4_layer_got_hits += 1

            if (with_4_unique_hits): 
                #events_with_4_unique_hits += 1
                
                if(cosPanel_hit): 
                    #events_CosVeto += 1
                    
                    if (beamPanel_hit): 
                        #events_BeamVeto +=1
                        
                      
                        NPECut,TimeCut = NPE_TimeCut_withoutPhoton(chan,layers,nPE,time)
                        if(NPECut):
                            #events_NPE += 1
                            
                            correct_time_plot(chan,layers,nPE,time,[histL0,histL1,histL2,histL3])
                            #if (TimeCut):
                            #    events_time += 1
                            #    print("index:" + str(index) + " pass time cut")

    file.Close()
root_file = ROOT.TFile("time_debug_WithouttLarZ.root", "RECREATE")

histL0.Write()
histL1.Write()
histL2.Write()
histL3.Write()

root_file.Close()

"""        
        AL_4_layer_got_hits_p,with_4_unique_hits_p,cosPanel_hit_p,beamPanel_hit_p=geometricCut_P(layers, chan, nPE)
        NPECut_p,TimeCut_p=NPE_TimeCut_P(chan,layers,nPE,time)
        
        
        if (AL_4_layer_got_hits_p):
            events_AL_4_layer_got_hits_p += 1

            if (with_4_unique_hits_p): 
                events_with_4_unique_hits_p += 1
                
                if(cosPanel_hit_p): 
                    events_CosVeto_p += 1
                    
                    if (beamPanel_hit_p): 
                        events_BeamVeto_p +=1

                        if (NPECut_p):
                            events_NPE_p += 1

                            if(TimeCut_p):
                                events_time_p +=1
"""
print(numberOfEvent)
"""
info.write(f"number of events :{str(numberOfEvent)} \n")
info.write(f"1+ Per Lay :{str(events_AL_4_layer_got_hits)} \n")
info.write(f"1 Per Lay :{str(events_with_4_unique_hits)} \n")
info.write(f"cosVeto  :{str(events_CosVeto)} \n")
info.write(f"beamVeto :{str(events_BeamVeto)} \n")
info.write(f"NPEcut :{str(events_NPE)} \n")
info.write(f"Timecut :{str(events_time)} \n")

info.write(f"1+ Per Lay with p:{str(events_AL_4_layer_got_hits_p)} \n")
info.write(f"1 Per Lay with p:{str(events_with_4_unique_hits_p)} \n")
info.write(f"cosVeto  with p:{str(events_CosVeto_p)} \n")
info.write(f"beamVeto with p:{str(events_BeamVeto_p)} \n")
info.write(f"NPEcut with p:{str(events_NPE_p)} \n")
info.write(f"Timecut with p:{str(events_time_p)} \n")
"""




                        


