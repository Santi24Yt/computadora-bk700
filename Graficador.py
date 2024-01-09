import pandas as pd
import matplotlib.pyplot as plt
data= pd.read_csv("VUELO30.TXT",header=0,delimiter=",")
time=data.iloc[:,0]
press=data.iloc[:,1]
alt=data.iloc[:,2]
acex=data.iloc[:,3]
acey=data.iloc[:,4]
acez=data.iloc[:,5]
temp=data.iloc[:,9]

# Graficando datos
plt.figure(figsize=(10, 8))
#print (data)

plt.subplot(221)
plt.plot(time, press)
plt.title('Presion rescpecto Tiempo')
plt.xlabel('Tiempo (sec)')
plt.ylabel('Presion ')
#plt.show()

#Segunda grafica
plt.subplot(222)
plt.plot(time, alt)
plt.title('Altura respecto Tiempo ')
plt.xlabel('Timpo (sec)')
plt.ylabel('Altura')
#plt.show()

# Tercera Grafica
# Cuarta Grafica
plt.subplot(223)
plt.plot(time, temp)
plt.title('Temperatura rescpecto Tiempo')
plt.xlabel('Tiempo (sec)')
plt.ylabel('Temperatura ')
#plt.show()

# Ajustamos el layout para que no se sobreempalme
plt.tight_layout()
plt.show()
