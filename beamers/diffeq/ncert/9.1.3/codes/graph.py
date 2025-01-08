import numpy as np
import matplotlib.pyplot as plt
import ctypes
import math

# dll linking
dll = ctypes.CDLL('./points.so')

# describing the argument and return types of the function 'diffEqPoints' and 'freeMultiMem' in the dll
dll.diffEqPoints.argtypes = [ctypes.c_int] + [ctypes.c_float]*4
dll.diffEqPoints.restype = ctypes.POINTER(ctypes.POINTER(ctypes.c_float))

dll.freeMultiMem.argtypes = [ctypes.POINTER(ctypes.POINTER(ctypes.c_float)), ctypes.c_int]
dll.freeMultiMem.restype = None

n = 200 # no of points to plot for given differential equation plot
h = 0.01 # step size

# initial conditions, y1 = y, y2 = y'
x = 0
y1 = 0.01
y2 = 1

# labeling the coordinates
tri_coords = np.block([np.array([[x, y1]]).reshape(-1, 1)])
plt.scatter(tri_coords[0,:], tri_coords[1,:])
vert_labels = ['A']
for i, txt in enumerate(vert_labels):
    plt.annotate(f'{txt}\n({tri_coords[0,i]:.0f}, {tri_coords[1,i]:.0f})',
                 (tri_coords[0,i], tri_coords[1,i]), # this is the point to label
                 textcoords="offset points", # how to position the text
                 xytext=(10,-20), # distance from text to points (x,y)
                 ha='center') # horizontal alignment can be left, right or center

# getting an array of all the points in the plot
pts = dll.diffEqPoints(n, h, x, y1, y2) 

# plotting the differential equation using plt.scatter
coords = []
for pt in pts[:n]:
    print(pt[0], pt[1])
    coords.append(np.array([[pt[0], pt[1]]]).reshape(-1, 1))

coords_plot = np.block(coords)
plt.scatter(coords_plot[0,:], coords_plot[1,:], marker=".", label = "Sim", color="royalblue")

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
