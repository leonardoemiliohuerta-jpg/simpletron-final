# Proyecto Final - Simulador Simpletron y Compilador Simple

**Materia:** Programación Avanzada (UPAEP)  
**Alumno:** Leonardo Huerta

## Descripción del proyecto
Este repositorio contiene el proyecto final de la materia, que consiste en el desarrollo de la computadora virtual **Simpletron** (capaz de ejecutar programas en lenguaje máquina **SML**) y un compilador para traducir código del lenguaje de alto nivel **Simple** a instrucciones SML, incluyendo la evaluación de expresiones aritméticas mediante conversión de notación infija a postfija.

## Estructura de carpetas
```text
src/simpletron.c     Código fuente del compilador y del simulador
programas/simple/    Programas de prueba escritos en lenguaje Simple
programas/sml/       Archivos en código máquina (.sml) generados al compilar
pruebas/             Scripts y archivos de entrada/salida para probar los casos
docs/                Documentos de requerimientos y diseño del sistema
evidencias/          Capturas de pantalla de las pruebas y del uso de ramas en Git
```

## Cómo compilar
Para compilar el programa con GCC desde la terminal (se agrega `-lm` por el uso de la librería `math.h`):

```bash
gcc src/simpletron.c -o simple -lm
```

## Cómo ejecutar

**1. Compilar un programa de Simple a SML:**
```bash
./simple --compile programas/simple/suma.simple programas/sml/suma.sml
```

**2. Correr el archivo SML en el simulador:**
```bash
./simple --run programas/sml/suma.sml
```
*(En Windows se ejecuta con `simple.exe` en lugar de `./simple`).*

### Ejemplo de uso
Al correr `suma.sml` e ingresar los valores `3` y `4`:
```text
? 3
? 4
7

REGISTROS:
...
```
Al terminar la ejecución, el programa muestra en pantalla el estado final de los registros y el vaciado completo de las 100 posiciones de memoria (memory dump).

## Comandos soportados en Simple
El compilador reconoce las siguientes instrucciones:
- `rem` para comentarios
- `input` y `print` para entrada y salida de datos
- `let` para asignación y operaciones aritméticas (`+`, `-`, `*`, `/`, `%`, `^`) con uso de paréntesis
- `goto` y `if ... goto` para saltos incondicionales y condicionales (`<`, `<=`, `>`, `>=`, `==`, `!=`)
- `end` para finalizar el programa

## Pruebas
Para correr los casos de prueba de forma automática:
```bash
bash pruebas/correr_pruebas.sh
```
Este script compila los programas de ejemplo, los ejecuta con las entradas de prueba y verifica que las salidas coincidan con los resultados esperados.

## Funcionamiento general del código
1. **Conversión de expresiones:** Se utiliza una pila para pasar las operaciones de los comandos `let` de notación infija a postfija respetando la jerarquía de operadores.
2. **Compilador (dos pasadas):** En la primera pasada lee línea por línea el archivo `.simple`, guarda variables, constantes y números de línea en la tabla de símbolos, y traduce cada instrucción a SML. Si encuentra un salto a una línea que todavía no aparece, lo guarda como pendiente y en la segunda pasada completa la dirección exacta antes de crear el archivo `.sml`.
3. **Simulador Simpletron:** Carga las instrucciones del archivo `.sml` en el arreglo de memoria de 100 posiciones y las va ejecutando una por una con el acumulador y el contador de instrucciones, validando errores como división entre cero o códigos de operación inválidos.

## Herramientas de apoyo
Durante el desarrollo me apoyé en Claude para resolver algunas dudas sobre el manejo de cadenas en C, revisar detalles en las condiciones de salto y verificar que los casos de prueba funcionaran bien.
