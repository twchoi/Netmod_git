#!/usr/bin/env python
import os, os.path, re, sys
import array, math
from pylab import *
import numpy as np

def autolabel(rects):
  # attach some text labels
  for rect in rects:
    height = rect.get_height()
    text(rect.get_x()+rect.get_width()/2., 1.05*height, '%d'%int(height), ha='center', va='bottom')

f_name = "statistic.txt"
f = open(f_name, 'r')
v = [line.split() for line in f if line[0] != "#"]
print v
s4_5e = v[3][2]

N = 4
#size
sizeMeans5 = [float(v[0][1]), float(v[1][1]), float(v[2][1]), float(v[3][1]) ]
sizeStd5 =  [float(v[0][2]), float(v[1][2]), float(v[2][2]), float(v[3][2]) ]

sizeMeans5e = [float(v[16][1]), float(v[17][1]), float(v[18][1]), float(v[19][1]) ]
sizeStd5e =  [float(v[16][2]), float(v[17][2]), float(v[18][2]), float(v[19][2]) ]

sizeMeans500 = [float(v[8][1]), float(v[9][1]), float(v[10][1]), float(v[11][1]) ]
sizeStd500 =  [float(v[8][2]), float(v[9][2]), float(v[10][2]), float(v[11][2]) ]

sizeMeans500e = [float(v[24][1]), float(v[25][1]), float(v[26][1]), float(v[27][1]) ]
sizeStd500e =  [float(v[24][2]), float(v[25][2]), float(v[26][2]), float(v[27][2]) ]

#cost
costMeans5 = [float(v[4][1]), float(v[5][1]), float(v[6][1]), float(v[7][1]) ]
costStd5 =  [float(v[4][2]), float(v[5][2]), float(v[6][2]), float(v[7][2]) ]

costMeans5e = [float(v[20][1]), float(v[21][1]), float(v[22][1]), float(v[23][1]) ]
costStd5e =  [float(v[20][2]), float(v[21][2]), float(v[22][2]), float(v[23][2]) ]

costMeans500 = [float(v[12][1]), float(v[13][1]), float(v[14][1]), float(v[15][1]) ]
costStd500 =  [float(v[12][2]), float(v[13][2]), float(v[14][2]), float(v[15][2]) ]

costMeans500e = [float(v[28][1]), float(v[29][1]), float(v[30][1]), float(v[31][1]) ]
costStd500e =  [float(v[28][2]), float(v[29][2]), float(v[30][2]), float(v[31][2]) ]


ind = np.arange(N)  #the x locations for the groups
width = 0.45  # the width of the bars
#subplot(111)
figure(1)
rects1 = bar(ind, sizeMeans5, width, color='r', yerr=sizeStd5)
rects2 = bar(ind+width, sizeMeans5e, width, color='y', yerr=sizeStd5e)
ylabel('Estimated Network Size')
title('Estimated Network Size (5,000 nodes)')
xticks(ind+width, ('M1', 'M2', 'M3', 'M4'))
legend( (rects1[0], rects2[0]), ('random','more uniform'), loc=2)
autolabel(rects1)
autolabel(rects2)
savefig('size5k.eps')

figure(2)
rects3 = bar(ind, sizeMeans500, width, color='r', yerr=sizeStd500)
rects4 = bar(ind+width, sizeMeans500e, width, color='y', yerr=sizeStd500e)
ylabel('Estimated Network Size')
title('Estimated Network Size (500,000 nodes)')
xticks(ind+width, ('M1', 'M2', 'M3', 'M4'))
legend( (rects3[0], rects4[0]), ('random','more uniform'), loc=2)
autolabel(rects3)
autolabel(rects4)
savefig('size500k.eps')

figure(3)
rects5 = bar(ind, costMeans5, width, color='r', yerr=costStd5)
rects6 = bar(ind+width, costMeans5e, width, color='y', yerr=costStd5e)
ylabel('Number of Messages')
title('Communication Cost (5,000 nodes)')
xticks(ind+width, ('M1', 'M2', 'M3', 'M4'))
legend( (rects5[0], rects6[0]), ('random','more uniform'), loc=2)
autolabel(rects5)
autolabel(rects6)
savefig('cost5k.eps')

figure(4)
rects7 = bar(ind, costMeans500, width, color='r', yerr=costStd500)
rects8 = bar(ind+width, costMeans500e, width, color='y', yerr=costStd500e)
ylabel('Number of Messages')
title('Communication Cost (500,000 nodes)')
xticks(ind+width, ('M1', 'M2', 'M3', 'M4'))
legend( (rects7[0], rects8[0]), ('random','more uniform'), loc=2)
autolabel(rects7)
autolabel(rects8)
savefig('cost500k.eps')
show()
"""
ylabel('Number of Messages')
title('Communication Cost by Each Method (500,000 nodes)')
xticks(ind+width/2., ('M1', 'M2', 'M3', 'M4'))
autolabel(rects2)

#rects1 = bar(ind, sizeMeans, width, color='r', yerr=sizeStd)

"""
