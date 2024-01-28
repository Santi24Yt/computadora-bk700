from sys import argv, exit
from os import path

from csv import reader as read

import numpy as np
import matplotlib.pyplot as plt

if len(argv) < 2 or not path.exists(argv[1]):
    exit("Archivo inválido")

file = open(argv[1])
raw = read(file)

data = list()
marcadores = list()

ln = 0
for row in raw:
    if ln == 0:
        ln = 1
        for column in row:
            data.append([column])

        continue

    for i in range(len(row)):
        column = row[i]
        if i == 0 and column[0] == '-':
            column = column[1:]
            marcadores.append((float(column), ln))

        data[i].append(column)
       
    ln += 1
       
del raw
file.close()
del file

datos = dict()

for a in data:
    datos[a[0]] = np.array(a[1:], dtype=float)

del data


def movingaverage(a, w):
    return np.convolve(a, np.ones(w), "valid") / w

w = 10
for k in datos.keys():
    if k == "tiempo":
        continue

    datos[k] = movingaverage(datos[k], w)

datos["tiempo"] = datos["tiempo"][:-w+1]

c = 0
for k in datos.keys():
    p = plt.figure()
    plt.plot(datos["tiempo"], datos[k], color="C"+str(c))
    plt.xlabel("tiempo")
    plt.ylabel(k)
    plt.grid()


    for marcador in marcadores:
        plt.plot(marcador[0], datos[k][marcador[1]], "k^", fillstyle='none')

    c += 1
    p.show()
    

time = datos["tiempo"]

datos["var. tiempo"] = np.append(np.diff(datos["tiempo"]), 0)

plt.figure()
plt.plot(time, datos["var. tiempo"])
plt.xlabel("tiempo")
plt.ylabel("var. tiempo")
plt.title("Promedio: "+str(np.average(datos["var. tiempo"]))+"ms")
plt.grid()
for marcador in marcadores:
        plt.plot(marcador[0], datos["var. tiempo"][marcador[1]], "k^", fillstyle='none')

acex = datos["aceleracion x"]
acey = datos["aceleracion y"]
acez = datos["aceleracion z"]
total_acceleration = (acex**2 + acey**2 + acez**2)**0.5

# Cuarta gráfica - Aceleración total respecto al tiempo
plt.figure()
plt.plot(time, total_acceleration, color='purple')
plt.title('Aceleración Total respecto al Tiempo')
plt.xlabel('Tiempo (sec)')
plt.ylabel('Aceleración Total')
plt.grid()

plt.show()
          
