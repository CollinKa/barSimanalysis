"""
10-21 
the goal of this file is to normalized histograms in folder OSU_quickCheck


"""


import ROOT

root_file = ROOT.TFile("/net/cms26/cms26r0/zheng/barSimulation/withPhotonAnalysis/OSU_quickCheck/NPEMerged.root", "UPDATE")

root_file_objects = root_file.GetListOfKeys()

for obj in root_file_objects:
    single_hist_name = obj.GetName()
    #if single_hist_name == "Chan NPE  distribution":
    print(single_hist_name)
    if "Chan NPE  distribution" == single_hist_name:
    #hist1=root_file.Get("Chan NPE distribution")
        hist1 = root_file.Get(single_hist_name)
        hist1.Scale(1.0 / hist1.Integral())
        hist1.Write("Normalized NPE(chan based) vs Chan")
    
    if "NPE  distribution" == single_hist_name:
        hist2 = root_file.Get(single_hist_name)
        hist2.Scale(1.0 / hist2.Integral())
        hist2.Write("Normalized NPE distribution(chan based)")



root_file.Close()


