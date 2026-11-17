#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Fernando");
MODULE_DESCRIPTION("Holi!");


static int __init hello(void){

    printk(KERN_INFO "Holi booli crayoli\n");

    return 0;
}


static void __exit hello_cleanup(void){

    printk(KERN_INFO "Limpiando el modulo\n");

}


module_init(hello);

module_exit(hello_cleanup);