#!/usr/bin/env python
import os, os.path, re, sys

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

f_name = sys.argv[1]
f = open(f_name, 'r')
v = [line.split() for line in f if line[0] != "#"]

result = []
for i in v:
  ct2 = int(i[6])
  ct3 = int(i[7])
  ct3_1 = (ct3-ct2)*ct2
  #print ct3_1
  result.append(ct3_1)

mean, std = meanstdv(result)
print mean, std
