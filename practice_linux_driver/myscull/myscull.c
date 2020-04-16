#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/slab.h>		/* kmalloc() */
#include <linux/fs.h>
#include <linux/mutex.h>
#include <linux/cdev.h>
#include <linux/kernel.h>

#include <linux/uaccess.h>	/* copy_*_user */

#include "myscull.h"

#define KBUFF_SIZE 	256

MODULE_LICENSE("Dual BSD/GPL");

char *kbuf = NULL;

int scull_major 	= 153;				/* Sungsu : If this number is not matched, driver won't work properly */
int scull_minor 	= 0;
int scull_nr_devs 	= SCULL_NR_DEVS;	/* number of bare scull devices */
int scull_quantum 	= SCULL_QUANTUM;
int scull_qset 		= SCULL_QSET;

module_param(scull_major, int, S_IRUGO);
module_param(scull_minor, int, S_IRUGO);
module_param(scull_nr_devs, int, S_IRUGO);

struct scull_dev *scull_devices;

int 	scull_open		(struct inode *inode, struct file *filp);
int 	scull_release	(struct inode *inode, struct file *filp);
ssize_t scull_read		(struct file *filp, char __user *buf, size_t count, loff_t *f_pos);
ssize_t scull_write		(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos);
                
struct file_operations scull_fops = {
	.read =     scull_read,
	.write =    scull_write,
	.open =     scull_open,
	.release =  scull_release,
};
 
int scull_open(struct inode *inode, struct file *filp)
{
	struct scull_dev *dev; /* device information */
	int mode;

	dev = container_of(inode->i_cdev, struct scull_dev, cdev);
	filp->private_data = dev; /* for other methods */

	mode = filp->f_flags & O_ACCMODE;
	
	if ( mode == O_WRONLY) {
		printk(KERN_ALERT "myscull:mode-WDONLY\n" );	
	}
	else if ( mode == O_RDONLY ) {
		printk(KERN_ALERT "myscull:mode-RDONLY\n" );	
	}
	else
		printk(KERN_ALERT "myscull:mode-other\n" );	

	return 0;
}

int scull_release(struct inode *inode, struct file *filp)
{
	return 0;
}

ssize_t scull_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
	ssize_t retval = 0;
	printk(KERN_ALERT "myscull:scull_read, count=%d\n", count );
	
	if ( count > KBUFF_SIZE )
		count = KBUFF_SIZE;

	if ( copy_to_user(buf, kbuf, count ) )
		retval = -EFAULT;

	retval = count;	
	
	printk(KERN_ALERT "myscull:scull_read...done\n" );		
	return retval;
}

ssize_t scull_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos)
{
	ssize_t retval = 0;
	printk(KERN_ALERT "myscull:scull_write, count=%d\n", count );
	
	if ( count > KBUFF_SIZE )
		count = KBUFF_SIZE;

	if ( copy_from_user(kbuf, buf, count ) )
		retval = -EFAULT;

	retval = count;

	return retval;
}

static void scull_cleanup_module(void)
{
	int i;
	dev_t devno = MKDEV(scull_major, scull_minor);

	if (scull_devices) {
		for (i = 0; i < scull_nr_devs; i++) {
			cdev_del(&scull_devices[i].cdev);
		}
		kfree(scull_devices);
	}

	unregister_chrdev_region(devno, scull_nr_devs);

	kfree ( kbuf );
	printk(KERN_ALERT "myscull:scull_cleanup_module...done\n" );
}

static void scull_setup_cdev(struct scull_dev *dev, int index)
{
	int err, devno = MKDEV(scull_major, scull_minor + index);
    
	cdev_init(&dev->cdev, &scull_fops);

	dev->cdev.owner = THIS_MODULE;
	dev->cdev.ops 	= &scull_fops;
	err = cdev_add (&dev->cdev, devno, 1);

	if (err)
	{
		printk(KERN_ALERT "Error %d adding scull%d", err, index);
		return;
	}
	
	printk(KERN_ALERT "myscull:scull_setup_cdev, major=%d minor=%d index=%d...done", scull_major, scull_minor, index );
}

static int scull_init_module(void)
{
	int result, i;
	dev_t dev = 0;
	
	if (scull_major) {
		dev = MKDEV(scull_major, scull_minor);
		result = register_chrdev_region(dev, scull_nr_devs, "scull");
	} 
	else {
		/* Sungsu : 
		 * These major and minor numbers should already exists by using 'mknod' command for this to work.
		 */
		result = alloc_chrdev_region(&dev, scull_minor, scull_nr_devs, "scull");
		scull_major = MAJOR(dev);
	}
	
	if (result < 0) {
		printk(KERN_WARNING "scull: can't get major %d\n", scull_major);
		return result;
	}

	kbuf = kmalloc(KBUFF_SIZE, GFP_KERNEL);
	if ( !kbuf ) {
		result = -1;
		printk(KERN_ALERT "myscull:scull_init_module, kbuf=NULL!\n" );	
		goto fail;
	}
	memset(kbuf, 0, KBUFF_SIZE);

	scull_devices = kmalloc(scull_nr_devs * sizeof(struct scull_dev), GFP_KERNEL);
	if (!scull_devices) {
		result = -ENOMEM;
		goto fail; 
	}
	memset(scull_devices, 0, scull_nr_devs * sizeof(struct scull_dev));

	for (i = 0; i < scull_nr_devs; i++) {
		scull_setup_cdev(&scull_devices[i], i);
	}

	dev = MKDEV(scull_major, scull_minor + scull_nr_devs);

	printk(KERN_ALERT "myscull:scull_init_module...done\n" );
	return 0;

  fail:
	scull_cleanup_module();
	return result;	
}

module_init(scull_init_module);
module_exit(scull_cleanup_module);
