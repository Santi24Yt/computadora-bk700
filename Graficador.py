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
       
datos = dict()

for a in data:
    datos[a[0]] = np.array(a[1:], dtype=float)

print(datos)


for k in datos.keys():
    p = plt.figure()
    plt.plot(datos["tiempo"], datos[k])
    plt.title(k)
    plt.xlabel("Tiempo")
    plt.ylabel(k)

    for marcador in marcadores:
        print(marcador)
        print(datos[k][marcador[1]])
        plt.plot(marcador[0], datos[k][marcador[1]], "ro")

    p.show()
    

acex = datos["aceleracion x"]
acey = datos["aceleracion y"]
acez = datos["aceleracion z"]
time = datos["tiempo"]
total_acceleration = (acex**2 + acey**2 + acez**2)**0.5

# Cuarta gráfica - Aceleración total respecto al tiempo
plt.plot(time, total_acceleration, color='purple')
plt.title('Aceleración Total respecto al Tiempo')
plt.xlabel('Tiempo (sec)')
plt.ylabel('Aceleración Total')

# Ajustar el diseño para evitar superposiciones
plt.show()
          
