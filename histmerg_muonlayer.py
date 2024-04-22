import ROOT

# Open the ROOT file
OLfile = ROOT.TFile.Open("offlineMerge.root")
simfile = ROOT.TFile.Open("simMerge.root")
# Access the histograms (replace "hist_name" with the actual names of your histograms)
hist1 = OLfile.Get("M_NPE_C")
hist2 = simfile.Get("M_NPE_C")

# Normalize histograms
hist1.Scale(1.0 / hist1.Integral())
hist2.Scale(1.0 / hist2.Integral())

# Create a canvas to draw histograms
canvas = ROOT.TCanvas("canvas", "Histograms", 800, 600)
hist1.GetYaxis().SetRangeUser(0, 1)
hist2.GetYaxis().SetRangeUser(0, 1)
# Draw the first histogram
hist1.Draw()

# Draw the second histogram on the same canvas with a different color or style
hist2.SetLineColor(ROOT.kRed)  # Change line color to red
hist2.Draw("SAME")  # Draw on the same canvas

# Add legend (optional)
legend = ROOT.TLegend(0.7, 0.7, 0.9, 0.9)
legend.AddEntry(hist1, "offline", "l")
legend.AddEntry(hist2, "sim", "l")
legend.Draw()

# Update canvas
canvas.Update()


# Create a new ROOT file
output_file = ROOT.TFile("output_file_muonlayer.root", "RECREATE")


# Save the canvas as an image (optional)
canvas.Write()

# Close the ROOT file
OLfile.Close()
simfile.Close()