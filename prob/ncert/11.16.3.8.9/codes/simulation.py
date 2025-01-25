import numpy as np
import matplotlib.pyplot as plt
import ctypes
import math

# dll linking
dll = ctypes.CDLL('./simulation.so')

# describing the argument and return types of the function 'simPlot' and 'freeMultiMem' in the dll
dll.simPlot.argtypes = [ctypes.c_int]*2 + [ctypes.c_float]
dll.simPlot.restype = ctypes.POINTER(ctypes.POINTER(ctypes.c_float))

dll.freeMultiMem.argtypes = [ctypes.POINTER(ctypes.POINTER(ctypes.c_float)), ctypes.c_int]
dll.freeMultiMem.restype = None

n = 10000 # no of points to plot for given simulation plot
m = 3 # number of tosses
p = 0.5 # probability of getting heads

# getting an array of all the points in the plot
pts = dll.simPlot(n, m, p)

# plotting the differential equation using plt.scatter
coords = []
for pt in pts[:n]:
    print(pt[0], pt[1])
    coords.append(np.array([[pt[0], pt[1]]]).reshape(-1, 1))

coords_plot = np.block(coords)
plt.scatter(coords_plot[0,:], coords_plot[1,:], marker=".", label = "Sim", color="royalblue")

# plotting the actual probability line
plt.plot(np.linspace(0, n, 1000), [0.875]*1000, linestyle='dashed', color="red")

# freeing the memory of the array 'pts'
dll.freeMultiMem(pts, n)

# saving the plot
plt.savefig('../figs/sim.png')
plt.show()
