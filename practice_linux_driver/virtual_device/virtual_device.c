/* NOTE 
 * This source comes from : https://temp123.tistory.com/16
 * 
 */
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/slab.h>

/* 
static char *buffer = NULL;
 
static int virtual_device_open(struct inode *inode, struct file *filp) {
    printk(KERN_ALERT "virtual_device open function called\n");
    return 0;
}
 
static int virtual_device_release(struct inode *inode, struct file *filp) {
    printk(KERN_ALERT "virtual device release function called\n");
    return 0;
}
 
static ssize_t virtual_device_write(struct file *filp, const char *buf, size_t count, loff_t *f_pos) {
    printk(KERN_ALERT "virtual_device write function called\n");
    strcpy(buffer, buf);
    return count;
}
 
static ssize_t virtual_device_read(struct file *filp, char *buf, size_t count, loff_t *f_pos) {
    printk(KERN_ALERT "virtual_device read function called\n");
  //  copy_to_user(buf, buffer, 1024);
    return count;
}
 
static struct file_operations vd_fops = {
    .read 		= virtual_device_read,
    .write 		= virtual_device_write,
    .open 		= virtual_device_open,
    .release 	= virtual_device_release
};*/
 
static int virtual_device_init(void) {
	
/*	int major;
	
    major = register_chrdev(0, "virtual_device", &vd_fops);
    if ( major < 0 )
        printk(KERN_ALERT "driver init failed, error=%d\n", major);
    else
        printk(KERN_ALERT "driver init successful\n");
    buffer = (char*)kmalloc(1024, GFP_KERNEL);
    if(buffer != NULL) 
        memset(buffer, 0, 1024);
  */
	printk(KERN_ALERT "driver init successful\n");
    return 0;
}
 
static void virtual_device_exit(void) {
//    unregister_chrdev(0, "virtual_device");
    printk(KERN_ALERT "driver cleanup successful\n");
//    kfree(buffer);
}
 
module_init(virtual_device_init);
module_exit(virtual_device_exit);
MODULE_LICENSE("GPL");
