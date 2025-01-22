import numpy as np
import matplotlib.pyplot as plt
import ctypes
import math

# dll linking
dll = ctypes.CDLL('./points.so')

# describing the argument and return types of the function 'quadPlot', 'quadRoot' and 'freeMultiMem' in the dll
dll.quadRoot.argtypes = [ctypes.c_int] + [ctypes.c_float]*2
dll.quadRoot.restype = (ctypes.c_float)

dll.quadPlot.argtypes = [ctypes.c_int] + [ctypes.c_float]*2
dll.quadPlot.restype =  ctypes.POINTER(ctypes.POINTER(ctypes.c_float))

dll.freeMultiMem.argtypes = [ctypes.POINTER(ctypes.POINTER(ctypes.c_float)), ctypes.c_int]
dll.freeMultiMem.restype = None

n = 10000 # no of points to plot for quadratic equation
h = 0.01 # step size
x0 = 8 # intial guess
x = -10 # starting point for the graph
tol = 1e-6

# getting an array of all the points in the plot
pts = dll.quadPlot(n, h, x) 

# plotting the objective function using plt.scatter
coords = []
for pt in pts[:n]:
    print(pt[0], pt[1])
    coords.append(np.array([[pt[0], pt[1]]]).reshape(-1, 1))

coords_plot = np.block(coords)
plt.plot(coords_plot[0,:], coords_plot[1,:], label = "Quadratic Equation", color="orange")

# Getting the root of the quadratic equation
maxIters = 1000
root = dll.quadRoot(maxIters, tol, x0)
print("x_1 = ", root)
plt.scatter([root], [2*root*root + 3*root - 90], marker = "o", color = "black", label = "Root 1")

# Getting the other root of the quadratic equation
x0 = -8
root2 = dll.quadRoot(maxIters, tol, x0)
print("x_2 = ", root2)
plt.scatter([root2], [2*root2*root2 + 3*root2 - 90], marker = "o", color = "royalblue", label = "Root 2")

# freeing the memory of the array 'pts'
dll.freeMultiMem(pts, n)

# including legend, grid, axis in the plot
plt.legend(loc='best')
plt.grid()
plt.axis('equal')
plt.xlim(-25, 25)
plt.ylim(-100, 20)

# including x, y labels in the plot
plt.xlabel('x')
plt.ylabel('y')

# saving the plot
plt.savefig('../figs/graph.png')
plt.show()
