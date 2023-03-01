from celluloid import Camera
from matplotlib import pyplot as plt
import sys

if len(sys.argv) < 2:
    print("Usage: python vis.py <filename>")
    exit(-1)

f = open(sys.argv[1], 'r')

fig = plt.figure()
camera = Camera(fig)

for line in f.readlines()[1:]:
    l = line.split(',')[1:-1]
    xs = list(map(float, l[::2]))
    ys = list(map(float, l[1::2]))
    plt.scatter(xs, ys, color='red', s=3)
    camera.snap()

camera.animate().save('vis.gif')
