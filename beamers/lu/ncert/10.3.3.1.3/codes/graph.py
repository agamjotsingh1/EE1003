import numpy as np
import matplotlib.pyplot as plt
import ctypes
import math

# dll linking
dll = ctypes.CDLL('./points.so')

# describing the argument and return types of the function 'lineGet' and 'freeMultiMem' in the dll
dll.lineGet.argtypes = [ctypes.c_int] + [ctypes.c_float]*5
dll.lineGet.restype =  ctypes.POINTER(ctypes.POINTER(ctypes.c_float))

dll.freeMultiMem.argtypes = [ctypes.POINTER(ctypes.POINTER(ctypes.c_float)), ctypes.c_int]
dll.freeMultiMem.restype = None

n = 1000 # no of points to plot for each line
x1 = -3
x2 = 3 # range of points x1 to x2 

'''Plotting Line 1'''
a = 3
b = -1 
c = -3
# getting an array of all the points in the plot
pts = dll.lineGet(n, x1, x2, a, b, c) 

# plotting the line
coords = []
for pt in pts[:n]:
    coords.append(np.array([[pt[0], pt[1]]]).reshape(-1, 1))

coords_plot = np.block(coords)
plt.plot(coords_plot[0,:], coords_plot[1,:], label = "3x - y = 3", color="orange", marker=".")

'''Plotting Line 2'''
a = 9
b = -3 
c = -9

# getting an array of all the points in the plot
pts2 = dll.lineGet(n, x1, x2, a, b, c) 

# plotting the line
coords = []
for pt in pts2[:n]:
    coords.append(np.array([[pt[0], pt[1]]]).reshape(-1, 1))

coords_plot = np.block(coords)
plt.plot(coords_plot[0,:], coords_plot[1,:], label = "9x - 3y = 9", color="blue")

# freeing the memory of the array 'pts' and 'pts2'
dll.freeMultiMem(pts, n)
dll.freeMultiMem(pts2, n)

# including legend, grid, axis in the plot
plt.legend(loc='best')
plt.grid()
plt.axis('equal')
#plt.xlim(-25, 25)
#plt.ylim(-100, 20)

# including x, y labels in the plot
plt.xlabel('x')
plt.ylabel('y')

# saving the plot
plt.savefig('../figs/graph.png')
plt.show()
