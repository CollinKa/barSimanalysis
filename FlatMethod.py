"""


"""





import math
import random


# this is used for without photon data
def geometricCut_noP(layers, chan, nPE):
    NPE_Cut = 0.6 # based on previous analysis, NPE >= 0.6 can reach the performance like P cut.
    events_AL_4_layer_got_hits = False
    events_with_4_unique_hits = False
    cosPanel_hit = True
    beamPanel_hit = True
    beamPanelNPE = 0   
    hits = 0 


    unique_layers = set()

    for i in range(len(layers)):
        if (chan[i] <= 65) and (nPE[i] >= NPE_Cut):
            unique_layers.add(layers[i])
            hits = hits + 1
        
        if (chan[i] == 71 or chan[i] == 75)and (nPE[i] >= NPE_Cut):
            beamPanelNPE += nPE[i]

        
        if (chan[i] >= 68 and chan[i] != 71 and chan[i] != 75)and (nPE[i] >= NPE_Cut):
            cosPanel_hit = False

        
    if len(unique_layers) == 4:
        events_AL_4_layer_got_hits = True

    if len(unique_layers) == 4 and hits == 4:
        events_with_4_unique_hits = True


    #if the sum of deposited energy on beam panel reach 1MeV, then we claim it fail the beamVeto cut.
    #The nPE/MeV ratio for beam panels is 75.1
    #Ryan suggest to change it to 50 because 75 can't do any useful cut
    if beamPanelNPE >= 50:
        beamPanel_hit = False
    
    return events_AL_4_layer_got_hits,events_with_4_unique_hits,cosPanel_hit,beamPanel_hit

# this method can only being used for without photon data. beacaseu with photon data doesn't need to use P cut!
def geometricCut_P(layers, chan, nPE):
    
    events_AL_4_layer_got_hits_p = False
    events_with_4_unique_hits_p = False
    cosPanel_hit_p = True
    beamPanel_p = True

    unique_layers_p = set()
    hits_p=0
    beamPanelNPE_p = 0


    for i in range(len(layers)):
        if nPE[i] == None:
            continue
        if nPE[i] < 0:
            nPE[i] = 0 #temporary solution for dealing with negative energy
        detect = 1-math.exp(-1*nPE[i])
        detector = random.random()

        if chan[i] <= 65 and (detector < detect):
            unique_layers_p.add(layers[i])
            hits_p = hits_p + 1
        

        if (chan[i] == 71 or chan[i] == 75)and (detector < detect):
            beamPanelNPE_p += nPE[i]

        if (chan[i] >= 68 and chan[i] != 71 and chan[i] != 75)and (detector < detect):
            cosPanel_hit_p = False


    if len(unique_layers_p) == 4:
        events_AL_4_layer_got_hits_p = True

    if len(unique_layers_p) == 4 and hits_p == 4:
        events_with_4_unique_hits_p = True

    if beamPanelNPE_p >= 50:
        beamPanel_p = False
    
    

    
    return events_AL_4_layer_got_hits_p,events_with_4_unique_hits_p,cosPanel_hit_p,beamPanel_p



def geometricCut_WithPhoton(layers, chan, nPE):

    events_AL_4_layer_got_hits = False
    events_with_4_unique_hits = False
    
    cosPanel_hit = False
    beamPanel_hit = False
    beamPanelNPE = 0


    unique_layers = set()


    hits=0



    for i in range(len(layers)):


        if chan[i] <= 65 and (nPE[i] >= 1):
            unique_layers.add(layers[i])
            hits = hits + 1
        
        if (chan[i] == 71 or chan[i] == 75 ) and (nPE[i] >= 1):
            beamPanelNPE += 1

        if (chan[i] >= 68 and chan[i] != 71 and chan[i] != 75) and (nPE[i] >= 1):
            cosPanel_hit = False
        

    
    if len(unique_layers) == 4:
        events_AL_4_layer_got_hits = True

    if len(unique_layers) == 4 and hits == 4:
        events_with_4_unique_hits = True
    
    if beamPanelNPE >= 50:
        beamPanel_hit = False
        

    
    return events_AL_4_layer_got_hits,events_with_4_unique_hits, cosPanel_hit, beamPanel_hit

