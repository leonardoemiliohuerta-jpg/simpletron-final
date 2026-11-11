#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define MEM_SIZE 100

// Jerarquia de operadores para el paso a postfijo
int obtener_prioridad(char op) {
    if (op == '^') return 3;
    if (op == '*' || op == '/' || op == '%') return 2;
    if (op == '+' || op == '-') return 1;
    return 0;
}

// Convierte una expresion infija a postfija
void pasar_a_postfija(const char *expr, char salida[][32], int *total) {
    char pila[100];
    int tope = -1;
    int i = 0;
    *total = 0;

    while (expr[i] != '\0') {
        if (isspace((unsigned char)expr[i])) {
            i++;
            continue;
        }

        if (isalnum((unsigned char)expr[i])) {
            int k = 0;
            while (isalnum((unsigned char)expr[i])) {
                salida[*total][k++] = expr[i++];
            }
            salida[*total][k] = '\0';
            (*total)++;
        } else if (expr[i] == '(') {
            pila[++tope] = expr[i++];
        } else if (expr[i] == ')') {
            while (tope >= 0 && pila[tope] != '(') {
                salida[*total][0] = pila[tope--];
                salida[*total][1] = '\0';
                (*total)++;
            }
            if (tope >= 0) tope--;
            i++;
        } else {
            char op = expr[i++];
            int p_op = obtener_prioridad(op);
            // '^' es asociativo por la derecha: solo saca los de prioridad estrictamente mayor
            while (tope >= 0 && pila[tope] != '(') {
                int p_tope = obtener_prioridad(pila[tope]);
                if (p_tope > p_op || (p_tope == p_op && op != '^')) {
                    salida[*total][0] = pila[tope--];
                    salida[*total][1] = '\0';
                    (*total)++;
                } else {
                    break;
                }
            }
            pila[++tope] = op;
        }
    }

    while (tope >= 0) {
        salida[*total][0] = pila[tope--];
        salida[*total][1] = '\0';
        (*total)++;
    }
}

// Estructura para el simulador Simpletron
typedef struct {
    int memoria[MEM_SIZE];
    int acumulador;
    int contador_inst;
    int registro_inst;
    int codigo_op;
    int operando;
} Simpletron;

void iniciar_vm(Simpletron *vm) {
    for (int i = 0; i < MEM_SIZE; i++) {
        vm->memoria[i] = 0;
    }
    vm->acumulador = 0;
    vm->contador_inst = 0;
    vm->registro_inst = 0;
    vm->codigo_op = 0;
    vm->operando = 0;
}

void mostrar_dump(Simpletron *vm) {
    printf("\nREGISTROS:\n");
    printf("acumulador:          %+05d\n", vm->acumulador);
    printf("contador_inst:          %02d\n", vm->contador_inst);
    printf("registro_inst:       %+05d\n", vm->registro_inst);
    printf("codigo_op:              %02d\n", vm->codigo_op);
    printf("operando:               %02d\n\n", vm->operando);

    printf("MEMORIA:\n   ");
    for (int i = 0; i < 10; i++) {
        printf("%6d", i);
    }
    printf("\n");

    for (int fila = 0; fila < MEM_SIZE; fila += 10) {
        printf("%02d ", fila);
        for (int col = 0; col < 10; col++) {
            printf(" %+05d", vm->memoria[fila + col]);
        }
        printf("\n");
    }
}

int cargar_sml(Simpletron *vm, const char *nombre_arch) {
    FILE *arch = fopen(nombre_arch, "r");
    if (!arch) {
        printf("No se pudo abrir el archivo: %s\n", nombre_arch);
        return 0;
    }
    iniciar_vm(vm);
    int valor, dir = 0;
    while (dir < MEM_SIZE && fscanf(arch, "%d", &valor) == 1) {
        vm->memoria[dir++] = valor;
    }
    fclose(arch);
    return 1;
}

