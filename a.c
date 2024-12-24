#include <linux/init.h>
#include <linux/module.h>
//#include <linux/kernel.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Rubén y Jaime");
MODULE_DESCRIPTION("Curro final Sistemas");
MODULE_VERSION("1.0");

typedef struct {
	unsigned int cantidadDeInodos; /* inodos de la partición */
	unsigned int cantidadDeBloques; /* bloques de la partición */
	unsigned int bloquesLibres; /* bloques libres */
	unsigned int inodosLibres; /* inodos libres */
	unsigned int primerBloque; /* primer bloque de datos */
	unsigned int tamañoBloque; /* tamaño del bloque en bytes */
	unsigned char s_relleno[SIZE_BLOQUE-6*sizeof(unsigned int)]; /* relleno a
0's*/
} superbloque;

typedef struct {
	unsigned char bmap_bloques[MAX_BLOQUES_PARTICION];
	unsigned char bmap_inodos[MAX_INODOS]; /* inodos 0 y 1 reservados, inodo 2 directorio */
	unsigned char bmap_relleno[SIZE_BLOQUE-(MAX_BLOQUES_PARTICION+MAX_INODOS)*sizeof(char)];
} bytemap;

typedef struct {
	unsigned int size_fichero;
	unsigned short int i_nbloque[MAX_NUMS_BLOQUE_INODO];
} inodo;


static int __init hello_init(void) {
	printk(KERN_INFO "Buenos días\n");

	superbloque superbloque;

	return 0;
}

static void __exit hello_exit(void) {
	printk(KERN_INFO "Buenas tardes\n");
}

module_init(hello_init);
module_exit(hello_exit);
