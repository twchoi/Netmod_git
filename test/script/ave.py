#!/usr/bin/env python
import sys, math
from pylab import *
def whichwindow(x):
  list = [ j for j in arange(0, 36000000, 3600) ]
  #idx = list.index(x)
  for i in list:
    if x > i:
      idx = list.index(i)
  return idx

def caclulateresult(data, windows):
  for win in windows:
    cost_ave = sum(z for [x,y,z,w] in win) / len(win)
    size_ave = sum(y for [x,y,z,w] in win) / len(win)
    cache_ave = sum(w for [x,y,z,w] in win) / len(win)
    t = win[-1][0]
    time.append(time)
    size.append(size_ave)
    cost.append(cost_ave)
    cache.append(cache_ave)
  return time, size, cost, cache

infilename = sys.argv[1]
ifile = open(infilename, 'r')

result = []
win = [[]]

print whichwindow(666)
for line in ifile:
  data = line.split()
  time = float(data[0])
  size = int(data[1])
  cost = int(data[2])
  cache = int(data[3])
  index = whichwindow(time)
  print time, size, cost, cache, index
  win[index].append = [time,cost, size, cache]

print win 
