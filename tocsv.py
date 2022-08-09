import csv
import sys
import os
import numpy as np

#python3 ./tocsv.py ./solution/saida.txt

filepath = sys.argv[-1]

avg = 0
n = 0
best = np.Infinity
best_seed = 0
instance = ''

for filename in os.listdir(filepath):
    with open(os.path.join(filepath, filename), 'r') as file:
        instance = file.readline().split()[-1].rstrip('\n')
        seed = file.readline().split()[-1].rstrip('\n')
        file.readline()
        qual = float(file.readline().split()[-1])
        avg += qual
        n += 1
        if qual < best:
            best = qual
            best_seed = seed

avg = avg/n

with open("solution/solutions.csv", mode='a') as outfile:
    outfile = csv.writer(outfile, delimiter=',')
    outfile.writerow([instance, avg, best, best_seed, n])