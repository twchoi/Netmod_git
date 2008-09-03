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
time = []
tmp = []
ave_hit = []
cache_time = []
no_cache = 0
for line in ifile:
  data = line.split()
  action = data[1]
  if action == 'caching':
    no_cache = no_cache + 1
    cache_time.append(float(data[0]) )
  elif action == 'querying':
    time.append(float(data[0]))
    hit = hitf(int(data[4]))
    tmp.append(hit)
    ave_hit.append(mean(tmp))
th = []
y = 1-math.exp(-1)
for x in xrange(len(time)):
  th.append(y)
"""  
hit = [int(line.split()[4]) for line in ifile]
ifile = open(infilename, 'r')
cost = [int(line.split()[5]) for line in ifile]
#print cost
print mean(cost)
depth = [int(line.split()[6]) for line in ifile]
#print hit
res = []
num_ave = []
for i in xrange(len(hit)):
  k = hit[i]
  j = hitf(k)
  #print k, j
  res.append(j)
  num_ave.append(mean(res))
#print res
su = 1 
mov_ave = []
for j in res:
  su = (1-alpha)*su + alpha * j
  mov_ave.append(su)
  #print su
print 'moving average: ', su
#print res
print 'numerical average: ', mean(res)
figure(1)
plot(mov_ave)
figure(2)
plot(num_ave)
figure(3)
plot(cost)
"""
figure(1)
plot(time,ave_hit,'b',time,th, 'r--')
#plot(time,th,label='theory')
legend(('simulation','theory'),'upper right', shadow=True)
xlabel('time')
ylabel('hit rate')
title('Hit Rate')
grid(True)
show()
