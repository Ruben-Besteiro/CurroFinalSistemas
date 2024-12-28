#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAMAGNO_BLOQUE 512		// Todo esto sale en el pdf
#define MAX_BLOQUES 100
#define MAX_INODOS 25000			// Esto tuve que cambiarlo
#define NUM_BLOQUES_POR_CADA_INODO 7
#define LEN_NFICH 20

typedef struct {
    int cantidadDeInodos;
    int cantidadDeBloques;
    int bloquesLibres;
    int inodosLibres;
    int primerBloqueNoReservado;
    int tamañoBloque;
    char s_relleno[TAMAGNO_BLOQUE - 6 * sizeof(unsigned int)];
} _superbloque;

typedef struct {
    unsigned char bytemapBloques[MAX_BLOQUES];
    unsigned char bytemapInodos[20];
    unsigned char bytemapRelleno[TAMAGNO_BLOQUE - (MAX_BLOQUES + 24) * sizeof(char)];
} _bytemap;

typedef struct {
    unsigned int tamañoFichero;
    unsigned short int bloques[NUM_BLOQUES_POR_CADA_INODO];
} _inodo;

typedef struct {
    unsigned char archivo[LEN_NFICH];
    unsigned short int inodo;
} _directorio;

// Variables globales
_superbloque superbloque;
_bytemap bytemaps;
_inodo lista_inodos[20];		// MAX_INODOS se supone que es 20 pero lo cambié a 25000
_directorio directorio[20];
char particion[MAX_BLOQUES][TAMAGNO_BLOQUE];

void main() {
	FILE *f = fopen("particion.bin", "rb");
    if (!f) {
        printf("Error al abrir el archivo");
        exit(1);
    }
    fread(particion, TAMAGNO_BLOQUE, MAX_BLOQUES, f);
    fclose(f);

    memcpy(&superbloque, particion[0], sizeof(_superbloque));		// Bloques 0-3
    memcpy(&bytemaps, particion[1], sizeof(_bytemap));
    memcpy(lista_inodos, particion[2], sizeof(lista_inodos));
    memcpy(directorio, particion[3], sizeof(directorio));
    
	char comando[10];
    while (1) {
        printf(">> ");
        scanf("%s", comando);
        if (strcmp(comando, "salir") == 0) {
			printf("Que tenga un buen dia\n");
			fclose(f);
            break;
        } else if (strcmp(comando, "info") == 0) {
            comando_info();
        } else if (strcmp(comando, "bytemaps") == 0) {
            comando_bytemaps();
        } else if (strcmp(comando, "dir") == 0) {
            comando_dir();
        } else if (strcmp(comando, "imprimir") == 0) {
            char nombre[LEN_NFICH];
            comando_imprimir();
        } else if (strcmp(comando, "remove") == 0) {
            char nombre[LEN_NFICH];
            scanf("%s", nombre);
            comando_remove(nombre);
        } else if (strcmp(comando, "copy") == 0) {
            char origen[LEN_NFICH], destino[LEN_NFICH];
            scanf("%s %s", origen, destino);
            comando_copy(origen, destino);
        } else if (strcmp(comando, "rename") == 0) {
            char antiguo[LEN_NFICH], nuevo[LEN_NFICH];
            scanf("%s %s", antiguo, nuevo);
            comando_rename(antiguo, nuevo);
        } else {
            printf("Comando desconocido\n");
        }
    }
}

void comando_info() {
	printf("INFORMACION DEL SUPERBLOQUE\n");
    printf("Inodos totales: %d\n", superbloque.cantidadDeInodos);
    printf("Bloques totales: %d\n", superbloque.cantidadDeBloques);
    printf("Bloques libres: %d\n", superbloque.bloquesLibres);
    printf("Inodos libres: %d\n", superbloque.inodosLibres);
    printf("Primer bloque de datos: %d\n", superbloque.primerBloqueNoReservado);
    printf("Tamagno de bloque: %d\n", superbloque.tamañoBloque);
}

void comando_bytemaps() {
	printf("BYTEMAPS (0 SIGNIFICA LIBRE Y 1 SIGNIFICA OCUPADO)\n");
    printf("Bloques: ");
    for (int i = 0; i < 25; i++) {
        printf("%d ", bytemaps.bytemapBloques[i]);
    }
    printf("\nInodos: ");
    for (int i = 0; i < 20; i++) {
        printf("%d ", bytemaps.bytemapInodos[i]);
    }
    printf("\n");
}

void comando_dir() {
    printf("LISTA DE FICHEROS:\n");
    for (int i = 0; i < 20; i++) {
        if (directorio[i].inodo != 0xFFFF) {
            if (directorio[i].inodo < MAX_INODOS) {
                _inodo *inodx = &lista_inodos[directorio[i].inodo];
                if (inodx->tamañoFichero > 0) {
                    printf("Nombre: %s, Tamagno: %d, Inodo: %d, Bloques: ",  directorio[i].archivo, inodx->tamañoFichero, directorio[i].inodo);
                    for (int j = 0; j < NUM_BLOQUES_POR_CADA_INODO; j++) {
                        if (inodx->bloques[j] != 0xFFFF && inodx->bloques[j] < MAX_BLOQUES) {
                            printf("%d ", inodx->bloques[j]);
                        }
                    }
                    printf("\n");
                }
			}
        }
    }
}


