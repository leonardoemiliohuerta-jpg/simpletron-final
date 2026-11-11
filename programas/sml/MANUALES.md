# Programas SML escritos a mano

## manual_suma.sml: suma dos numeros
| Dir | Instruccion | Significado |
|---|---|---|
| 00 | +1007 | READ: lee A en la 07 |
| 01 | +1008 | READ: lee B en la 08 |
| 02 | +2007 | LOAD: carga A al acumulador |
| 03 | +3008 | ADD: suma B |
| 04 | +2109 | STORE: guarda el resultado en la 09 |
| 05 | +1109 | WRITE: imprime la 09 |
| 06 | +4300 | HALT |
| 07-09 | +0000 | Area de datos: A, B, resultado |

Prueba: entradas 3 y 4, salida 7.

## manual_mayor.sml: imprime el mayor de dos numeros
| Dir | Instruccion | Significado |
|---|---|---|
| 00 | +1009 | READ: lee A en la 09 |
| 01 | +1010 | READ: lee B en la 10 |
| 02 | +2009 | LOAD A |
| 03 | +3110 | SUBTRACT B (A - B) |
| 04 | +4107 | BRANCHNEG a la 07 si A < B |
| 05 | +1109 | WRITE A |
| 06 | +4300 | HALT |
| 07 | +1110 | WRITE B |
| 08 | +4300 | HALT |
| 09-10 | +0000 | Area de datos: A, B |

Pruebas: 2 y 9 imprime 9; 7 y 3 imprime 7.
