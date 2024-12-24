// Hecho por Gepeto

#include <linux/init.h>
#include <linux/module.h>

// Constantes del sistema
#define SIZE_BLOQUE 512
#define MAX_BLOQUES_PARTICION 100
#define MAX_INODOS 24
#define LEN_NFICH 32
#define MAX_NUMS_BLOQUE_INODO 7

// Estructura del superbloque
typedef struct {
    unsigned int s_inodes_count;
    unsigned int s_blocks_count;
    unsigned int s_free_blocks_count;
    unsigned int s_free_inodes_count;
    unsigned int s_first_data_block;
    unsigned int s_block_size;
    unsigned char s_relleno[SIZE_BLOQUE - 6 * sizeof(unsigned int)];
} EXT_SIMPLE_SUPERBLOCK;

// Bytemaps
typedef struct {
    unsigned char bmap_bloques[MAX_BLOQUES_PARTICION];
    unsigned char bmap_inodos[MAX_INODOS];
    unsigned char bmap_relleno[SIZE_BLOQUE - (MAX_BLOQUES_PARTICION + MAX_INODOS) * sizeof(char)];
} EXT_BYTE_MAPS;

// Inodo simplificado
typedef struct {
    unsigned int size_fichero;
    unsigned short int i_nbloque[MAX_NUMS_BLOQUE_INODO];
} EXT_SIMPLE_INODE;

// Entrada de directorio
typedef struct {
    char dir_nfich[LEN_NFICH];
    unsigned short int dir_inodo;
} EXT_ENTRADA_DIR;

// Variables globales
static EXT_SIMPLE_SUPERBLOCK superbloque;
static EXT_BYTE_MAPS bytemaps;
static EXT_SIMPLE_INODE lista_inodos[MAX_INODOS];
static EXT_ENTRADA_DIR directorio[20];

// Funciones para comandos
static void comando_info(void) {
    printk(KERN_INFO "Superbloque:\n");
    printk(KERN_INFO "Inodos totales: %u\n", superbloque.s_inodes_count);
    printk(KERN_INFO "Bloques totales: %u\n", superbloque.s_blocks_count);
    printk(KERN_INFO "Bloques libres: %u\n", superbloque.s_free_blocks_count);
    printk(KERN_INFO "Inodos libres: %u\n", superbloque.s_free_inodes_count);
    printk(KERN_INFO "Primer bloque de datos: %u\n", superbloque.s_first_data_block);
    printk(KERN_INFO "Tamaño de bloque: %u bytes\n", superbloque.s_block_size);
}

static void comando_bytemaps(void) {
    int i;
    printk(KERN_INFO "Bytemap de bloques (primeros 25 elementos):\n");
    for (i = 0; i < 25; i++) {
        printk(KERN_INFO "%d ", bytemaps.bmap_bloques[i]);
    }
    printk(KERN_INFO "\nBytemap de inodos:\n");
    for (i = 0; i < MAX_INODOS; i++) {
        printk(KERN_INFO "%d ", bytemaps.bmap_inodos[i]);
    }
    printk(KERN_INFO "\n");
}

// Inicialización del módulo
static int __init simul_ext_init(void) {
    printk(KERN_INFO "Iniciando simulador de sistema de ficheros EXT simplificado\n");

    // Inicializar estructuras con datos simulados
    superbloque.s_inodes_count = MAX_INODOS;
    superbloque.s_blocks_count = MAX_BLOQUES_PARTICION;
    superbloque.s_free_blocks_count = 96;
    superbloque.s_free_inodes_count = 21;
    superbloque.s_first_data_block = 5;
    superbloque.s_block_size = SIZE_BLOQUE;

    // Simular datos de bytemaps    memset(bytemaps.bmap_bloques, 0, MAX_BLOQUES_PARTICION);
    memset(bytemaps.bmap_inodos, 0, MAX_INODOS);

    // Marcar bloques e inodos reservados
    bytemaps.bmap_bloques[0] = 1;
    bytemaps.bmap_bloques[1] = 1;
    bytemaps.bmap_bloques[2] = 1;
    bytemaps.bmap_bloques[3] = 1;

    bytemaps.bmap_inodos[0] = 1;
    bytemaps.bmap_inodos[1] = 1;
    bytemaps.bmap_inodos[2] = 1;

    comando_info();
    comando_bytemaps();

    return 0;
}

// Finalización del módulo
static void __exit simul_ext_exit(void) {
    printk(KERN_INFO "Saliendo del simulador de sistema de ficheros EXT simplificado\n");
}

module_init(simul_ext_init);
module_exit(simul_ext_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Tu Nombre");
MODULE_DESCRIPTION("Curro final sistemas");
