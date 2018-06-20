#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/cdev.h>
#include <linux/semaphore.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "killa"
#define MAX_LENGTH 100

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ajith <ajithar204@gmail.com>");
MODULE_DESCRIPTION("A sample module");
MODULE_SUPPORTED_DEVICE("testdevice");
MODULE_VERSION("0.1");

static int major_number;
struct cdev *m_cdev;
dev_t dev_num;
int err;

char data[MAX_LENGTH];

static int device_open(struct inode *inode, struct file *filp) {
	printk(KERN_ALERT "you opened this device");
	return 0;
}

static int device_close(struct inode *inode, struct file *filp) {
	printk(KERN_INFO "closed device successfully");
	return 0;
}

static ssize_t device_read(struct file *filp, char __user *buffer, size_t length, loff_t *off_t) {
	printk(KERN_INFO "reading from device");
	return simple_read_from_buffer(buffer, length, off_t, data, MAX_LENGTH);
}

static ssize_t device_write(struct file *filp, const char __user *buffer, size_t length, loff_t *off_t) {
	printk(KERN_INFO "writing to device");
	return simple_write_to_buffer(data, MAX_LENGTH, off_t, buffer, length);
}

static struct file_operations fops = {
	.open = device_open,
	.release = device_close,
	.read = device_read,
	.write = device_write,
	.owner = THIS_MODULE,
};


static int __init chdev_init(void) {

	err = alloc_chrdev_region(&dev_num, 0, 1, DEVICE_NAME);
	if (err) {
		printk(KERN_ALERT "cannot add device");
		return err;
	}

	major_number = MAJOR(dev_num);
	printk(KERN_INFO "Major Number Allocated = %d", major_number);
	
	m_cdev = cdev_alloc();
	m_cdev->ops = &fops;
	m_cdev->owner = THIS_MODULE;

	err = cdev_add(m_cdev, dev_num, 1);
	if (err) {
		printk(KERN_ALERT "cannot add device");
		return err;
	}

	return 0;
}

static void __exit chdev_exit(void) {
	cdev_del(m_cdev);
	
	unregister_chrdev(major_number, DEVICE_NAME);
	printk(KERN_INFO "successfully exited from the device");
}

module_init(chdev_init);
module_exit(chdev_exit);

/* 
ssize_t simple_read_from_buffer(void __user *to, size_t count, loff_t *ppos,
				const void *from, size_t available)

ssize_t simple_write_to_buffer(void *to, size_t available, loff_t *ppos,
		const void __user *from, size_t count)
*/