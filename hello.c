#include <linux/init.h>
#include <linux/module.h>
//#include <linux/kernel.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Rubén y Jaime");
MODULE_DESCRIPTION("Curro final Sistemas");
MODULE_VERSION("1.0");

static int __init hello_init(void) {
    printk(KERN_INFO "Buenos días\n");
    return 0;
}

static void __exit hello_exit(void) {
    printk(KERN_INFO "Buenas tardes\n");
}

// Especificar las funciones de inicialización y limpieza
module_init(hello_init);
module_exit(hello_exit);
