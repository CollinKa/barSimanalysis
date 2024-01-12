import math
import random

#it can only returns two output!! fix it !



# this method can only being used for without photon data. beacaseu with photon data doesn't need to use P cut!
def geometricCut(layers, chan, nPE)->tuple[bool, bool, bool,bool]:

    events_AL_4_layer_got_hits = False
    events_with_4_unique_hits = False
    events_AL_4_layer_got_hits_p = False
    events_with_4_unique_hits_p = False
    cosPanel_hit = False
    beamPanel = False
    cosPanel_hit_p = False
    beamPanel_p = False


    unique_layers = set()
    barlayerlist = []


    hits=0

    unique_layers_p = set()
    barlayerlist_p = []
    hits_p=0


    for i in range(len(layers)):
        detect = 1-math.exp(-1*nPE[i])
        detector = random.random()

        if chan[i] <= 65 and nPE[i]>0:
            unique_layers.add(layers[i])
            barlayerlist.append(layers[i])
            hits = hits + 1
        
        

        if chan[i] <= 65 and (detector < detect):
            unique_layers_p.add(layers[i])
            barlayerlist_p.append(layers[i])
            hits_p = hits_p + 1
        
        if (chan[i] == 71 or chan[i] == 75)and nPE[i]>0:
            beamPanel = True

        if (chan[i] == 71 or chan[i] == 75)and (detector < detect):
            beamPanel_p = True

        if (chan[i] >= 68 and chan[i] != 71 and chan[i] != 75)and nPE[i]>0:
            cosPanel_hit = True
        
        if (chan[i] >= 68 and chan[i] != 71 and chan[i] != 75)and (detector < detect):
            cosPanel_hit_p = True

    
    if len(unique_layers) == 4:
        events_AL_4_layer_got_hits = True

    if len(unique_layers) == 4 and hits == 4:
        events_with_4_unique_hits = True
    
    cosPanel_hit = cosPanel_hit and events_with_4_unique_hits

    beamPanel = beamPanel and cosPanel_hit


    if len(unique_layers_p) == 4:
        events_AL_4_layer_got_hits_p = True

    if len(barlayerlist_p) == 4 and hits == 4:
        events_with_4_unique_hits_p = True

    cosPanel_hit_p = cosPanel_hit_p and events_with_4_unique_hits_p

    beamPanel_p = beamPanel_p and cosPanel_hit_p

    
    

    
    return events_AL_4_layer_got_hits,events_with_4_unique_hits,events_AL_4_layer_got_hits_p,events_with_4_unique_hits_p


# this one doesn't come with probability cut. It can work with any kind of data
def geometricCut_WithPhoton(layers, chan, nPE)->tuple[bool, bool]:

    events_AL_4_layer_got_hits = False
    events_with_4_unique_hits = False
    
    cosPanel_hit = False
    beamPanel = False



    unique_layers = set()
    barlayerlist = []


    hits=0



    for i in range(len(layers)):


        if chan[i] <= 65 and nPE[i]>0:
            unique_layers.add(layers[i])
            barlayerlist.append(layers[i])
            hits = hits + 1
        
        if (chan[i] == 71 or chan[i] == 75)and nPE[i]>0:
            beamPanel = True

        if (chan[i] >= 68 and chan[i] != 71 and chan[i] != 75)and nPE[i]>0:
            cosPanel_hit = True
        

    
    if len(unique_layers) == 4:
        events_AL_4_layer_got_hits = True

    if len(unique_layers) == 4 and hits == 4:
        events_with_4_unique_hits = True
    
    cosPanel_hit = cosPanel_hit and events_with_4_unique_hits

    beamPanel = beamPanel and cosPanel_hit

    
    return events_AL_4_layer_got_hits,events_with_4_unique_hits


# panel,NPE,time TBD