void correr_vm(Simpletron *vm) {
    while (vm->contador_inst >= 0 && vm->contador_inst < MEM_SIZE) {
        vm->registro_inst = vm->memoria[vm->contador_inst];
        vm->codigo_op = abs(vm->registro_inst) / 100;
        vm->operando = abs(vm->registro_inst) % 100;
        if (vm->registro_inst < 0) {
            vm->codigo_op = -vm->codigo_op;
        }
        vm->contador_inst++;

        switch (vm->codigo_op) {
            case 10: // READ
                printf("? ");
                scanf("%d", &vm->memoria[vm->operando]);
                break;
            case 11: // WRITE
                printf("%d\n", vm->memoria[vm->operando]);
                break;
            case 12: // NEWLINE
                printf("\n");
                break;
            case 20: // LOAD
                vm->acumulador = vm->memoria[vm->operando];
                break;
            case 21: // STORE
                vm->memoria[vm->operando] = vm->acumulador;
                break;
            case 30: // ADD
                vm->acumulador += vm->memoria[vm->operando];
                break;
            case 31: // SUBTRACT
                vm->acumulador -= vm->memoria[vm->operando];
                break;
            case 32: // DIVIDE
                if (vm->memoria[vm->operando] == 0) {
                    printf("*** Error: Division entre cero ***\n");
                    mostrar_dump(vm);
                    return;
                }
                vm->acumulador /= vm->memoria[vm->operando];
                break;
            case 33: // MULTIPLY
                vm->acumulador *= vm->memoria[vm->operando];
                break;
            case 34: // MODULO
                if (vm->memoria[vm->operando] == 0) {
                    printf("*** Error: Modulo entre cero ***\n");
                    mostrar_dump(vm);
                    return;
                }
                vm->acumulador %= vm->memoria[vm->operando];
                break;
            case 35: // POTENCIA
                vm->acumulador = (int)pow(vm->acumulador, vm->memoria[vm->operando]);
                break;
            case 40: // BRANCH
                vm->contador_inst = vm->operando;
                break;
            case 41: // BRANCHNEG
                if (vm->acumulador < 0) vm->contador_inst = vm->operando;
                break;
            case 42: // BRANCHZERO
                if (vm->acumulador == 0) vm->contador_inst = vm->operando;
                break;
            case 43: // HALT
                mostrar_dump(vm);
                return;
            default:
                printf("*** Error: Instruccion desconocida (%d) ***\n", vm->registro_inst);
                mostrar_dump(vm);
                return;
        }
    }
}

// Estructuras para la tabla de simbolos y el compilador
typedef struct {
    char nombre[32];
    char tipo; // 'v' variable, 'c' constante, 'l' linea
    int ubicacion;
} Simbolo;

typedef struct {
    int pos_instruccion;
    int linea_destino;
} Pendiente;

typedef struct {
    Simbolo tabla[100];
    int num_simbolos;
    Pendiente pendientes[100];
    int num_pendientes;
    int memoria_sml[MEM_SIZE];
    int inst_actual;
    int dir_datos;
    int error;
} Compilador;

void iniciar_compilador(Compilador *comp) {
    comp->num_simbolos = 0;
    comp->num_pendientes = 0;
    for (int i = 0; i < MEM_SIZE; i++) {
        comp->memoria_sml[i] = 0;
    }
    comp->inst_actual = 0;
    comp->dir_datos = 99;
    comp->error = 0;
}

// Escribe una instruccion verificando que no choque con la zona de datos
void emitir(Compilador *comp, int valor) {
    if (comp->inst_actual >= MEM_SIZE || comp->inst_actual > comp->dir_datos) {
        comp->error = 1;
        return;
    }
    comp->memoria_sml[comp->inst_actual++] = valor;
}

// Reserva una posicion de datos (variable, constante o temporal)
int reservar_dato(Compilador *comp) {
    if (comp->dir_datos < comp->inst_actual) {
        comp->error = 1;
        return 0;
    }
    return comp->dir_datos--;
}

int registrar_simbolo(Compilador *comp, const char *nombre, char tipo) {
    for (int i = 0; i < comp->num_simbolos; i++) {
        if (strcmp(comp->tabla[i].nombre, nombre) == 0 && comp->tabla[i].tipo == tipo) {
            return comp->tabla[i].ubicacion;
        }
    }

    if (comp->num_simbolos >= 100 || strlen(nombre) >= 32) {
        comp->error = 1;
        return 0;
    }

    int dir;
    if (tipo == 'v' || tipo == 'c') {
        dir = reservar_dato(comp);
        if (comp->error) return 0;
        if (tipo == 'c') {
            comp->memoria_sml[dir] = atoi(nombre);
        }
    } else {
        dir = comp->inst_actual;
    }

    strcpy(comp->tabla[comp->num_simbolos].nombre, nombre);
    comp->tabla[comp->num_simbolos].tipo = tipo;
    comp->tabla[comp->num_simbolos].ubicacion = dir;
    comp->num_simbolos++;
    return dir;
}

