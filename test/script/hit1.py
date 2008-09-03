#!/usr/bin/env python
import sys, math
from pylab import *

def hitf(a):
  r = 0
  if (a >= 1):
    r = 1
  else:
    r = 0
  return r
def mean(b):
  return float(sum(b)) / float(len(b))

#infilename = "d2out"
infilename = sys.argv[1]
#alpha = float(sys.argv[2])
ifile = open(infilename, 'r')
no_cache = 0
time = []
stab_cost = []
cache_time = []
#tmp_cr = [] 
tmp_cr = 0 
for line in ifile:
  data = line.split()
  action = data[1]
  if action == "caching":
    #print action
    no_cache = no_cache + 1
    cache_time.append(float(data[0]))
  elif action == "Node_Join":
    #print action
    if no_cache != 0:
      time.append(float(data[0]))
      cost = float(data[6])
      cost_rate = cost / float(no_cache)
      #tmp_cr.append(cost_rate)
      tmp_cr = tmp_cr + cost_rate
      f_res = float(tmp_cr) / float(len(stab_cost) + 1) 
      #stab_cost.append(mean(tmp_cr))
      stab_cost.append(f_res)
      #print no_cache, cost, cost_rate
  else:
    st = 0
    #leave or query. handle later

#print stab_cost
print len(time), len(stab_cost)
#print cache_time
for i in xrange(len(time)):
  print time[i],    stab_cost[i]
print '                       '
for j in xrange(len(cache_time)):
  print cache_time[j], 0
"""
figure(1)
plot(time, stab_cost)
xlabel('time')
ylabel('number of messages per unique object')
title('Stabilization Cost')
grid(True)
#figure(2)
#plot(num_ave)
#figure(3)
#plot(cost)
show()
"""
