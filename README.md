# Simpletron: simulador SML y compilador de Simple

Proyecto final de Programación Avanzada (UPAEP). Incluye una computadora virtual
**Simpletron**, su lenguaje máquina **SML**, un compilador del lenguaje **Simple** a SML
y la conversión/evaluación de expresiones infijas a postfijas.

**Integrante:** Leonardo Huerta

## Atribución y apoyo utilizado
- El código base del simulador y del compilador me fue proporcionado por otra persona.
- Utilicé el asistente de IA Claude como apoyo para entender el código, corregirlo y extenderlo
  (operadores `>`, `<=`, `>=`, `!=`, validación de paréntesis, manejo de errores), y para armar
  las pruebas y la documentación.
- Yo revisé el código y lo ejecuté en mi equipo.

## Estructura
```
src/simpletron.c     Simulador, conversión infija->postfija y compilador (un solo archivo)
programas/simple/    Programas de prueba en Simple
programas/sml/       SML generado por el compilador
pruebas/             Script de pruebas, entradas y salidas
docs/                Requerimientos y diseño
evidencias/          Capturas de ejecución y de Git
```

## Compilación
```
gcc src/simpletron.c -o simple -lm
```
(`-lm` es necesario porque se usa `pow`.)

## Ejecución
```
./simple --compile programas/simple/suma.simple programas/sml/suma.sml
./simple --run programas/sml/suma.sml
```
En Windows usar `simple.exe` en lugar de `./simple`.

## Ejemplo
Entrada del programa `suma.simple` con `3` y `4`:
```
? ? 7
REGISTROS: ...  (después se imprime el volcado de memoria)
```

## Lenguaje Simple soportado
`rem`, `input`, `let`, `print`, `goto`, `if ... goto`, `end`. Variables de una letra, constantes
enteras, operadores `+ - * / % ^` con paréntesis, y condiciones `< <= > >= == !=`.

## Pruebas
```
bash pruebas/correr_pruebas.sh
```
Compila cada programa, lo ejecuta con entradas fijas y compara con la salida esperada (10 casos).

## Archivos principales
| Función | Qué hace |
|---|---|
| `pasar_a_postfija` | Algoritmo de pila; respeta precedencia y `^` asociativo por la derecha |
| `correr_vm` | Ciclo traer-decodificar-ejecutar del Simpletron, con errores fatales |
| `procesar_linea` | Primera pasada del compilador; marca saltos hacia adelante como pendientes |
| `compilar` | Segunda pasada: resuelve saltos pendientes y escribe el `.sml` |
| `emitir_salto` | Genera un salto o lo deja pendiente si la línea destino aún no existe |

## Limitaciones conocidas
Sin números negativos literales (`let x = -5`), sin variables de más de una letra obligatoria
(se aceptan nombres largos), sin detección de desbordamiento a 4 dígitos.
