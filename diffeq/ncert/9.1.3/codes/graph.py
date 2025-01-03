import numpy as np
import matplotlib.pyplot as plt
import ctypes
import math

#dll linking
dll = ctypes.CDLL('./points.so')

dll.diffEqPoints.argtypes = [ctypes.c_int] + [ctypes.c_float]*4
dll.diffEqPoints.restype = ctypes.POINTER(ctypes.POINTER(ctypes.c_float))

n = 1000 #no of points to plot for given differential equation plot

x = 0
y1 = 1
y2 = 2
h = 0.01

#Labeling the coordinates
tri_coords = np.block([np.array([[x, y1]]).reshape(-1, 1)])
plt.scatter(tri_coords[0,:], tri_coords[1,:])
vert_labels = ['A']
for i, txt in enumerate(vert_labels):
    plt.annotate(f'{txt}\n({tri_coords[0,i]:.0f}, {tri_coords[1,i]:.0f})',
                 (tri_coords[0,i], tri_coords[1,i]), # this is the point to label
                 textcoords="offset points", # how to position the text
                 xytext=(10,-20), # distance from text to points (x,y)
                 ha='center') # horizontal alignment can be left, right or center

pts = dll.diffEqPoints(n, h, x, y1, y2) 

#Plotting the circle
coords = []
for pt in pts[:n]:
    print(pt[0], pt[1])
    coords.append(np.array([[pt[0], pt[1]]]).reshape(-1, 1))

coords_plot = np.block(coords)
plt.scatter(coords_plot[0,:], coords_plot[1,:], marker=".", label = "$({y^{\prime}})^4 + 3yy^{\prime\prime} = 0$", color="royalblue")

dll.freeMultiMem(pts, n)

# use set_position
ax = plt.gca()
ax.spines['top'].set_color('none')
ax.spines['left'].set_position('zero')
ax.spines['right'].set_color('none')
ax.spines['bottom'].set_position('zero')

plt.legend(loc='best')
plt.grid()
plt.axis('equal')
plt.savefig('../figs/graph.png')
plt.show()
