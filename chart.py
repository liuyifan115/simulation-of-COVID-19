import numpy
import matplotlib.pyplot
import csv

file = []
for line in csv.reader(open("./output/I_want_this.csv")):
    file.append(line)
day = [int(i) for i in file[0]]
infected = [int(i) for i in file[1]]
shelter = [int(i) for i in file[2]]
cost = [int(i) for i in file[3]]
print("真不会有人闲的没事看控制台输出吧")

chart = matplotlib.pyplot.figure(1)
infect = matplotlib.pyplot.subplot(3, 1, 1)
matplotlib.pyplot.plot(day, infected, color='r')
matplotlib.pyplot.xlabel("Days")
matplotlib.pyplot.ylabel("Infections")

shelters = matplotlib.pyplot.subplot(3, 1, 2)
matplotlib.pyplot.plot(day, shelter, color='g')
matplotlib.pyplot.xlabel("Days")
matplotlib.pyplot.ylabel("Shelter")

costs = matplotlib.pyplot.subplot(3, 1, 3)
matplotlib.pyplot.plot(day, shelter, color='b')
matplotlib.pyplot.xlabel("Days")
matplotlib.pyplot.ylabel("Cost")


matplotlib.pyplot.show()
