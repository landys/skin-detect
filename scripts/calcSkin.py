# This code is used to calculate skin percentage of the test result
import sys

def usage():
    print '''USAGE: python calcSkin.py targetFile interval column resultFile
Options: column is start from 0'''

if len(sys.argv) < 5:
    usage()
    sys.exit()

interval = float(sys.argv[2])
column = int(sys.argv[3])
print "targetFile="+sys.argv[1]
print "interval="+str(interval)
print "column="+str(column)
print "resultFile="+sys.argv[4]

testFile = open(sys.argv[1])
count = 0
MAX_NUM = 100
bigIndex = 0
re = []
for i in range(0, 100):
    re.append(0)
realCount = 0
for line in testFile:
    count += 1
    if (count == 1 or len(line) == 0):
	continue

    lineData = line.split()
    if (len(lineData) < column + 1):
	continue
    realCount += 1
    value = float(lineData[column])
    index = int(value / interval)
    re[index] += 1

    if (bigIndex < index):
	bigIndex = index
testFile.close()

print "count="+str(count)
print "realCount="+str(realCount)

print "bigIndex ="+str(bigIndex)

reFile = open(sys.argv[4], "w")
reFile.write("keypoints\t\tpics\t\ttotalPics\n")
totalPics = 0
for i in range(0, bigIndex+1):
    totalPics += re[i]
    reFile.write("<"+str((i+1)*interval)+"\t\t"+str(re[i])+"\t\t"+str(totalPics)+'\n')

reFile.close()
