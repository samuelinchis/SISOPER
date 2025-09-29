============================================================
   SIMULADOR DE ALGORITMOS DE PLANIFICACIÓN DE CPU EN C++
============================================================

Fecha de creación: 14 de septiembre de 2025
Autor: Juan David Vasquez, Samuel Rueda, Daniela Tellez, David Bayona

------------------------------------------------------------
1. DESCRIPCIÓN DEL PROYECTO
------------------------------------------------------------

Este programa es un simulador desarrollado en C++ que implementa y evalúa cuatro algoritmos fundamentales de planificación de procesos de un sistema operativo:

1.  **FCFS (First-Come, First-Served)**: Atiende los procesos en el estricto orden en que llegan.
2.  **SJF (Shortest Job First)**: Selecciona el proceso no apropiativo con el tiempo de ráfaga más corto.
3.  **STCF (Shortest Time to Completion First)**: Versión apropiativa de SJF, que puede interrumpir un proceso si llega otro más corto.
4.  **RR (Round Robin)**: Asigna a cada proceso una pequeña porción de tiempo de CPU (quantum) y rota entre ellos.

El simulador lee los datos desde un archivo, ejecuta el algoritmo seleccionado y muestra métricas de rendimiento y un diagrama de Gantt.

------------------------------------------------------------
2. DISEÑO Y CREACIÓN DE LA SOLUCIÓN
------------------------------------------------------------

La solución se diseñó siguiendo un enfoque modular y orientado a objetos para garantizar que el código fuera organizado, reutilizable y fácil de entender.

### 2.1. Representación del Proceso (`class Process`)

El primer paso fue definir una estructura de datos para contener toda la información de un proceso. Se eligió una `class` en lugar de una `struct` para encapsular los datos de manera más formal, aunque en esta implementación todos sus miembros son públicos para facilitar el acceso.

La clase `Process` contiene:
- **Datos de Entrada**: `label`, `burstTime`, `arrivalTime`, etc., que se leen del archivo.
- **Métricas de Rendimiento**: `completionTime`, `turnaroundTime`, `waitingTime`, y `responseTime` para almacenar los resultados del cálculo.
- **Variables de Estado**: `remainingTime` y `responded` para ayudar a los algoritmos apropiativos (como RR y STCF) a rastrear el progreso de un proceso.

### 2.2. El Planificador Central (`class CPUScheduler`)

Para evitar tener funciones globales y variables dispersas, se creó una clase `CPUScheduler`. Esta clase actúa como el motor principal del simulador y es responsable de:

1.  **Almacenar los Procesos**: Contiene un `std::vector<Process>` como miembro privado, que es la lista maestra de todos los procesos.
2.  **Cargar los Datos (`loadProcesses`)**: Este método se encarga de abrir y leer el archivo de entrada. Utiliza `stringstream` para parsear cada línea delimitada por punto y coma (';'). Se incluyó un bloque `try-catch` para manejar errores de formato en los números, haciendo el programa más robusto ante archivos de entrada imperfectos.
3.  **Reiniciar el Estado (`resetProcesses`)**: Una de las decisiones clave fue añadir este método. Antes de ejecutar cada algoritmo, las métricas y el tiempo restante de cada proceso se resetean a sus valores iniciales. Esto es **fundamental** para que la ejecución de un algoritmo no altere los datos de entrada para el siguiente, garantizando resultados independientes y correctos.

### 2.3. Implementación de los Algoritmos

Cada algoritmo se implementó como un método dentro de la clase `CPUScheduler`.

-   **FCFS**: Es el más simple. Se ordena el vector de procesos por `arrivalTime`. Luego, se recorre la lista secuencialmente, simulando la ejecución y calculando los tiempos. Si hay inactividad en la CPU, el tiempo actual se adelanta hasta la llegada del siguiente proceso.

-   **SJF (No Expropiativo)**: Este algoritmo necesita una estructura que mantenga los procesos llegados ordenados por su `burstTime`. Se utilizó una **cola de prioridad (`std::priority_queue`)** con un comparador personalizado (`CompareBurst`). La simulación avanza en el tiempo, añadiendo procesos a la cola de prioridad a medida que llegan. Cuando la CPU está libre, se extrae el proceso con el `burstTime` más corto de la cola y se ejecuta hasta completarse.

-   **STCF (Expropiativo)**: Es similar a SJF pero con interrupciones. Utiliza una cola de prioridad que ordena por el **tiempo restante (`remainingTime`)**. La simulación avanza unidad por unidad de tiempo. En cada instante, el proceso en la cima de la cola (el más corto) se ejecuta por un solo tick. Luego, se vuelve a insertar en la cola. Esto asegura que si en el siguiente tick llega un proceso aún más corto, este nuevo proceso tendrá la prioridad.

-   **RR (Round Robin)**: Utiliza una **cola estándar (`std::queue`)** para mantener el orden de llegada (FIFO). Los procesos se añaden a la cola a medida que llegan. Se extrae un proceso, se ejecuta por un `timeSlice` (el mínimo entre el `quantum` y su tiempo restante) y, si no ha terminado, se vuelve a encolar al final.

### 2.4. Visualización (`printResults` y `printGanttChart`)

Para presentar los resultados de forma clara, se crearon dos funciones:
-   `printResults`: Muestra una tabla formateada con las métricas de cada proceso y los promedios finales.
-   `printGanttChart`: Genera un diagrama de Gantt en la terminal. Para ello, cada algoritmo construye un `vector<pair<string, int>>` que registra la secuencia de ejecución (qué proceso y por cuánto tiempo). Esta función luego dibuja el diagrama basándose en ese registro.

### 2.5. Controlador Principal (`main`)

La función `main` es el punto de entrada y se diseñó para ser un controlador simple e interactivo. Su única responsabilidad es interpretar los argumentos de la línea de comandos (algoritmo y archivo), crear una instancia de `CPUScheduler` y llamar a los métodos correspondientes.

------------------------------------------------------------
3. CÓMO COMPILAR Y EJECUTAR
------------------------------------------------------------

### 3.1. Compilación
Navega con la terminal hasta la carpeta donde guardaste el archivo `scheduler.cpp` y ejecuta:

   g++ -std=c++17 -o scheduler scheduler.cpp

### 3.2. Ejecución
Usa la siguiente sintaxis:

   ./scheduler <ALGORITMO> <ARCHIVO_DE_ENTRADA> [QUANTUM]

   - **<ALGORITMO>**: Elige entre `FCFS`, `SJF`, `STCF`, o `RR`.
   - **<ARCHIVO_DE_ENTRADA>**: El nombre de tu archivo de datos (ej. `input.txt`).
   - **[QUANTUM]**: Un número entero, obligatorio solo para el algoritmo `RR`.
