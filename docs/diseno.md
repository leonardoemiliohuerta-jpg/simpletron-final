# Diseño y decisiones
- **Memoria:** el código crece desde la posición 0 y los datos (variables, constantes, temporales)
  bajan desde la 99; si se cruzan, el compilador reporta error en vez de corromper el programa.
- **Dos pasadas:** los `goto` hacia líneas futuras se guardan como pendientes y se resuelven al final.
- **Relaciones:** todas se reducen a restar y saltar con BRANCHNEG/BRANCHZERO.
  `a > b` se hace como `b < a`; `<=` y `>=` usan dos saltos; `!=` revisa `a-b<0` y `b-a<0`.
- **Potencia:** asociativa por la derecha (`2^3^2 = 512`).
- **Constantes:** se guardan en memoria de datos al registrarse.
