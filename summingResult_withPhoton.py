import os

event_sums = {}

input_files = []

directory= "/net/cms26/cms26r0/zheng/barSimulation/withPhotonAnalysis/resultsWithPhoton/"
base_name = "file"

for filename in os.listdir(directory):
    if filename.startswith(base_name) and filename.endswith(".txt"):
        input_files.append(directory+filename)

#print(input_files)

for filepath in input_files:
    with open(filepath, 'r') as file:
        lines = file.readlines()
        for line in lines:
            if ':' in line:
                parts = line.split(':')

                event = parts[0]
                count = parts[1]
                count = int(count.strip())

                # Update the count in the dictionary
                if event in event_sums:
                    event_sums[event] += count
                else:
                    event_sums[event] = count
            #else:
            #    event = line
            #    count = []

for event, count in event_sums.items():
    print(f"{event}: {count}")