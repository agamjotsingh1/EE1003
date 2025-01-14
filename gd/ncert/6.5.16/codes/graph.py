import numpy as np
import matplotlib.pyplot as plt
import ctypes
import math

# dll linking
dll = ctypes.CDLL('./points.so')

# describing the argument and return types of the function 'objectiveFunctionPoints', 'gradientDescentPoints' and 'freeMultiMem' in the dll
dll.gradientDescentPoint.argtypes = [ctypes.c_int] + [ctypes.c_float]*3
dll.gradientDescentPoint.restype = (ctypes.c_float)

dll.objectiveFunctionPoints.argtypes = [ctypes.c_int] + [ctypes.c_float]*2
dll.objectiveFunctionPoints.restype =  ctypes.POINTER(ctypes.POINTER(ctypes.c_float))

dll.freeMultiMem.argtypes = [ctypes.POINTER(ctypes.POINTER(ctypes.c_float)), ctypes.c_int]
dll.freeMultiMem.restype = None

n = 100 # no of points to plot for objective function
h = 0.01 # step size
x = 7 # intial guess
tol = 1e-3

'''
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
'''

x0 = 7.5

# getting an array of all the points in the plot
pts = dll.objectiveFunctionPoints(n, h, x0) 

# plotting the objective function using plt.scatter
coords = []
for pt in pts[:n]:
    coords.append(np.array([[pt[0], pt[1]]]).reshape(-1, 1))

coords_plot = np.block(coords)
plt.plot(coords_plot[0,:], coords_plot[1,:], label = "Objective Function", color="orange")

# getting minimum x using gradient descent and plotting it
grad_n = 1000
x_min = dll.gradientDescentPoint(grad_n, h, x, tol)
print("x_min = ", x_min)
print("f_min = ", x_min**3 + (16 - x_min)**3)
plt.scatter(x_min, x_min**3 + (16 - x_min)**3, label= "Minimum Point", color="black", marker="o")

# freeing the memory of the array 'pts'
dll.freeMultiMem(pts, n)

# including legend, grid, axis in the plot
plt.legend(loc='best')
plt.grid()
plt.axis('equal')
plt.xlim(7.5, 8.5)
plt.ylim(1023.9, 1025)

# including x, y labels in the plot
plt.xlabel('x')
plt.ylabel('y')

# saving the plot
plt.savefig('../figs/graph.png')
plt.show()
