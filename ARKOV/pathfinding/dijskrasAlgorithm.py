import numpy as np
import time

xsize = 10
ysize = 10

map = np.zeros((xsize,ysize))

curx = 0
cury = 0

que = set([(curx,cury)])

def make_obstacles():
    map[0][0] = 1 # place start
    map[9][9] = 3 # place end

    map[4][0] = 2 # obstacles 
    map[4][1] = 2
    map[4][2] = 2
    map[4][3] = 2
    map[4][4] = 2

def initalize():
    pass

def Dijkstras():
    for row in map:
        for vertex in row:
            que.add(vertex)