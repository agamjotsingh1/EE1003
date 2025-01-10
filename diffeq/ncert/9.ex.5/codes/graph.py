import numpy as np
import matplotlib.pyplot as plt
import ctypes
import math

# dll linking
dll = ctypes.CDLL('./points.so')

# describing the argument and return types of the function 'diffEqPoints', 'diffEqPointsBilinear' and 'freeMultiMem' in the dll
dll.diffEqPoints.argtypes = [ctypes.c_int] + [ctypes.c_float]*4
dll.diffEqPoints.restype = ctypes.POINTER(ctypes.POINTER(ctypes.c_float))

dll.diffEqPointsBilinear.argtypes = [ctypes.c_int] + [ctypes.c_float]*4
dll.diffEqPointsBilinear.restype = ctypes.POINTER(ctypes.POINTER(ctypes.c_float))

dll.freeMultiMem.argtypes = [ctypes.POINTER(ctypes.POINTER(ctypes.c_float)), ctypes.c_int]
dll.freeMultiMem.restype = None

n = 500 # no of points to plot for given differential equation plot
h = 0.05 # step size

# initial conditions, y1 = y, y2 = y'
x = 0
y1 = 0
y2 = 1

''' Plotting Sim-1 Graph '''
# getting an array of all the points in the plot
pts = dll.diffEqPoints(n, h, x, y1, y2) 

# plotting the differential equation using plt.scatter
coords = []
for pt in pts[:n]:
    coords.append(np.array([[pt[0], pt[1]]]).reshape(-1, 1))

coords_plot = np.block(coords)
plt.plot(coords_plot[0,:], coords_plot[1,:], label = "Sim-1", color="royalblue")

''' Plotting Sim-2 Graph '''
# getting an array of all the points in the plot
pts = dll.diffEqPointsBilinear(n, h, x, y1, y2) 

# plotting the differential equation using plt.scatter
coords = []
for pt in pts[:n]:
    coords.append(np.array([[pt[0], pt[1]]]).reshape(-1, 1))

coords_plot = np.block(coords)
plt.scatter(coords_plot[0,:], coords_plot[1,:], marker=".", label = "Sim-2", color="lightgreen")

# theoretical plot
x_linspace = np.linspace(0, int(n*h), n)
y_linspace = np.sin(x_linspace)
plt.plot(x_linspace, y_linspace, c = 'r', label = "Theory")

# freeing the memory of the array 'pts'
dll.freeMultiMem(pts, n)

# use set_position
ax = plt.gca()
ax.spines['top'].set_color('none')
ax.spines['left'].set_position('zero')
ax.spines['right'].set_color('none')
ax.spines['bottom'].set_position('zero')

# including legend, grid, axis in the plot
plt.legend(loc='best')
plt.grid()
plt.axis('equal')

# including x, y labels in the plot
plt.xlabel('x')
plt.ylabel('y')

# saving the plot
plt.savefig('../figs/graph.png')
plt.show()
