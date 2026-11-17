#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h> // para nuevas funciones 
#include <linux/uaccess.h> // para usar READ

#define SUCCESS 0
#define DEVICE_NAME "chardev"
#define BUF_LEN 80 // tam del buffer

static int Major;
static int Device_Open = 0; // 0 nadie usa
static char msg[BUF_LEN];
static char *msg_Ptr;


static int device_open(struct inode *, struct file *); // abrir
static int device_release(struct inode *, struct file *); // cerrar
static ssize_t device_read(struct file *, char __user *, size_t, loff_t *); // leer
static ssize_t device_write(struct file *, const char __user *, size_t, loff_t *); //escribir


// tabla, que... se comportaria como un constructor en C
static struct file_operations fops = {
    .read = device_read,
    .write = device_write,
    .open = device_open,
    .release = device_release
};

// cargar modulo:
static int __init chardev_init(void)
{
    Major = register_chrdev(0, DEVICE_NAME, &fops);

    if (Major < 0) {
        printk(KERN_ALERT "Fallo el registro: %d\n", Major);
        return Major;
    }

    printk(KERN_INFO "Me asignaron el major %d\n", Major);
    printk(KERN_INFO "Crea el archivo con: mknod /dev/%s c %d 0\n", DEVICE_NAME, Major);
    return SUCCESS;
}



// sacar / retirar el modulo
static void __exit chardev_exit(void)
{
    unregister_chrdev(Major, DEVICE_NAME);
    printk(KERN_INFO "Dispositivo desregistrado\n");
}



// pa qeu se active el insmod y el remmod

module_init(chardev_init);
module_exit(chardev_exit);

MODULE_LICENSE("GPL");


/*----------------------------------*/

// AHORA SI LAS FUNCIONES:

// para abrir el modulo:
static int device_open(struct inode *inode, struct file *file)
{
    static int counter = 0;

    if (Device_Open)
        return -EBUSY;

    Device_Open++;
    snprintf(msg, BUF_LEN, "Ya te dije %d veces Hola mundo!\n", counter++);
    msg_Ptr = msg;
    try_module_get(THIS_MODULE);

    return SUCCESS;
}

// para retirar el modulo:
static int device_release(struct inode *inode, struct file *file)
{
    Device_Open--;
    module_put(THIS_MODULE);

    return SUCCESS;
}


// para leer:

static ssize_t device_read(struct file *filp, char __user *buffer, size_t length, loff_t *offset)
{
    int bytes_read = 0;

    if (*msg_Ptr == 0)
        return 0;

    while (length && *msg_Ptr) {
        put_user(*(msg_Ptr++), buffer++);
        length--;
        bytes_read++;
    }

    return bytes_read;
}


// Para escribrir: TAREA

static ssize_t device_write(struct file *filp, const char __user *buff, size_t len, loff_t *off)
{
    int i;
    char c;
    // 80 max, tons 80 - 1 = 79 para no pasarme y meterel \0 al final.
    for (i = 0; i < len && i < BUF_LEN - 1; i++) {
        if (get_user(c, buff + i)) // sacar y meter en c
            return -EFAULT;
        msg[i] = c;// meter en el msg lo que tenemos en c
    }

    msg[i] = '\0';

    printk(KERN_INFO "escrito: %s\n", msg);

    return len; //return a echo
}


/*
comandos:
-----------
make clean
make
sudo insmod chardev.ko
sudo dmesg | tail -3

*/

/*

El dmesg muestra el major. Puede ser 239 y otro.
-------
sudo mknod /dev/chardev c 239 0
sudo sh -c 'echo "hola ucsp" > /dev/chardev'
sudo dmesg | tail -3


*/

/*
para salir
-----

sudo rmmod chardev
sudo rm /dev/chardev

*/