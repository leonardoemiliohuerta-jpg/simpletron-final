#!/bin/bash
# Compila y ejecuta cada caso: programa .simple + entradas -> compara la salida con la esperada.
# Uso: bash pruebas/correr_pruebas.sh   (desde la raiz del repositorio)
set -u
gcc -Wall src/simpletron.c -o simple -lm || exit 1
ok=0; fallo=0

probar() {  # nombre programa "entradas" esperado
    local nombre=$1 prog=$2 entradas=$3 esperado=$4
    ./simple --compile "programas/simple/$prog.simple" "programas/sml/$prog.sml" > /dev/null
    local salida
    salida=$(echo "$entradas" | ./simple --run "programas/sml/$prog.sml" | grep -v '^$' | sed -n '1p' | sed 's/^\(? \)*//')
    echo "$entradas -> $salida" > "pruebas/salidas/$nombre.txt"
    echo "$entradas" > "pruebas/entradas/$nombre.txt"
    if [ "$salida" == "$esperado" ]; then echo "OK    $nombre"; ok=$((ok+1))
    else echo "FALLO $nombre (esperado $esperado, salio $salida)"; fallo=$((fallo+1)); fi
}

probar suma            suma        "3 4"     7
probar mayor_a         mayor       "7 3"     7
probar mayor_b         mayor       "2 9"     9
probar contador_5      contador    "5"       15
probar contador_0      contador    "0"       0
probar distinto_si     relaciones  "4 9"     9
probar distinto_no     relaciones  "5 5"     5
probar expresiones     expresiones "2 3 4"   -108
probar menor_igual_si   limites     "3 3"     1
probar menor_igual_no   limites     "5 3"     0

echo; echo "Pasaron: $ok  Fallaron: $fallo"
