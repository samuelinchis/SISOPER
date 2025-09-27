# Ejecución del programa `procesador.cpp`

Para ejecutar este programa es necesario contar con un compilador de C++ (por ejemplo, **g++**).

## Pasos de ejecución

1. Asegúrate de tener instalados los archivos:

   * `procesador.cpp`
   * Los archivos `.txt` que se utilizarán como entrada
     Todos deben estar en la **misma carpeta**.

2. Abre una terminal (Shell, CMD o PowerShell) y navega hasta la carpeta donde están los archivos.

3. Compila el programa con:

   ```bash
   g++ procesador.cpp -o procesador
   ```

   Esto creará un ejecutable llamado `procesador.exe` en Windows, o `procesador` en Linux/Mac.

4. Ejecuta el programa:

   * En **Windows**:

     ```bash
     .\procesador.exe
     ```
   * En **Linux/Mac**:

     ```bash
     ./procesador
     ```

## Cambiar el archivo de lectura

Por defecto, el programa lee un archivo `.txt` especificado dentro del código fuente.
Si deseas cambiarlo, edita la línea en `procesador.cpp` donde se abre el archivo, por ejemplo:

```cpp
ifstream archivo("datos.txt");
```

y reemplaza `"datos.txt"` por el nombre completo (con extensión) del archivo que quieras leer.
