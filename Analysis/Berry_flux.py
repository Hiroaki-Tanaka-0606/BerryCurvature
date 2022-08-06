# Berry flux calculations

import sys
import os
import re
import subprocess

file="input_Weyl.dat"
width=0.1
numPoints=500

exe="./berry.o"

dk=width/numPoints
offset=dk*(numPoints-1)/2

if not os.path.isfile(file):
    print(("Error: {0:s} does not exist").format(file))
    sys.exit()
    
print(("Input file: {0:s}").format(file))

file_xp=open(file+".xp", "w")
file_xm=open(file+".xm", "w")
file_yp=open(file+".yp", "w")
file_ym=open(file+".ym", "w")
file_zp=open(file+".zp", "w")
file_zm=open(file+".zm", "w")

def writeAll(line):
    file_xp.write(line)
    file_xm.write(line)
    file_yp.write(line)
    file_ym.write(line)
    file_zp.write(line)
    file_zm.write(line)

with open(file, "r") as f:
    # Parameters
    line=f.readline()
    writeAll(line)    
    numParams=int(re.findall(r"(\S+)\s", line)[0])
    print(("Number of parameters: {0:d}").format(numParams))
    for i in range(numParams):
        line=f.readline()
        writeAll(line)
    # Equations
    line=f.readline()
    writeAll(line)    
    numEqns=int(re.findall(r"(\S+)\s", line)[0])
    print(("Number of equations: {0:d}").format(numEqns))
    for i in range(numEqns):
        line=f.readline()
        writeAll(line)
    # Matrix
    line=f.readline()
    writeAll(line)    
    matSize=int(re.findall(r"(\S+)\s", line)[0])
    print(("Matrix size: {0:d}").format(matSize))
    for i in range(matSize):
        line=f.readline()
        writeAll(line)
    # Basis, should be the elementary matrix
    for i in range(3):
        line=f.readline()
    writeAll("1 0 0\n")
    writeAll("0 1 0\n")
    writeAll("0 0 1\n")
    # Eigen range (discarded)
    for i in range(3):
        line=f.readline()
        writeAll(line)
    # Berry range, the start point should be the center
    line=f.readline()
    kx=float(re.findall(r"(\S+)\s", line)[0])
    line=f.readline()
    ky=float(re.findall(r"(\S+)\s", line)[0])
    line=f.readline()
    kz=float(re.findall(r"(\S+)\s", line)[0])
    print(("Center: ({0:.8f}, {1:.8f}, {2:.8f})").format(kx, ky, kz))

    file_xp.write(("{0:.8f} {0:.8f} 0\n").format(kx+offset))
    file_xp.write(("{0:.8f} {1:.8f} {2:d}\n").format(ky-offset, ky+offset, numPoints-1))
    file_xp.write(("{0:.8f} {1:.8f} {2:d}\n").format(kz-offset, kz+offset, numPoints-1))
    
    file_xm.write(("{0:.8f} {0:.8f} 0\n").format(kx-offset))
    file_xm.write(("{0:.8f} {1:.8f} {2:d}\n").format(ky-offset, ky+offset, numPoints-1))
    file_xm.write(("{0:.8f} {1:.8f} {2:d}\n").format(kz-offset, kz+offset, numPoints-1))
    
    file_yp.write(("{0:.8f} {1:.8f} {2:d}\n").format(kx-offset, kx+offset, numPoints-1))
    file_yp.write(("{0:.8f} {0:.8f} 0\n").format(ky+offset))
    file_yp.write(("{0:.8f} {1:.8f} {2:d}\n").format(kz-offset, kz+offset, numPoints-1))
    
    file_ym.write(("{0:.8f} {1:.8f} {2:d}\n").format(kx-offset, kx+offset, numPoints-1))
    file_ym.write(("{0:.8f} {0:.8f} 0\n").format(ky-offset))
    file_ym.write(("{0:.8f} {1:.8f} {2:d}\n").format(kz-offset, kz+offset, numPoints-1))

    file_zp.write(("{0:.8f} {1:.8f} {2:d}\n").format(kx-offset, kx+offset, numPoints-1))
    file_zp.write(("{0:.8f} {1:.8f} {2:d}\n").format(ky-offset, ky+offset, numPoints-1))
    file_zp.write(("{0:.8f} {0:.8f} 0\n").format(kz+offset))
    
    file_zm.write(("{0:.8f} {1:.8f} {2:d}\n").format(kx-offset, kx+offset, numPoints-1))
    file_zm.write(("{0:.8f} {1:.8f} {2:d}\n").format(ky-offset, ky+offset, numPoints-1))
    file_zm.write(("{0:.8f} {0:.8f} 0\n").format(kz-offset))
    
    # delta k
    line=f.readline()
    writeAll(line)
    
file_xp.close()
file_xm.close()
file_yp.close()
file_ym.close()
file_zp.close()
file_zm.close()

suffixes=["xp","xm","yp","ym","zp","zm"]
# execute berry.o
for suffix in suffixes:
    command=("{0:s} {1:s}.{2:s} > {1:s}.{2:s}.out").format(exe, file, suffix)
    print(command)
    subprocess.run(command, shell=True)

# sum
indices=[0,0,1,1,2,2]
coeffs=[1,-1,1,-1,1,-1]

numData=3+matSize*3
numRows=numPoints*numPoints

berry_sum=[]
for i in range(matSize):
    berry_sum.append(0.0)

for index, suffix in enumerate(suffixes):
    outName=("{0:s}.{1:s}.out").format(file, suffix)
    print(("Open {0:s}").format(outName))

    readData=0
    with open(outName, "r") as f:
        while True:
            line=f.readline()
            if len(line)==0:
                break
            line=line.rstrip().lstrip()
            if len(line)==0:
                continue
            # skip comment
            if line[0]=="#":
                continue
            dataArr=re.split(r"\s+", line)
            # print(dataArr)
            if len(dataArr)>=numData:
                readData+=1
                for i in range(matSize):
                    berry_sum[i]+=float(dataArr[3+3*i+indices[index]])*dk*dk*coeffs[index]
            else:
                # print(("{0:d} {1:d}").format(len(dataArr),numData))
                pass
        if readData==numRows:
            print("OK")
        else:
            print("Not OK")
            print(("{0:d} {1:d}").format(readData, numRows))
            

print(berry_sum)
    
          
