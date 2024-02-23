import ROOT
import os
import math
import random

startFile=1
endFile=1300

# Create a list of file names

#file before introducing channel based meV/NPE corection
#file_names = [f"/net/cms26/cms26r0/schmitz/milliQanFlatSim/cosmic/barWithPhoton/output_{i}.root" for i in range(startFile, endFile)]

#file with introducing channel based meV/NPE corection
file_names = [f"/mnt/hadoop/se/store/user/czheng/SimFlattree/withPhoton/output_{i}.root" for i in range(startFile, endFile)]


# Create a histogram (outside the loop)
maxLayers = 6  # Adjust this based on your expected max number of layers
h = ROOT.TH1F("h", "Number of Unique Layers Hit per Event", maxLayers, 0, maxLayers)
barh = ROOT.TH1F("bar h", "Number of unique bar hit per Event", 32,0,32)
npeh = ROOT.TH1F("npe h", "NPE distribution", 500,0,1000)
npeRatioS = ROOT.TH1F("npe h ratio s", "NPE ratio(after 1 hit per layer) distribution", 500,0,1000)
npeRatioh = ROOT.TH1F("npe h ratio", "NPE ratio distribution", 500,0,1000)

#time diff follow the run3 projection way
#collect time difference after 
timediffMaxh_FL = ROOT.TH1F("max time diff-FL(new)", "max time diff(pass 1 hit per layer) distribution", 50,-50,50)
timediffMaxh_COS2 = ROOT.TH1F("max time diff-cos2(new)", "max time diff(pass 1 hit per layer & cosVeto) distribution", 50,-50,50)

#introduce time correction and plot the time distribution.
#The goal is just to make the time of arrival of a particle coming from the beam to be 0 for each layer.

correct_timeDist =  ROOT.TH1F("correct time diff", "correct time diff distribution;dT(ns)", 50,-50,50)


#chan distribution for event passing at least 1 hits per layer or 1 hit per layer
ChanMapping_AL1Hit = ROOT.TH1F("ChanMapping_AL1Hit", "chan distribution;channel number", 80,0,80)
ChanMapping_EX1Hit = ROOT.TH1F("ChanMapping_EX1Hit", "chan distribution;channel number", 80,0,80)

 

#cos 

# Variable to count events with exactly 4 hits, 1 per layer (outside the loop)

events_with_AL1HitsPl = 0
events_with_4_unique_hits = 0
events_with_no_panel_hit = 0
events_with_no_endcap_hit = 0
NPERatioCut = 0
corretTimeCut = 0


# Prepare branch variables

#change this into to PMT branch and then check negative value
# i want ot check if summing NPE(PMT) has zero or negative data