void comando_imprimir() {
	char nombre[20];
	scanf("%s", &nombre);
	
    for (int i = 0; i < 20; i++) {
        if (!strcmp(directorio[i].archivo, nombre) && directorio[i].inodo != 0xFFFF) {
            _inodo *inodx = &lista_inodos[directorio[i].inodo];
            for (int j = 0; j < NUM_BLOQUES_POR_CADA_INODO; j++) {
                if (inodx->bloques[j] != 0xFFFF) {
                    fwrite(particion[inodx->bloques[j]], 1, TAMAGNO_BLOQUE, stdout);
                }
            }
            printf("\n");
            return;
        }
    }
    printf("Fichero no encontrado\n");
}

void comando_remove(char *nombre) {
    for (int i = 0; i < 20; i++) {
        if (!strcmp(directorio[i].archivo, nombre) && directorio[i].inodo != 0xFFFF) {
            unsigned short int inodo_index = directorio[i].inodo;
            _inodo *inodx = &lista_inodos[inodo_index];

            for (int j = 0; j < NUM_BLOQUES_POR_CADA_INODO; j++) {		// Segmentation fault en este for
                if (inodx->bloques[j] != 0xFFFF) {
                    bytemaps.bytemapBloques[inodx->bloques[j]] = 0;
                    inodx->bloques[j] = 0xFFFF;
                }
            }

            bytemaps.bytemapInodos[inodo_index] = 0;
            inodx->tamañoFichero = 0;
            directorio[i].inodo = 0xFFFF;
            memset(directorio[i].archivo, 0, LEN_NFICH);

            printf("Fichero eliminado\n");
            return;
        }
    }
    printf("Fichero no encontrado\n");
}

void comando_copy(char *origen, char *destino) {
    int inodo_origen = -1;
    for (int i = 0; i < 20; i++) {
        if (!strcmp(directorio[i].archivo, origen) && directorio[i].inodo != 0xFFFF) {
            inodo_origen = directorio[i].inodo;
            break;
        }
    }
    if (inodo_origen == -1) {
        printf("Fichero origen no encontrado\n");
        return;
    }

    for (int i = 0; i < 20; i++) {
        if (!strcmp(directorio[i].archivo, destino)) {
            printf("Fichero destino ya existe\n");
            return;
        }
    }

    int inodo_libre = -1;
    for (int i = 0; i < MAX_INODOS; i++) {
        if (bytemaps.bytemapInodos[i] == 0) {
            inodo_libre = i;
            break;
        }
    }

    int entrada_libre = -1;
    for (int i = 0; i < 20; i++) {
        if (directorio[i].inodo == 0xFFFF) {
            entrada_libre = i;
            break;
        }
    }

    _inodo *inodo_destino = &lista_inodos[inodo_libre];
    _inodo *inodo_origen_ptr = &lista_inodos[inodo_origen];

    inodo_destino->tamañoFichero = inodo_origen_ptr->tamañoFichero;
    for (int j = 0; j < NUM_BLOQUES_POR_CADA_INODO; j++) {
        if (inodo_origen_ptr->bloques[j] != 0xFFFF) {
            int bloque_libre = -1;
            for (int k = 0; k < MAX_BLOQUES; k++) {
                if (bytemaps.bytemapBloques[k] == 0) {
                    bloque_libre = k;
                    break;
                }
            }
            if (bloque_libre == -1) {
                printf("No hay bloques libres\n");
                return;
            }

            bytemaps.bytemapBloques[bloque_libre] = 1;
            inodo_destino->bloques[j] = bloque_libre;
            memcpy(particion[bloque_libre], particion[inodo_origen_ptr->bloques[j]], TAMAGNO_BLOQUE);
        } else {
            inodo_destino->bloques[j] = 0xFFFF;
        }
    }

    bytemaps.bytemapInodos[inodo_libre] = 1;
    strcpy(directorio[entrada_libre].archivo, destino);
    directorio[entrada_libre].inodo = inodo_libre;

    printf("Fichero copiado\n");
}

void comando_rename(char *antiguo, char *nuevo) {
    for (int i = 0; i < 20; i++) {		// Es hasta 20 porque en la partición hay hasta 20 archivos
        if (strcmp(directorio[i].archivo, nuevo) == 0) {
            printf("Error: El nombre destino ya existe\n");
            return;
        }
    }

    for (int i = 0; i < 20; i++) {
        if (strcmp(directorio[i].archivo, antiguo) == 0) {
            strcpy(directorio[i].archivo, nuevo);
            printf("Fichero renombrado\n");
            return;
        }
    }

    printf("Error: El fichero origen no existe\n");
}