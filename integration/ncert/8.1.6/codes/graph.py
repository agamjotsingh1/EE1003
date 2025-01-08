import numpy as np
import matplotlib.pyplot as plt
import ctypes
import math

# dll linking
dll = ctypes.CDLL('./points.so')

# describing the argument and return types of the function 'areaTrapezoidLine', 'areaTrapezoidCirc', lineGet', 'fillSector', circleGet' and 'freeMultiMem' in the dll

dll.areaTrapezoidLine.argtypes = [ctypes.c_float]*3
dll.areaTrapezoidLine.restype = ctypes.c_float

dll.areaTrapezoidCirc.argtypes = [ctypes.c_float]*3
dll.areaTrapezoidCirc.restype = ctypes.c_float

dll.lineGet.argtypes = [ctypes.c_int] + [ctypes.c_float]*5
dll.lineGet.restype = ctypes.POINTER(ctypes.POINTER(ctypes.c_float))

dll.fillSector.argtypes = [ctypes.c_int]*2 + [ctypes.c_float]*4
dll.fillSector.restype = ctypes.POINTER(ctypes.POINTER(ctypes.c_float))

dll.circleGet.argtypes = [ctypes.c_int] + [ctypes.c_float]*3
dll.circleGet.restype = ctypes.POINTER(ctypes.POINTER(ctypes.c_float))

dll.freeMultiMem.argtypes = [ctypes.POINTER(ctypes.POINTER(ctypes.c_float)), ctypes.c_int]
dll.freeMultiMem.restype = None


# calculating final area using trapezoidal method
lineArea = dll.areaTrapezoidLine(0, math.sqrt(3), 0.0001)
circArea = dll.areaTrapezoidCirc(math.sqrt(3), 2, 0.0001)
print("Area of the enclosed region is: ", lineArea + circArea)

n = 1000 # number of points to plot for both line and circle

# getting points for the line x - sqrt(3)y = 0
# line is ax + by + c = 0
a = 1.0
b = -math.sqrt(3)
c = 0.0

# getting an array of all the points of the sim plot
ptsLine = dll.lineGet(n, -2, 2, a, b, c) 

# getting points for the circle x^2 + y^2 = 4
O = (0, 0) # center (0, 0)
r = 2 # radius

# getting an array of all the points of the sim plot
ptsCirc = dll.circleGet(n, O[0], O[1], r)

# filling the area enclosed
m = 100
ptsFill = dll.fillSector(m, n, math.pi/6, O[0], O[1], r)
coordsFill = []

for pt in ptsFill[:m*n]:
    coordsFill.append(np.array([[pt[0], pt[1]]]).reshape(-1, 1))

coords_plot_fill = np.block(coordsFill)
plt.scatter(coords_plot_fill[0,:], coords_plot_fill[1,:], marker=".", label = "$A$", color="cyan")

# plotting the line and the circle
coordsLine = []
for pt in ptsLine[:n]:
    coordsLine.append(np.array([[pt[0], pt[1]]]).reshape(-1, 1))

coords_plot_line = np.block(coordsLine)
plt.scatter(coords_plot_line[0,:], coords_plot_line[1,:], marker=".", label = "Line", color="royalblue")

coordsCirc = []
for pt in ptsCirc[:n]:
    coordsCirc.append(np.array([[pt[0], pt[1]]]).reshape(-1, 1))

coords_plot_circ = np.block(coordsCirc)
plt.scatter(coords_plot_circ[0,:], coords_plot_circ[1,:], marker=".", label = "Circle", color="red")

# freeing the memory of the points of all three intervals
dll.freeMultiMem(ptsLine, n)
dll.freeMultiMem(ptsCirc, n)
dll.freeMultiMem(ptsFill, m*n)

# including legend, grid, axis in the plot
plt.legend(loc='best')
plt.grid()
plt.axis('equal')

# including x, y labels and x limit in the plot
plt.xlim(-3, 3)
plt.ylim(-3, 3)
plt.xlabel('x')
plt.ylabel('y')

# saving the plot
plt.savefig('../figs/graph.png')
plt.show()