layers = ROOT.std.vector('int')()
chan = ROOT.std.vector('int')()
nPE = ROOT.std.vector('float')()
time = ROOT.std.vector('float')()
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
    tree.SetBranchAddress("pmt_layer", layers)
    tree.SetBranchAddress("pmt_chan", chan)
    tree.SetBranchAddress("pmt_nPE", nPE)
    tree.SetBranchAddress("pmt_time",time)
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
        barnpelist = []
        lay0Time = []
        lay3Time = []
        panellist = []
        alllist = []
        alllistnpe = []
        hits=0
        timelist = []
        layerlist = []
        unique_bars = set()
        panelhit=0
        endcaphit=0
        
        correctTime = []
        unique_Chan_AL1Hit = set()
        unique_Chan_EX1Hit = set()
        for i in range(len(layers)):
            detect = 1-math.exp(-1*nPE[i])
            detector = random.random()
            alllist.append(chan[i])
            alllistnpe.append(nPE[i])
            if nPE[i]==0.0:
                print("find 0.0 npe")
            if nPE[i] < 0.0:
                print(nPE[i])
            if chan[i] <= 65 and (nPE[i]>=1):
            #if chan[i] <= 65 and (detector < detect):
                unique_layers.add(layers[i])
                unique_bars.add(chan[i])
                npelist.append(nPE[i])
                barlayerlist.append(layers[i])
                detectlist.append(detect)
                detectorlist.append(detector)
                hits = hits + 1
                timelist.append(time[i])
                layerlist.append(layers[i])
                unique_Chan_AL1Hit.add(chan[i])
                if layers[i] == 0:
                    lay0Time.append(time[i])
                    correctTime.append(time[i])
                if layers[i] == 1:
                    correctTime.append(time[i]-3.96)
                
                if layers[i] == 2:
                    correctTime.append(time[i]-(3.96 * 2))

                if layers[i] == 3:
                    lay3Time.append(time[i])
                    correctTime.append(time[i]-(3.96 * 3))


            else:
                #if (chan[i] == 71 or chan[i] == 75) and detector < detect:
                if (chan[i] == 71 or chan[i] == 75) and (nPE[i]>=1):
                    endcaphit = endcaphit + 1
                    endcapnpelist.append(nPE[i])
                    endcaplist.append(chan[i])
                else:
                    #if chan[i] > 66 and detector < detect:
                    if chan[i] > 66 and (nPE[i]>=1):
                        panelhit = panelhit + 1
                        panelnpelist.append(nPE[i])
                        panellist.append(chan[i])
        h.Fill(len(unique_layers))
        
        #one hit per layer
        if len(unique_layers) == 4  and len(unique_bars) ==4:
            minNPES = min(npelist)
            maxNPES = max(npelist)
            npeRatioS.Fill(maxNPES/minNPES)   
            if maxNPES/minNPES < 10:
                NPERatioCut += 1
                if max(correctTime)-min(correctTime) < 15.09:
                    corretTimeCut += 1
                


            #at least one hits per layer
            #if len(unique_layers) == 4:
            barh.Fill(len(unique_bars))
                           


            barh.Fill(len(unique_bars))
             
            #find the correct dT
               
            correct_timeDist.Fill(max(correctTime)-min(correctTime))

            #find dT(does this one is correct?yes)
            dT1 = abs(max(lay0Time)-min(lay3Time))
            dT2 = abs(max(lay3Time)-min(lay0Time))

            if dT1 > dT2:
                #if the last hit is at lay3 then fill + |dT| else fill -|dT| to histogram
                if max(lay3Time) > max(lay0Time):
                    timediffMaxh_FL.Fill(dT1)
                else:
                    timediffMaxh_FL.Fill(-dT1)
            else:
                if max(lay3Time) > max(lay0Time):
                    timediffMaxh_FL.Fill(dT2)
                else:
                    timediffMaxh_FL.Fill(-dT2)
            



            #the NPE & time separation histograms will be made if event pass 1 hit per layer & cosmic 1.
            if panelhit == 0:
               minNPE = min(npelist)
               maxNPE = max(npelist)
               npeRatioh.Fill(maxNPE/minNPE)   
        

               for npe in npelist:
                   npeh.Fill(npe)
                   


               dT1 = abs(max(lay0Time)-min(lay3Time))
               dT2 = abs(max(lay3Time)-min(lay0Time))
               
               if dT1 > dT2:
                   if max(lay3Time) > max(lay0Time):
                       timediffMaxh_COS2.Fill(dT1)
                   else:
                       timediffMaxh_COS2.Fill(-dT1)
               else:
                   if max(lay3Time) > max(lay0Time):
                       timediffMaxh_COS2.Fill(dT2)
                   else:
                       timediffMaxh_COS2.Fill(-dT2)
               



        # Check for exactly 4 hits with 1 hit in each layer
        if len(unique_layers) == 4:
            events_with_AL1HitsPl += 1
            print(f"{file_name} pass 4 layer cut") 
            #unique_Chan_AL1Hit.unfinihsed




        if len(unique_layers) == 4 and hits == 4:
            events_with_4_unique_hits += 1
            #print("we got one!")
            print("File {}, event {}".format(file_name,eventID))
            #print("all channel hit list, npe")
            #print(alllist)
            #print(alllistnpe)
            #print(arlayerlist, npe, prob to detect, detect rng, unique layer list")
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
        
        eventID = eventID+1

    # Close the file
    file.Close()

# Print number of entries in each bin
for bin in range(1, h.GetNbinsX() + 1):
    print(f"Bin {bin}: {h.GetBinContent(bin)} entries")

# Print the count of events with exactly 4 hits, 1 per layer
print(f"Event with 1 + hit per layer : {events_with_AL1HitsPl}")
print(f"Events with exactly 4 hits, 1 per layer: {events_with_4_unique_hits}")
print(f"Events with exactly 4 hits, 1 per layer, no panel hits: {events_with_no_panel_hit}")
print(f"Events with exactly 4 hits, 1 per layer, no panel or endcap hits: {events_with_no_endcap_hit}")
print(f"NPE ratio cut(after 1 hit per layer) : {NPERatioCut}")
print(f"correct time cut(after NPE ratio) : {corretTimeCut}")
output_file = ROOT.TFile("withPhotonbarCount.root", "RECREATE")
h.Write()
barh.Write()
npeRatioh.Write()
npeRatioS.Write()
npeh.Write()
timediffMaxh_FL.Write()
timediffMaxh_COS2.Write()
correct_timeDist.Write()
output_file.Close()
