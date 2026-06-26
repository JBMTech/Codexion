*This project has been created as part of the 42 curriculum by jabuleje*

# Codexion

**Codexion** es un simulador concurrente desarrollado en C utilizando POSIX Threads
(pthreads). El proyecto simula un conjunto de programadores ("coders") que
comparten un número limitado de dongles para poder compilar código.

La simulación implementa dos algoritmos de planificación:

- FIFO (First In, First Out)
- EDF (Earliest Deadline First)

---

# Objetivos

- Aprender programación concurrente.
- Evitar condiciones de carrera (Data Races).
- Gestionar exclusión mutua mediante mutex.
- Sincronizar hilos mediante variables de condición.
- Implementar dos algoritmos de planificación.

---

# Compilación

```bash
make
```

# Uso

```bash
./codexion <number_of_coders> <burnout_time> <compile_time><debug_time> <refactor_time> <compile_required> <dongle_cooldown> <fifo|edf>
```

Ejemplo:

```bash
./codexion 5 550 100 100 100 3 50 fifo
```

---

# Parámetros

| Parámetro | Descripción |
|-----------|-------------|
| number_of_coders | Número de coders. |
| burnout_time | Tiempo máximo sin comenzar una compilación. |
| compile_time | Tiempo de compilación. |
| debug_time | Tiempo de depuración. |
| refactor_time | Tiempo de refactorización. |
| compile_required | Número de compilaciones requeridas por coder. |
| dongle_cooldown | Tiempo que un dongle permanece bloqueado tras liberarse. |
| scheduler | fifo o edf. |

---

# Ciclo de vida de un coder

Cada coder realiza continuamente:

```
Obtener dos dongles
        │
        ▼
Compilar
        │
        ▼
Liberar dongles
        │
        ▼
Debug
        │
        ▼
Refactor
        │
        ▼
Repetir
```

Si pasa más tiempo que `burnout_time` sin comenzar una compilación:

```
Coder burnout
```

La simulación finaliza inmediatamente.

---

# Algoritmo FIFO

Cada dongle mantiene su propia cola FIFO.

Cuando un coder solicita un dongle:

1. Se añade a la cola del dongle.
2. Espera su turno.
3. Cuando es el primero y el dongle está libre:
   - obtiene el dongle.
4. Si tiene los dos dongles:
   - Compila.

---

# Algoritmo EDF

Cada dongle mantiene una cola ordenada por deadline.

El deadline se calcula como:

```
deadline = last_compile_start + burnout_time
```

EL coder cuyo deadline es menor tiene prioridad.

---

# Comprobaciones

**Memory leaks**

```bash
valgrind ./codexion 5 1000 300 10 10 3 10 edf
```

**Data races**

```bash
valgrind --tool=drd ./codexion 3 500 50 50 50 3 10 edf
```

La ejecucion de Valgrind produce retraso en la ejecucion de codexion, se recomienda un numero reducido de coders, tiempo de compilación, tiempo de debuger, tiempo de refactorización, tiempo de cooldown y numero de compilaciones. Y más tiempo de burnout.

---

# Additionnal informations

## Blocking cases handled 

### Deadlock prevention

El principal objetivo era permitir que los coders obtuvieran los dos dongles necesarios para compilar manteniendo la ejecución concurrente y evitando situaciones de interbloqueo (deadlock).

Para conseguirlo se utilizó una estrategia basada en el identificador de cada coder. Los coders con identificador impar intentan adquirir primero el dongle izquierdo y después el derecho, mientras que los coders con identificador par realizan la operación en el orden inverso.

Esta estrategia rompe el orden único de adquisición de recursos. Si todos los hilos intentaran obtener primero el mismo dongle, podría producirse una espera circular en la que cada coder mantuviera un recurso mientras espera indefinidamente el otro. Al alternar el orden de adquisición según la paridad del identificador, se elimina esa condición y se evita el interbloqueo.

Además, los coders con identificador par realizan un pequeño retraso antes de comenzar su ejecución. Este desfase temporal reduce la contención inicial sobre los dongles, distribuye mejor las solicitudes de los recursos y favorece un mayor grado de paralelismo durante la simulación.

## Thread synchronization mechanisms 

### Serialización de registros

Para garantizar que los mensajes de registro sean legibles y consistentes, el acceso a `printf` está protegido por un mutex dedicado. Antes de escribir en la consola, cada programador debe adquirir este mutex, lo que garantiza que solo un hilo pueda imprimir a la vez.

Esto evita que la salida de varios hilos se mezcle, lo que produce registros deterministas y legibles que reflejan con precisión el orden de los eventos.

### Detección y finalización del agotamiento

La detección del agotamiento se gestiona mediante un hilo de monitorización dedicado que comprueba continuamente si algún programador ha superado el tiempo permitido sin compilar.

Separar esta responsabilidad de los hilos de trabajo centraliza la lógica de detección y permite identificar el agotamiento en cuanto se produce, independientemente de lo que estén haciendo los programadores. El hilo de monitorización finaliza solo cuando termina la simulación, ya sea porque un programador se agota o porque se han completado todas las compilaciones necesarias. El hilo principal espera (`pthread_join`) a que el hilo de monitorización finalice antes de cerrar el programa, lo que garantiza una finalización limpia y sincronizada.

# Recursos
- https://dev.to/yel-bakk/codexion-4fk8 concepto de codexion.
- https://dev.to/yel-bakk/thread-in-c-codexion-42-1ao hilos de codexion.
- https://www.youtube.com/watch?v=d9s_d28yJq0&list=PLfqABt5AS4FmuQf70psXrsMLEDQXNkLq2 to understand multi-threading in C.
