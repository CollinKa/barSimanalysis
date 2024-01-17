import sys
import ROOT
import os

import numpy as np

from FlatMethod import *

K = str(sys.argv[1])

Datafile_names= [f"/net/cms26/cms26r0/schmitz/milliQanFlatSim/cosmic/barNoPhoton/output_{K}.root"]

Cutinfo = f"newFlatResult/withOutPhotonCutresult{K}.txt"


with open(Cutinfo, 'w') as info:


    #count the without photon events with 0.6 NPE fix cut    
    
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

    # Prepare branch variables with photon 
    layers = ROOT.std.vector('int')()
    chan = ROOT.std.vector('int')()
    nPE = ROOT.std.vector('float')()
    time = ROOT.std.vector('float')()

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
            AL_4_layer_got_hits,with_4_unique_hits,cosPanel_hit,beamPanel_hit=geometricCut_noP(layers, chan, nPE)
            NPECut,TimeCut = NPE_TimeCut_withoutPhoton(chan,layers,nPE,time)
            
            #print("index: " + str(index))   
            if (AL_4_layer_got_hits):
                events_AL_4_layer_got_hits += 1

                if (with_4_unique_hits): 
                    events_with_4_unique_hits += 1
                    
                    if(cosPanel_hit): 
                        events_CosVeto += 1
                        
                        if (beamPanel_hit): 
                            events_BeamVeto +=1

                            if(NPECut):
                                events_NPE += 1

                                if (TimeCut):
                                    events_time += 1



            
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
    print(numberOfEvent)
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


    


                            

    
