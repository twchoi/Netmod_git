#!/usr/bin/env python
import sys, math
from pylab import *

infilename = sys.argv[1]
ifile = open(infilename, 'r')
no_cache = 0
c_time = []
cache =[]
#stab = {}
cost = []
size = []
c_sum = 0
s_sum = 0
time = 0
count = 0
for line in ifile:
  data = line.split()
  action = data[1]
  time = float(data[0])
  if action == "caching" :
    if count is not 0:
      no_cache = no_cache + 1
      c_time.append( time )
      cache.append(no_cache)
      cost.append(c_sum/count)
      size.append(s_sum/count)
      c_sum = 0
      s_sum = 0
      count = 0
  elif action == "Node_Join":
    if no_cache != 0:
      #time = float(data[0])
      c_sum += int(data[6])
      s_sum += int(data[2])
      count += 1
      stab[time] = (size, cost)
  else:
    st = 0
#cost.append(c_sum/count)
#size.append(s_sum/count)
#c_time.append(time)
print 'time,	size,	cost'
for t in xrange(len(c_time) ):
  print c_time[t], size[t], cost[t]
"""
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
figure()
plot(c_time, cache)
show()
"""
