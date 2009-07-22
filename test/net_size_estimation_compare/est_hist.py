#!/usr/bin/env python
import os, os.path, re, sys
import array, math
from pylab import *
import numpy as np

def meanstdv(x):
  from math import sqrt
  n, mean, std = len(x), 0, 0
  for a in x:
    mean = mean + a
  mean = mean / float(n)
  for a in x:
    std = std + (a - mean)**2
  std = sqrt(std / float(n-1) )
  return mean, std

file_name = sys.argv[1]
f1 = open(file_name,"r")

vals_c = [line.split() for line in f1 if line[0] != "#"]
c1 = [] #1. get average from local neighbors
c2 = [] #2. log N hops away
c3 = [] #3. from neighbors using 2, median
c4 = [] #4. from neighbors using 2, median
cost1 = [] #1. get average from local neighbors
cost2 = [] #2. log N hops away
cost3 = [] #3. from neighbors using 2, median
cost4 = [] #4. from neighbors using 2, median
for v in vals_c:
  if v[0] == "1":
    c1.append(int(v[1]))
    c2.append(int(v[2]))
    c3.append(int(v[3]))
    c4.append(int(v[4]))
    cost1.append(int(v[5])-2)
    cost2.append(int(v[6])-2)
    cost3.append(int(v[7])-2)
    cost4.append(int(v[8]))
c1_m, c1_s = meanstdv(c1)
c2_m, c2_s = meanstdv(c2)
c3_m, c3_s = meanstdv(c3)
c4_m, c4_s = meanstdv(c4)
cost1_m, cost1_s = meanstdv(cost1)
cost2_m, cost2_s = meanstdv(cost2)
cost3_m, cost3_s = meanstdv(cost3)
cost4_m, cost4_s = meanstdv(cost4)
#print '\tmean\tstdv'
print 'c1', '\t', c1_m, '\t', c1_s
print 'c2', '\t', c2_m, '\t', c2_s
print 'c3', '\t', c3_m, '\t', c3_s
print 'c4', '\t', c4_m, '\t', c4_s
print 'ct1', '\t', cost1_m, '\t', cost1_s
print 'ct2', '\t', cost2_m, '\t', cost2_s
print 'ct3', '\t', cost3_m, '\t', cost3_s
print 'ct4', '\t', cost4_m, '\t', cost4_s

"""
## drawing histograms of each method.
figure(1)
title("1. calculate distances from nearest neighbors (500,000 nodes)")
xlabel("Estimated Network Size")
ylabel("Number of Occurrence")
hist(c1,50)
savefig('1_hist_even_500k.eps')
figure(2)
title("2. calculate distance to logN hops-away node (500,000 nodes)")
xlabel("Estimated Network Size")
ylabel("Number of Occurrence")
hist(c2,50)
savefig('2_hist_even_500k.eps')
figure(3)
title("3. median of shortcut neighbors' estimation (500,000 nodes)")
xlabel("Estimated Network Size")
ylabel("Number of Occurrence")
hist(c3,50)
savefig('3_hist_even_500k.eps')
figure(4)
title("4. square-root estimation (500,000 nodes)")
xlabel("Estimated Network Size")
ylabel("Number of Occurrence")
hist(c4,50)
savefig('4_hist_even_500k.eps')
"""
def autolabel(rects):
  # attach some text labels
  for rect in rects:
    height = rect.get_height()
    text(rect.get_x()+rect.get_width()/2., 1.05*height, '%d'%int(height), ha='center', va='bottom')

N = 4
sizeMeans = [c1_m, c2_m, c3_m, c4_m]
sizeStd = [c1_s, c2_s, c3_s, c4_s]
costMeans = [cost1_m, cost2_m, cost3_m, cost4_m]
costStd = [cost1_s, cost2_s, cost3_s, cost4_s]
ind = np.arange(N)  #the x locations for the groups
width = 0.4  # the width of the bars
"""
#subplot(111)
figure(5)
rects1 = bar(ind, sizeMeans, width, color='r', yerr=sizeStd)
ylabel('Estimated Network Size')
title('Estimated Network Size by Each Method (500,000 nodes)')
xticks(ind+width/2., ('M1', 'M2', 'M3', 'M4'))
autolabel(rects1)
savefig('size_meanstd_even_500k.eps')
figure(6)
rects2 = bar(ind, costMeans, width, color='r', yerr=costStd)
ylabel('Number of Messages')
title('Communication Cost by Each Method (500,000 nodes)')
xticks(ind+width/2., ('M1', 'M2', 'M3', 'M4'))
autolabel(rects2)
savefig('cost_meanstd_500k.eps')

#rects1 = bar(ind, sizeMeans, width, color='r', yerr=sizeStd)

"""

