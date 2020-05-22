import matplotlib.pyplot as plt
import sys


plt.figure(figsize=(16, 9))
n = int(input())
for i in range(n):
    coords = list(map(float, input().split()))
    plt.plot([coords[0], coords[2]], [coords[1], coords[3]], color='black')
m = int(input())
for i in range(m):
    coords = list(map(float, input().split()))
    plt.plot([coords[0], coords[2]], [coords[1], coords[3]], color='red')
plt.savefig(sys.argv[1])
