#!/usr/bin/env python
import sys, math
def mean(b):
  return float(sum(b)) / float(len(b))

infile = sys.argv[1]
ifile = open(infile,'r')
c_time = []
q_time = []
q_size = []
c_size = []
for line in ifile:
  data = line.split()
  action = data[1]
  if action == 'caching':
    c_time.append(float(data[0]) )
    c_size.append(int(data[4]) )
  elif action == 'querying':
    q_time.append(float(data[0]) )
    q_size.append(int(data[5]) )
#print 'caching size'
#for i in xrange(len(c_time)):
#  print c_size[i]

print 'querying size'
for i in xrange(len(q_time)):
  print q_size[i]
print mean(c_size)
print mean(q_size)
