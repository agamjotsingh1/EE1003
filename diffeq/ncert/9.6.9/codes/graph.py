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

h = 0.01 # step size

# three sets of intervals are to be plotted

n1 = 290 # no of points to plot for given interval

# initial conditions for first interval, y1 = y
x = 0.21
y1 = 22

# getting an array of all the points in the first interval 
pts1 = dll.diffEqPoints(n1, h, x, y1) 

n2 = 305 # no of points to plot for first interval

# initial conditions for second interval, y1 = y
x = 3.2
y1 = -20

# getting an array of all the points in the second interval 
pts2 = dll.diffEqPoints(n2, h, x, y1) 

n3 = 280 # no of points to plot for third interval

# initial conditions for third interval, y1 = y
x = -3.1
y1 = -20

# getting an array of all the points in the first interval 
pts3 = dll.diffEqPoints(n3, h, x, y1) 

# plotting the differential equation using plt.scatter
coords = []
for pt in pts1[:n1] + pts2[:n2] + pts3[:n3]:
    coords.append(np.array([[pt[0], pt[1]]]).reshape(-1, 1))

coords_plot = np.block(coords)
plt.scatter(coords_plot[0,:], coords_plot[1,:], marker=".", label = "Sim", color="royalblue")

# theoretical plot
x_linspace = np.linspace(-3.1, 6.3, 1000)
y_linspace = -(1/np.tan(x_linspace)) + 1/x_linspace + 1/(x_linspace * np.sin(x_linspace))
plt.plot(x_linspace, y_linspace, c = 'g', label = "Theory")

# freeing the memory of the points of all three intervals
dll.freeMultiMem(pts1, n1)
dll.freeMultiMem(pts2, n2)
dll.freeMultiMem(pts3, n3)

# including legend, grid, axis in the plot
plt.legend(loc='best')
plt.grid()
plt.axis('equal')

# including x, y labels and x limit in the plot
plt.xlim(-10, 10)
plt.ylim(-10, 10)
plt.xlabel('x')
plt.ylabel('y')

# saving the plot
plt.savefig('../figs/graph.png')
plt.show()
