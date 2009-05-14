#!/usr/bin/env python

# This code is for drawing stabilization cost figure.
# To utilize this code, res.gz file is required.
# twchoi@twlab:~/code/netmod1-dev/test/script$ gzip -d < res.gz | ./ave.py

import sys, math
from pylab import *
def whichwindow(x, list):
  #idx = list.index(x)
  for i in list:
    if x > i:
      idx = list.index(i)
  return idx

def calculateresult(windows):
  result = [[666.0,896,0,1,0,0]]
  for win in windows:
    time = win[-1][0]
    size = float(sum(y for [x,y,z,w] in win) )/ float(len(win) )
    cache = float(sum(w for [x,y,z,w] in win) ) / float(len(win) )
    cost = float(sum(z for [x,y,z,w] in win) ) / float(len(win) )
    u_bound = 2*cache / math.sqrt(size)
    #this 'theory' is current acctpted theory
    #theory = (4*size-1)*cache / ( 4*size*math.sqrt(size) )
    #----------------trials
    theory = cache*( (1/math.sqrt(size) ) + (1/size) )
    #theory = ( (2*math.sqrt(size) - 1) * cache ) / size
    #theory = (2-2/math.sqrt(size) + 1/size ) * cache / math.sqrt(size) 
    #theory = (2*(1-1/(2*math.sqrt(size) ) )- (math.sqrt(size) - 1)/(math.sqrt(size) + 1) ) * cache / math.sqrt(size) 
    result.append([time,size,cost,cache,theory,u_bound])
  return result 

#infilename = sys.argv[1]
#ifile = open(infilename, 'r')

result = []
lst = [i for i in arange(0,36000000, 36000) ]
win = [ [] for k in arange(len(lst) ) ]

#for line in ifile:
for line in sys.stdin:
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
t = []
c = []
s = []
u = []
for out in output:
  t.append(out[0])
  c.append(out[2])
  s.append(out[4])
  u.append(out[5])
"""
print 'time    size    cost    cache'
for out in output:
  print out[0], out[1], out[2], out[3]
"""
#print output
figure()
plot(t,c,'b', t,s, 'r-.')
#plot(t,c,'b', t,s, 'r--',t,u,'g-.')
#legend(('simulation','expectation','union bound'),'upper left',shadow=True)
legend(('simulation','expectation'),'upper left',shadow=True)
xlabel('time')
ylabel('stabilization cost')
title(r'stabilization cost, $\alpha$ = 1')
show()

