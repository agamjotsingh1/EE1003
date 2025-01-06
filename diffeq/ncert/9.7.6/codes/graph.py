import numpy as np
import matplotlib.pyplot as plt
import ctypes
import math

# dll linking
dll = ctypes.CDLL('./points.so')

# describing the argument and return types of the function 'diffEqPoints' and 'freeMultiMem' in the dll
dll.diffEqPoints.argtypes = [ctypes.c_int] + [ctypes.c_float]*3
dll.diffEqPoints.restype = ctypes.POINTER(ctypes.POINTER(ctypes.c_float))

dll.freeMultiMem.argtypes = [ctypes.POINTER(ctypes.POINTER(ctypes.c_float)), ctypes.c_int]
dll.freeMultiMem.restype = None

h = 0.0001 # step size
n = 15000 # no of points to plot

# initial conditions, y1 = y
x = -0.5
y1 = 0.9988864023252176 # math.sin(1 + math.asin(0.5))

# getting an array of all the points of the sim plot
pts = dll.diffEqPoints(n, h, x, y1) 

# plotting the differential equation using plt.scatter
coords = []
for pt in pts[:n]:
    coords.append(np.array([[pt[0], pt[1]]]).reshape(-1, 1))

coords_plot = np.block(coords)
plt.scatter(coords_plot[0,:], coords_plot[1,:], marker=".", label = "Sim", color="royalblue")

# theoretical plot
x_linspace = np.linspace(-0.5, 0.99, 1000)
y_linspace = np.sin(-np.arcsin(x_linspace) + 1)
plt.plot(x_linspace, y_linspace, c = 'g', label = "Theory")

# freeing the memory of the points of all three intervals
dll.freeMultiMem(pts, n)

# including legend, grid, axis in the plot
plt.legend(loc='best')
plt.grid()
plt.axis('equal')

# including x, y labels and x limit in the plot
plt.xlim(-2, 2)
plt.ylim(-2, 2)
plt.xlabel('x')
plt.ylabel('y')

# saving the plot
plt.savefig('../figs/graph.png')
plt.show()
