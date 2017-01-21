import matplotlib.pyplot as plt

lines = open("results.txt").read().splitlines()

xarr = []
yarr = []
for line in lines:
	a,b = line.split(":")
	_ , x = a.split(" ")
	y = float(b)
	xarr.append(x)
	yarr.append(y)

plt.plot(xarr,yarr,label="Results")
plt.xlim(1,int(xarr[-1]))
plt.ylim(yarr[0],1)
plt.show()
