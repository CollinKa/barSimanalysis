"""
12-5 this file is created for using in the  flat tree version's simulation root file

data location: /net/cms26/cms26r0/schmitz/milliQanFlatSim/


there is a need to understand how to use eventID. why does this value ranges from 0-1000
"""


import numpy as np
import pandas as pd
import ROOT as r
import os 
import sys
import time
import uproot 
from functools import partial
from InputConstants import uprootInputs



class triggerChecker():


	#function to open the input root file
	def openFile(self, dataIn):

		fin = uproot.open(dataIn)
		tree = fin['t']
		self.myarray = tree.arrays(uprootInputs, library='pd')

		self.geometricCut(data)	

	def geometricCut(self,data):
		NumEvent=data["eventID"].max() + 1
		if np.isnan(NumEvent): return
		Al1HitEventN = 0

		for event in range(NumEvent): #event based
			selected_data = data[data["eventID"] == event]
			layer_list = selected_data["layer"]
			layer_set = set(layer_list)
			if  len(layer_set) == 4:
				Al1HitEventN +=1
		
		print(Al1HitEventN)



if __name__ == "__main__":

	r.gROOT.SetBatch(1)

	mychecker = triggerChecker()

	data = "/flatRootFolder/output_995.root"

	mychecker.openFile(data)