int buscar_linea(Compilador *comp, int linea) {
    char texto[16];
    sprintf(texto, "%d", linea);
    for (int i = 0; i < comp->num_simbolos; i++) {
        if (strcmp(comp->tabla[i].nombre, texto) == 0 && comp->tabla[i].tipo == 'l') {
            return comp->tabla[i].ubicacion;
        }
    }
    return -1;
}

// Emite un salto a una linea Simple; si aun no existe lo deja pendiente
void emitir_salto(Compilador *comp, int codigo, int linea_dest) {
    int dir = buscar_linea(comp, linea_dest);
    if (dir != -1) {
        emitir(comp, codigo + dir);
    } else if (comp->num_pendientes < 100) {
        comp->pendientes[comp->num_pendientes].pos_instruccion = comp->inst_actual;
        comp->pendientes[comp->num_pendientes].linea_destino = linea_dest;
        comp->num_pendientes++;
        emitir(comp, codigo);
    } else {
        comp->error = 1;
    }
}

void procesar_linea(Compilador *comp, char *linea) {
    int num_linea;
    char comando[32] = "";
    char argumentos[128] = "";

    if (comp->error) return;

    // Quita \r y \n (archivos creados en Windows)
    linea[strcspn(linea, "\r\n")] = '\0';

    if (sscanf(linea, "%d %31s %127[^\n]", &num_linea, comando, argumentos) < 2) {
        return;
    }

    char num_str[16];
    sprintf(num_str, "%d", num_linea);
    registrar_simbolo(comp, num_str, 'l');

    if (strcmp(comando, "rem") == 0) {
        return;
    } else if (strcmp(comando, "input") == 0) {
        emitir(comp, 1000 + registrar_simbolo(comp, argumentos, 'v'));
    } else if (strcmp(comando, "print") == 0) {
        emitir(comp, 1100 + registrar_simbolo(comp, argumentos, 'v'));
    } else if (strcmp(comando, "goto") == 0) {
        int dest = atoi(argumentos);
        int dir = buscar_linea(comp, dest);
        if (dir != -1) {
            emitir(comp, 4000 + dir);
        } else if (comp->num_pendientes < 100) {
            comp->pendientes[comp->num_pendientes].pos_instruccion = comp->inst_actual;
            comp->pendientes[comp->num_pendientes].linea_destino = dest;
            comp->num_pendientes++;
            emitir(comp, 4000);
        } else {
            comp->error = 1;
        }
    } else if (strcmp(comando, "if") == 0) {
        char izq[32], op_rel[8], der[32], palabra_goto[8];
        int dest;
        if (sscanf(argumentos, "%31s %7s %31s %7s %d", izq, op_rel, der, palabra_goto, &dest) == 5) {
            // Cada relacion se reduce a restar y saltar segun el signo.
            // saltos[]: codigos de salto a emitir tras la resta (1 o 2).
            const char *a = izq;
            const char *b = der;
            int saltos[2];
            int n_saltos = 0;

            if (strcmp(op_rel, "==") == 0) {
                saltos[n_saltos++] = 4200;                      // a-b == 0
            } else if (strcmp(op_rel, "<") == 0) {
                saltos[n_saltos++] = 4100;                      // a-b < 0
            } else if (strcmp(op_rel, ">") == 0) {
                a = der; b = izq;                               // b-a < 0
                saltos[n_saltos++] = 4100;
            } else if (strcmp(op_rel, "<=") == 0) {
                saltos[n_saltos++] = 4100;                      // a-b < 0
                saltos[n_saltos++] = 4200;                      // o a-b == 0
            } else if (strcmp(op_rel, ">=") == 0) {
                a = der; b = izq;                               // b-a <= 0
                saltos[n_saltos++] = 4100;
                saltos[n_saltos++] = 4200;
            } else if (strcmp(op_rel, "!=") == 0) {
                saltos[n_saltos++] = 4100;                      // a-b < 0 ...
                // ... o b-a < 0: se emite abajo con los operandos invertidos
            } else {
                printf("Error: operador relacional no soportado: %s\n", op_rel);
                comp->error = 1;
                return;
            }

            int dir_a = registrar_simbolo(comp, a, isdigit((unsigned char)a[0]) ? 'c' : 'v');
            int dir_b = registrar_simbolo(comp, b, isdigit((unsigned char)b[0]) ? 'c' : 'v');
            int es_distinto = (strcmp(op_rel, "!=") == 0);

            emitir(comp, 2000 + dir_a);
            emitir(comp, 3100 + dir_b);
            for (int k = 0; k < n_saltos; k++) {
                emitir_salto(comp, saltos[k], dest);
            }
            if (es_distinto) {
                emitir(comp, 2000 + dir_b);
                emitir(comp, 3100 + dir_a);
                emitir_salto(comp, 4100, dest);
            }
        }
    } else if (strcmp(comando, "let") == 0) {
        char var_dest[32], expr[128];
        if (sscanf(argumentos, "%31s = %127[^\n]", var_dest, expr) != 2) {
            return;
        }
        // Validacion de balance de parentesis antes de convertir
        int nivel = 0;
        for (int k = 0; expr[k] != '\0'; k++) {
            if (expr[k] == '(') nivel++;
            else if (expr[k] == ')' && --nivel < 0) break;
        }
        if (nivel != 0) {
            printf("Error: parentesis desbalanceados en linea %d\n", num_linea);
            comp->error = 1;
            return;
        }
        int dir_dest = registrar_simbolo(comp, var_dest, 'v');

        char postfija[50][32];
        int n_tokens = 0;
        pasar_a_postfija(expr, postfija, &n_tokens);

        int pila[50];
        int tope = -1;

        for (int i = 0; i < n_tokens && !comp->error; i++) {
            if (isalnum((unsigned char)postfija[i][0])) {
                char tipo = isdigit((unsigned char)postfija[i][0]) ? 'c' : 'v';
                pila[++tope] = registrar_simbolo(comp, postfija[i], tipo);
            } else {
                if (tope < 1) {
                    printf("Error: expresion invalida en linea %d\n", num_linea);
                    comp->error = 1;
                    return;
                }
                int op2 = pila[tope--];
                int op1 = pila[tope--];

                emitir(comp, 2000 + op1);
                char op = postfija[i][0];
                int inst_op = 3000;
                if (op == '+') inst_op = 3000;
                else if (op == '-') inst_op = 3100;
                else if (op == '*') inst_op = 3300;
                else if (op == '/') inst_op = 3200;
                else if (op == '%') inst_op = 3400;
                else if (op == '^') inst_op = 3500;

                emitir(comp, inst_op + op2);
                int temporal = reservar_dato(comp);
                emitir(comp, 2100 + temporal);
                pila[++tope] = temporal;
            }
        }
        if (tope < 0) {
            comp->error = 1;
            return;
        }
        int resultado = pila[tope--];
        emitir(comp, 2000 + resultado);
        emitir(comp, 2100 + dir_dest);
    } else if (strcmp(comando, "end") == 0) {
        emitir(comp, 4300);
    }
}

