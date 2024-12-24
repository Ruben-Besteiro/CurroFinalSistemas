#include <stdio.h>
#include <string.h>

typedef struct {
	int cantidadDeInodos;
	int cantidadDeBloques;
	int bloquesLibres;
	int inodosLibres;
	int primerBloque;
	int tamañoDeBloque;
	char relleno[512 - 6 * sizeof(unsigned int)];
} _superbloque;

typedef struct {
	char bytemapBloques[100];		// 100 porque el bytemap de bloques tiene 100 elementos
	char bytemapInodos[24];		// 24 porque el bytemap de inodos tiene 24 elementos
	char bytemapRelleno[512 - (100 + 24) * sizeof(char)];	// 512 es el tamaño del bloque
} _bytemaps;

typedef struct {
	int tamañoFichero;
	int bloques[7];		// El inodo guarda en qué bloques está guardado su archivo y puede haber hasta 7
} inodo;

typedef struct {
  char nombre;		// El nombre de la carpeta es '.'
  int inodo;		// El inodo de la carpeta es 2
} _entradaCarpeta;


void info();
void listaBytemaps();

static _superbloque superbloque;
static _bytemaps bytemaps;
static inodo lista_inodos[24];
static _entradaCarpeta entradaCarpeta;


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

	b = fread(&bytemaps, sizeof(bytemaps), 1, archivo);
	if (b != 1) {
		printf("Error\n");
		fclose(archivo);
	}

	c = fread(&lista_inodos, sizeof(lista_inodos), 1, archivo);
	if (c != 1) {
		printf("Error\n");
		fclose(archivo);
	}

	d = fread(&entradaCarpeta, sizeof(entradaCarpeta), 1, archivo);
	if (d != 1) {
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
		} else if (!strcmp(comando, "salir")) {
			printf("Has elegido salir");
			fclose(archivo);
			return;
		} else {
			printf("Comando no valido\n");
		}
	}
}

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
		printf("%d ", bytemaps.bytemapBloques[i]);
	}
	printf("\nBytemap de inodos:\n");
	for (int i = 0; i < 24; i++) {
		printf("%d ", bytemaps.bytemapInodos[i]);
	}
	printf("\n");
}
