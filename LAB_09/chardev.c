#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

#define SUCCESS 0
#define DEVICE_NAME "chardev"
#define TAM_BUFER 80

static int Major;
static int dispositivo_abierto = 0;
static char mensaje[TAM_BUFER];
static char *ptr_mensaje;

static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);
static ssize_t device_read(struct file *, char __user *, size_t, loff_t *);
static ssize_t device_write(struct file *, const char __user *, size_t, loff_t *);

/* tabla que le dice al kernel qué función llamar para cada operación */
static struct file_operations fops = {
    .read = device_read,
    .write = device_write,
    .open = device_open,
    .release = device_release
};

static int __init chardev_init(void)
{
    Major = register_chrdev(0, DEVICE_NAME, &fops);

    if (Major < 0) {
        printk(KERN_ALERT "Fallo el registro: %d\n", Major);
        return Major;
    }

    printk(KERN_INFO "Major asignado: %d\n", Major);
    printk(KERN_INFO "Crea el archivo con: mknod /dev/%s c %d 0\n", DEVICE_NAME, Major);
    return SUCCESS;
}

static void __exit chardev_exit(void)
{
    unregister_chrdev(Major, DEVICE_NAME);
    printk(KERN_INFO "Dispositivo desregistrado\n");
}

module_init(chardev_init);
module_exit(chardev_exit);

MODULE_LICENSE("GPL");

static int device_open(struct inode *inode, struct file *file)
{
    static int contador = 0;

    if (dispositivo_abierto) {
        return -EBUSY;
    }

    dispositivo_abierto++;
    snprintf(mensaje, TAM_BUFER, "Hola soy FERNANDO! y Ya te dije %d veces Hola mundo!\n", contador++);
    ptr_mensaje = mensaje;
    try_module_get(THIS_MODULE);

    return SUCCESS;
}

static int device_release(struct inode *inode, struct file *file)
{
    dispositivo_abierto--;
    module_put(THIS_MODULE);

    return SUCCESS;
}

/* kernel -> usuario: put_user copia de mensaje al buffer del usuario */
static ssize_t device_read(struct file *filp, char __user *buffer_usuario, size_t cantidad, loff_t *off)
{
    int bytes_leidos = 0;

    if (*ptr_mensaje == 0) {
        return 0;
    }

    while (cantidad && *ptr_mensaje) {
        put_user(*(ptr_mensaje++), buffer_usuario++);
        cantidad--;
        bytes_leidos++;
    }

    return bytes_leidos;
}

/* usuario -> kernel: get_user copia del buffer del usuario a mensaje */
static ssize_t device_write(struct file *filp, const char __user *buffer_usuario, size_t cantidad, loff_t *off)
{
    int i;
    char c;

    /* máximo TAM_BUFER - 1 caracteres, el último lugar es para el '\0' */
    for (i = 0; i < cantidad && i < TAM_BUFER - 1; i++) {
        if (get_user(c, buffer_usuario + i)) {
            return -EFAULT;
        }
        mensaje[i] = c;
    }

    mensaje[i] = '\0';

    printk(KERN_INFO "escrito: %s\n", mensaje);

    return cantidad;
}

/*
# Compilar y cargar
-----------------------------
make clean
make
sudo insmod chardev.ko
sudo dmesg | tail -3    # muestra el major, ej. 239



# Crear el archivo y probar (cambia 239 si tu major es otro)
-----------------------------
sudo mknod /dev/chardev c 239 0
cat /dev/chardev
sudo sh -c 'echo "hola ucsp" > /dev/chardev'
sudo dmesg | tail -3


# Salir
-----------------------------
sudo rmmod chardev
sudo rm /dev/chardev
*/