def timeCutManipulation(Lay0Time,Lay1Time,Lay2Time,Lay3Time):
    if len(Lay0Time) == 0 or len(Lay1Time) == 0 or len(Lay2Time) == 0 or len(Lay3Time) == 0:
        return False
    dT = 3.96 #The shortest time for photon travel 1 bar scitillator + 1 air gap between two bars.
    Time = []
    for time0 in Lay0Time:
        Time.append(time0)
    for time1 in Lay1Time:
        Time.append(time1-dT)
    for time2 in Lay2Time:
        Time.append(time2-2*dT)
    for time3 in Lay3Time:
        Time.append(time3-3*dT)
    
    return max(Time)-min(Time) <= 15.09

    

#NPE,time 
def NPE_TimeCut_withPhoton(chan,layer,nPE,time):
    NPECut = True
    TimeCut = False
    nPEList = list()   
    Lay0time = list()
    Lay1time = list()
    Lay2time = list()
    Lay3time = list()
    nPEList = []

    for i in range(len(layer)):
        if chan[i] <= 65 and (nPE[i] >= 1):
            nPEList.append(nPE[i])
            if layer[i] == 0:
                Lay0time.append(time[i])
            if layer[i] == 1:
                Lay1time.append(time[i])
            if layer[i] == 2:
                Lay2time.append(time[i])
            if layer[i] == 3:  
                Lay3time.append(time[i])

    TimeCut=timeCutManipulation(Lay0time,Lay1time,Lay2time,Lay3time)
    

    if len(nPEList) == 0:
        return False, False

    maxNPE = max(nPEList)
    minNPE = min(nPEList)
    

    if maxNPE/minNPE > 10:
        NPECut = False      

    TimeCut = TimeCut and NPECut

    return NPECut,TimeCut

def NPE_TimeCut_P(chan,layer,nPE,time):
    NPECut = True
    TimeCut = False 
    Lay0time = list()
    Lay1time = list()
    Lay2time = list()
    Lay3time = list()
    nPEList = []

    for i in range(len(layer)):
        if nPE[i] < 0:
            nPE[i] = 0
        detect = 1-math.exp(-1*nPE[i])
        detector = random.random()

        if chan[i] <= 65 and (detector < detect):
            nPEList.append(nPE[i])
            if layer[i] == 0:
                Lay0time.append(time[i])
            if layer[i] == 1:
                Lay1time.append(time[i])
            if layer[i] == 2:
                Lay2time.append(time[i])
            if layer[i] == 3:  
                Lay3time.append(time[i])

    TimeCut=timeCutManipulation(Lay0time,Lay1time,Lay2time,Lay3time)

    if len(nPEList) == 0:
        return False, False

    maxNPE = max(nPEList)
    minNPE = min(nPEList)
    
    if maxNPE/minNPE > 10:
        NPECut = False      

    TimeCut = TimeCut and NPECut

    return NPECut,TimeCut


def NPE_TimeCut_withoutPhoton(chan,layer,nPE,time):
    NPECut = True
    TimeCut = False 
    Lay0time = list()
    Lay1time = list()
    Lay2time = list()
    Lay3time = list()
    nPEList = []

    for i in range(len(layer)):

        if chan[i] <= 65 and (nPE[i] > 0.6):
            nPEList.append(nPE[i])
            if layer[i] == 0:
                Lay0time.append(time[i])
            if layer[i] == 1:
                Lay1time.append(time[i])
            if layer[i] == 2:
                Lay2time.append(time[i])
            if layer[i] == 3:  
                Lay3time.append(time[i])

    TimeCut=timeCutManipulation(Lay0time,Lay1time,Lay2time,Lay3time)

    if len(nPEList) == 0:
        return False, False

    maxNPE = max(nPEList)
    minNPE = min(nPEList)
    

    if maxNPE/minNPE > 10:
        NPECut = False      

    TimeCut = TimeCut and NPECut 

    return NPECut,TimeCut


#method used for debugging

#check if the issue of summing NPE > 0 but summing Energy < 0 still exist

#PMT_NPE: withphoton data from the pmt branch
#SCINT_NPE: withoutphoton data from the scint branch
def EDPNPEdebug(PMT_NPE,SCINT_NPE):
    for P_NPE,S_NPE in zip(PMT_NPE,SCINT_NPE):
        if S_NPE <=0 and P_NPE >= 1:
            #return flase when the issue occurs
            return False
    
    return True





