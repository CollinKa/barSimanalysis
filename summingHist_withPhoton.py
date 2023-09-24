import os
import re


event_sums = {}

input_files = []

directory= "/net/cms26/cms26r0/zheng/barSimulation/withOutPhotonAnalysis/resultWithoutPhoton/"
base_name = "hist"

interestEventlist = []
interestEvent = "interestEvent_threeInaLine.txt"

for filename in os.listdir(directory):
    if filename.startswith(base_name) and filename.endswith(".txt"):
        input_files.append(directory+filename)

for filepath in input_files:
    with open(filepath, 'r') as file:
        lines = file.readlines()
        for line in lines:
            if ':' in line:
                parts = line.split(':')

                event = parts[0]
                eventNumber = parts[1]
                if event == "totoal events": continue


                #grep the file number
                match = re.search(r'hist(\d+)\.txt', filepath)
                file_number = match.group(1)
                eventName = str(event)
                if eventName == "CTHreeinaline ":
                    #print(event)
                    #print(eventNumber)
                    #print(file_number)
                    info=str(event) + "  " + str(file_number) + "  " + str(eventNumber)
                    interestEventlist.append(info)


with open(interestEvent, 'w') as file:
    file.write("event type     file number     event number" + "\n")
    for string in interestEventlist:
        file.write(string + "\n")