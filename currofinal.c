#include <stdio.h>
#include <string.h>

#define SIZE_BLOQUE 512
#define MAX_BLOQUES_PARTICION 100
#define MAX_INODOS 24
#define LEN_NFICH 32
#define MAX_NUMS_BLOQUE_INODO 7

typedef struct {
	int cantidadDeInodos;
	int cantidadDeBloques;
	int bloquesLibres;
	int inodosLibres;
	int primerBloque;
	int tamañoDeBloque;
	char s_relleno[SIZE_BLOQUE - 6 * sizeof(unsigned int)];
} _superbloque;

typedef struct {
	char bmap_bloques[100];		// 100 porque el bytemap de bloques tiene 100 elementos
	char bmap_inodos[24];		// 24 porque el bytemap de inodos tiene 24 elementos
	char bmap_relleno[512 - (MAX_BLOQUES_PARTICION + MAX_INODOS) * sizeof(char)];
} _bytemaps;

typedef struct {
	int size_fichero;
	short int i_nbloque[7];
} inodo;

typedef struct {
	char dir_nfich[LEN_NFICH];
	short int dir_inodo;
} _carpeta;

static _superbloque superbloque;
static _bytemaps bytemaps;
static inodo lista_inodos[24];
static _carpeta carpeta[20];



void info() {		// Si este programa fuese a nivel de kernel, todas las funciones tendrían que ser static
	printf("INFORMACION DEL SUPERBLOQUE\n");
	printf("Inodos totales: %d\n", superbloque.cantidadDeInodos);
	printf("Bloques totales: %d\n", superbloque.cantidadDeBloques);
	printf("Bloques libres: %d\n", superbloque.bloquesLibres);
	printf("Inodos libres: %d\n", superbloque.inodosLibres);
	printf("Primer bloque no reservado: %d\n", superbloque.primerBloque);
	printf("Tamano de bloque (bytes): %d\n", superbloque.tamañoDeBloque);
}

void listaBytemaps() {
	printf("BYTEMAPS (0 SIGNIFICA LIBRE Y 1 SIGNIFICA OCUPADO)\n");
	printf("Bytemap de bloques:\n");
	for (int i = 0; i < 100; i++) {
		printf("%d ", bytemaps.bmap_bloques[i]);
	}
	printf("\nBytemap de inodos:\n");
	for (int i = 0; i < MAX_INODOS; i++) {
		printf("%d ", bytemaps.bmap_inodos[i]);
	}
	printf("\n");
}

void main() {			// Si este programa estuviese a nivel de kernel, el main tendría que ir al final sí o sí, aunque como no es el caso pues da un poco igual
	FILE *archivo;
	int a;
	char *comando;
	char _comando;
	comando = &_comando;
	
	
	archivo = fopen("particion.bin", "rb");
	if (!archivo) {
		printf("El archivo no existe\n");
	}

	a = fread(&superbloque, sizeof(superbloque), 1, archivo);
	if (a != 1) {
		printf("Error\n");
		fclose(archivo);
	}

	a = fread(&bytemaps, sizeof(bytemaps), 1, archivo);
	if (a != 1) {
		printf("Error\n");
		fclose(archivo);
	}

	a = fread(lista_inodos, sizeof(lista_inodos), 1, archivo);
	if (a != 1) {
		printf("Error\n");
		fclose(archivo);
	}

	a = fread(carpeta, sizeof(carpeta), 1, archivo);
	if (a != 1) {
		printf("Error\n");
		fclose(archivo);
	}


	printf("Buenos dias\n");
	
	while (1) {
		printf("Elige una opcion: ");
		scanf("%s", comando);
		while (getchar() != '\n');
		if (!strcmp(comando, "info")) {
			info();
		} else if (!strcmp(comando, "bytemaps")) {
			listaBytemaps();
		} else {
			printf("Comando no valido\n");
		}
	}
	
	fclose(archivo);
}
