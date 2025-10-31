<p align='center'> 
  <img src="https://capsule-render.vercel.app/api?type=waving&height=200&color=80354A&text=Sistema%20de%20vigilancia%20de%20temperatura&fontSize=45&fontAlignY=34&fontColor=ffffff&desc=Con%20sensor%20LM35,%20display%20de%207%20segmentos%20y%20control%20de%20alarma%20LED%20con%20%20arduino&descAlignY=55" /> 
</p>
<p align='center'>
  <a href="https://www.tinkercad.com/things/g2KJFCynQXD-proyecto-7?sharecode=9vcAkaMya27d3jknMqULHOZhC2GdI4eEACdDD21LyC4" target="_blank">
    <img src="https://i.imgur.com/ZmD36kN.png" width="512" height="384" alt="Diseño estructural con Arduino" />
  </a>
</p>
<p align='center'>
    <code>Haz clic en la imagen para ver la simulación interactiva del proyecto en Tinkercad</code>
</p>
<p align='center'>
    <img 
        src="https://capsule-render.vercel.app/api?type=rect&height=5&color=80354A&reversal=false&fontAlignY=40&fontColor=FFFFFF&fontSize=60"
    />
</p>

### Descripción

Sistema basado en Arduino que **monitorea la temperatura ambiente** usando un sensor LM35. Al superar un umbral de seguridad, inicia un **contador regresivo en display de 7 segmentos**; si la temperatura no se normaliza, activa una alarma visual con un LED. La lógica prioriza la seguridad evitando alarmas falsas y permite rearmar el sistema si la situación cambia.

***

### Estructura principal

El proyecto está compuesto por los siguientes elementos y archivos principales:

- `sensor (A0)`: Entrada analógica que lee la temperatura ambiente usando LM35.
- `pins (2-8)`: Salidas digitales que controlan los segmentos numéricos del display para el contador regresivo.
- `led (pin 10)`: Salida digital que se enciende como alerta cuando la temperatura es peligrosa.
- **Variables de estado**: Gestión de temporización, control de inicio de conteo, y lógica de interrupción de alarma.
- **Funciones**:
  - `leerTemperatura`: Convierte el valor analógico del LM35 a grados Celsius.
  - `mostrarNumero` y `apagarDisplay`: Controlan la visualización numérica en el display.
  - `confirmarArranque`: Evita inicios accidentales con doble lectura superior al umbral.
- `setup` y `loop`: Configuran la placa y ejecutan la lógica continua de seguridad y bloqueo de reinicio.

***

### Características principales

- **Monitoreo automático** de la temperatura ambiente.
- **Contador regresivo** en display de 7 segmentos (de 9 a 0) para que el usuario tenga tiempo de reaccionar en caso de emergencia térmica.
- **Interrupción automática** del conteo si la temperatura baja o el sensor detecta cambios bruscos.
- **Alarma visual con LED** activa dos segundos si la situación de peligro permanece al finalizar el conteo.
- **Bloqueo temporal de reinicio** tras una interrupción, evita reinicios por rebotes del sensor o ambiente.

***

### Ejemplo de Flujo


<p align='center'>
    <img 
        src="https://imgur.com/Y71UD9I.png" width="138" height="570" alt="Foto sistema"
    />
</p>

1. El sistema monitorea la temperatura ambiente de forma continua.
2. Si la temperatura supera **el límite peligroso (> 20°C, por defecto)** y mantiene dos lecturas consecutivas, inicia el **contador regresivo**.
3. El display muestra los números del 9 al 0 con intervalos breves. El LED parpadea dando señal de peligro visible.
4. Si la temperatura baja durante el conteo o se detecta un cambio brusco, el sistema se **reinicia**, apaga alarmas y espera antes de rearmar.
5. Si el contador finaliza y la temperatura sigue alta, el **LED se enciende fijo como alarma durante 2 segundos**.
6. El sistema retoma vigilancia activa y espera condiciones para nuevo ciclo.
***

### Entradas y salidas

- **Entrada principal**: Sensor LM35 (pin A0) – lectura analógica de temperatura.
- **Salidas**:
  - Display 7 segmentos (pins 2-8) – visualización de número para cuenta regresiva.
  - LED indicador (pin 10) – alerta visual durante y al final del conteo de peligro.

<p align='center'>
    <img 
        src="https://capsule-render.vercel.app/api?type=rect&height=5&color=80354A&reversal=false&fontAlignY=40&fontColor=FFFFFF&fontSize=60"
    />
</p>
