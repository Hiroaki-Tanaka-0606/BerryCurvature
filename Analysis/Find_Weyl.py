# Find Weyl points

import sys
import os
import re
import subprocess

file="input_Weyl.band2.dat"
minDiff=0
minDiff_line=""
minDiff_row=-1
lineNumber=0
index=12
with open(file, "r") as f:
    while True:
        line=f.readline()
        lineNumber+=1
        if len(line)==0:
            break
        line=line.rstrip().lstrip()
        if len(line)==0:
            continue
        # skip comment
        if line[0]=="#":
            continue
        dataArr=re.split(r"\s+", line)
        diff=float(dataArr[index+4])-float(dataArr[index+3])
        if minDiff_row<0 or diff<minDiff:
            minDiff=diff
            minDiff_line=line
            minDiff_row=lineNumber
            

print(("{0:e}").format(minDiff))
print(minDiff_line)
print(("Line {0:d}").format(minDiff_row))
          
