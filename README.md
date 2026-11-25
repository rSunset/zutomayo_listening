# zutomayo_listening 🌙

Simulador visual de **planificación de CPU**, ambientado a medianoche.

¿Y si el procesador fuera un reproductor y los procesos fueran álbumes de
**ZUTOMAYO** (ずっと真夜中でいいのに。) esperando su turno para sonar?
Aquí el *tiempo de llegada* de cada proceso es el **lanzamiento** del álbum y su
*tiempo de servicio* es su **duración**. El programa reparte el escenario con
cuatro políticas de planificación y dibuja cómo se ejecutaría cada una.

> Proyecto de clase hecho por un fan. No tiene ninguna relación con ZUTOMAYO
> ni con sus sellos.

## Qué hace

- Genera cinco álbumes con datos aleatorios: el álbum **A** siempre llega en
  `t = 0`, los demás llegan en `[0, 15]`, y todos duran entre `1` y `10`
  unidades de tiempo (enteras).
- Ejecuta cuatro algoritmos sobre esos datos y muestra un diagrama de Gantt
  por cada uno, con una fila por álbum:

| Algoritmo | Idea | ¿Apropiativo? |
|---|---|---|
| **FCFS** | primero en llegar, primero en servirse | no |
| **SPN** | primero el álbum más corto | no |
| **SRT** | primero el de menor tiempo restante | sí |
| **HRRN** | primero el de mayor tasa de respuesta | no |

- En los empates gana el álbum que llegó antes y, si persiste, el de menor `id`.
- El botón **Generar** crea una semilla nueva y recalcula todo. La tabla de la
  izquierda muestra la portada, el título, el lanzamiento y la duración de cada
  álbum, y su color es el mismo que usan sus barras en los diagramas.

## Requisitos

- Compilador con soporte de **C++20**
- **CMake** 4.3 o superior
- **SFML 2.6** (módulos `graphics`, `window` y `system`, con sus archivos de
  desarrollo)

## Compilar y ejecutar

```bash
cmake -S . -B build
cmake --build build
./build/zutomayo_listening
```

Pruebas de la lógica (los planificadores se comprueban con ejemplos calculados
a mano):

```bash
ctest --test-dir build --output-on-failure
```

## Estructura

```
src/
  models/      datos puros: Album, ExecutionSegment, SchedulingAlgorithm
  logic/       generador aleatorio y planificadores (sin SFML)
  interface/   dibujo con SFML: tabla, diagramas de Gantt, portadas
  main.cpp     une las tres capas y gestiona la ventana
tests/         pruebas de la lógica (assert + CTest)
assets/        fuente y, opcionalmente, portadas
```

`logic/` no conoce SFML, y por eso se prueba sin abrir ninguna ventana.

## Recursos (`assets/`)

- **Fuente:** `regularsans.ttf` es Open Sans, con licencia Apache 2.0
  (texto en `assets/LICENSE-OpenSans.txt`).
- **Portadas:** no se incluyen en el repositorio porque pertenecen a sus
  autores. Son opcionales: si faltan, el programa avisa por consola y dibuja un
  cuadro con la letra del álbum. Si quieres verlas, guarda tus propias imágenes
  en `assets/` con los nombres que aparecen en el catálogo de `src/main.cpp`.

## Autor
#### - Cristopher Roque