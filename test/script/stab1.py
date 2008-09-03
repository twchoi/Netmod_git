#!/usr/bin/env python
import sys, math
from pylab import *

infilename = sys.argv[1]
ifile = open(infilename, 'r')
time = []
size = []
cost = []
cache =[]
no_cache = 0
cst = 0
theory = []
for line in ifile:
  data = line.split()
  action = data[1]
  tm = float(data[0])
  sz = int(data[2])
  time.append(tm)
  size.append(sz)
  #cst = 0
  if action == "caching" :
      no_cache = no_cache + 1
  else:
      #time = float(data[0])
      cst = int(data[6])
  cost.append(cst)
  cache.append(no_cache)
  #print no_cache, sz
  th = 2 * (no_cache / math.sqrt(sz) )
  theory.append(th)

#print 'list_size', len(time)
"""
for t in xrange(len(time) ):
  print time[t], size[t], cost[t], cache[t]
count = 0
size_ave = []
cost_ave = []
#for t in xrange(0,36000000,1000):
for c_t in c_time:
  pre_ct = 0
  for t in stab.keys():
    if c_t is not 0:
      if (t >= pre_ct and t < c_t):
	s_sum = s_sum + stab[t][0]
        c_sum = c_sum + stab[t][1]
        count += 1
      else:
	continue
   size_ave.append

#ave_stab = {}
#for iter in xrange(0,100000000, 1000):
#  n_sum = n_sum + stab
print len(time), len(stab_cost)
for i in xrange(len(time)):
  print time[i],    stab_cost[i]
print '                       '
for j in xrange(len(cache_time)):
  print cache_time[j], 0
"""
figure(1)
plot(time, cost, 'r-', time, theory)
xlabel('time')
ylabel('number of messages per unique object')
title('Stabilization Cost')
grid(True)
#figure(2)
#plot(num_ave)
#figure(3)
#plot(cost)
figure()
#plot(c_time, cache)
show()
