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

total_acceleration = (acex**2 + acey**2 + acez**2)**0.5

# Configuración de subgráficas
plt.figure(figsize=(12, 10))

# Primera gráfica - Presión respecto al tiempo
plt.subplot(221)
plt.plot(time, press, color='r')
plt.title('Presión respecto al Tiempo')
plt.xlabel('Tiempo (sec)')
plt.ylabel('Presión')

# Segunda gráfica - Altura respecto al tiempo
plt.subplot(222)
plt.plot(time, alt, color='g')
plt.title('Altura respecto al Tiempo')
plt.xlabel('Tiempo (sec)')
plt.ylabel('Altura')

# Tercera gráfica - Temperatura respecto al tiempo
plt.subplot(223)
plt.plot(time, temp, color='b')
plt.title('Temperatura respecto al Tiempo')
plt.xlabel('Tiempo (sec)')
plt.ylabel('Temperatura')

# Cuarta gráfica - Aceleración total respecto al tiempo
plt.subplot(224)
plt.plot(time, total_acceleration, color='purple')
plt.title('Aceleración Total respecto al Tiempo')
plt.xlabel('Tiempo (sec)')
plt.ylabel('Aceleración Total')

# Ajustar el diseño para evitar superposiciones
plt.tight_layout()
plt.show()
          
