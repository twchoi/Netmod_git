#!/usr/bin/env python
import sys, math
from pylab import *
def whichwindow(x, list):
  #idx = list.index(x)
  for i in list:
    if x > i:
      idx = list.index(i)
  return idx

def calculateresult(windows):
  result = [[666.0,896,0,1]]
  for win in windows:
    time = win[-1][0]
    size = float(sum(y for [x,y,z,w] in win) )/ float(len(win) )
    cache = float(sum(w for [x,y,z,w] in win) ) / float(len(win) )
    cost = float(sum(z for [x,y,z,w] in win) ) / float(len(win) )
    result.append([time,size,cost,cache])
  return result 

infilename = sys.argv[1]
ifile = open(infilename, 'r')

result = []
lst = [i for i in arange(0,40000000, 3600) ]
win = [ [] for k in arange(len(lst) ) ]

for line in ifile:
  data = line.split()
  time = round(float(data[0]) )
  size = int(data[1])
  cost = int(data[2])
  cache = int(data[3])
  index = whichwindow(time,lst)
  #print time, size, cost, cache, index
  win[index].append([time, size, cost, cache])
#print 'win', win

output = calculateresult(win)
print 'time    size    cost    cache'
for out in output:
  print out[0], out[1], out[2], out[3]
#print output