int compilar(const char *arch_entrada, const char *arch_salida) {
    FILE *in = fopen(arch_entrada, "r");
    if (!in) {
        printf("Error al abrir el archivo fuente: %s\n", arch_entrada);
        return 0;
    }

    Compilador comp;
    iniciar_compilador(&comp);

    char linea[256];
    while (fgets(linea, sizeof(linea), in)) {
        procesar_linea(&comp, linea);
    }
    fclose(in);

    if (comp.error) {
        printf("Error de compilacion: el programa no cabe en %d posiciones de memoria "
               "(o tiene un error de sintaxis).\n", MEM_SIZE);
        return 0;
    }

    // Segunda pasada para resolver saltos pendientes
    for (int i = 0; i < comp.num_pendientes; i++) {
        int dir = buscar_linea(&comp, comp.pendientes[i].linea_destino);
        if (dir != -1) {
            comp.memoria_sml[comp.pendientes[i].pos_instruccion] += dir;
        } else {
            printf("Error: salto a la linea %d, que no existe.\n", comp.pendientes[i].linea_destino);
            return 0;
        }
    }

    FILE *out = fopen(arch_salida, "w");
    if (!out) {
        printf("No se pudo crear el archivo de salida: %s\n", arch_salida);
        return 0;
    }

    int limite = (comp.dir_datos < 99) ? MEM_SIZE : comp.inst_actual;
    for (int i = 0; i < limite; i++) {
        fprintf(out, "%+05d\n", comp.memoria_sml[i]);
    }
    fclose(out);
    printf("Compilacion exitosa: %s\n", arch_salida);
    return 1;
}

int main(int argc, char *argv[]) {
    if (argc > 3 && strcmp(argv[1], "--compile") == 0) {
        compilar(argv[2], argv[3]);
    } else if (argc > 2 && strcmp(argv[1], "--run") == 0) {
        Simpletron vm;
        if (cargar_sml(&vm, argv[2])) {
            correr_vm(&vm);
        }
    } else {
        printf("Uso:\n  %s --compile <entrada.simple> <salida.sml>\n  %s --run <archivo.sml>\n", argv[0], argv[0]);
    }
    return 0;
}
