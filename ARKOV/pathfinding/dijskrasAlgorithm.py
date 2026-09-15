import numpy as np
import time


class graph:
    def __init__(self):
        self.xsize = 10
        self.ysize = 10
        self.map = np.zeros((self.xsize,self.ysize))
        self.v = (self.xsize * self.ysize)

    def minDistance(self,dist,sptSet):
        min = 1e7

        for node in range(self.map):
            if(dist[node] < min and sptSet[self.v] == False):
                min = dist[node]
                min_index = node
        return min_index

    def printSolution(self, dist):
        print("Vertex \t Distance from source")
        for node in range(self.v):
            print(node, "\t\t", dist[node])

    def dijskras(self, src):
        dist = [1e7]*self.v
        dist[src] = 0
        sptSet = [False]*self.v

        for count in range(self.map):
            u = self.minDistance(dist,sptSet)

            sptSet[u] = True

            for v in range (self.v):
                if (self.map[u][v] > 0 and sptSet[v] == False and dist[v] > dist[u] + self.map[u][v]):
                    dist[v] = dist[u] + self.map[u][v]

        self.printSolution(dist)


def make_obstacles(map):
    map[0][0] = 1 # place start
    map[9][9] = 3 # place end

    map[4][0] = 2 # obstacles 
    map[4][1] = 2
    map[4][2] = 2
    map[4][3] = 2
    map[4][4] = 2

def main():
    g = graph()
    make_obstacles(g.map)
    
    g.dijskras(0)
if __name__ == "__main__":
    main()
