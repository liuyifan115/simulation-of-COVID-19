import numpy
import matplotlib.pyplot
import matplotlib.backends.backend_pdf
import csv

file = []
for line in csv.reader(open("./output/I_want_this.csv")):
    file.append(line)
day = [int(i) for i in file[0]]
infected = [int(i) for i in file[1]]
shelter = [int(i) for i in file[2]]
cost = [int(i) for i in file[3]]
print(infected)
print(shelter)
print(cost)
print("真不会有人闲的没事看控制台输出吧")

matplotlib.pyplot.rcParams["font.sans-serif"] = ["FangSong"]
matplotlib.pyplot.rcParams["axes.unicode_minus"] = False

chart = matplotlib.pyplot.figure(1)
infect = matplotlib.pyplot.subplot(3, 1, 1)
matplotlib.pyplot.title("COVID-19的模拟\n感染率=2.1 封城需要的感染人数=100,000 单位：million")
matplotlib.pyplot.yticks([0, 1000000, 2000000, 3000000], [0, 1, 2, 3])
matplotlib.pyplot.plot(day, infected, color='r')
matplotlib.pyplot.xlabel("模拟天数")
matplotlib.pyplot.ylabel("感染人数")

shelters = matplotlib.pyplot.subplot(3, 1, 2)
matplotlib.pyplot.yticks([0, 1000000, 2000000, 3000000], [0, 1, 2, 3])
matplotlib.pyplot.plot(day, shelter, color='g')
matplotlib.pyplot.xlabel("模拟天数")
matplotlib.pyplot.ylabel("在方舱人数")

costs = matplotlib.pyplot.subplot(3, 1, 3)
ax = matplotlib.pyplot.gca()
ax.ticklabel_format(style='sci', scilimits=(-1, 2), axis='y')
matplotlib.pyplot.plot(day, cost, color='b')
matplotlib.pyplot.xlabel("模拟天数")
matplotlib.pyplot.ylabel("总花费")

chart.savefig("chart.pdf")

