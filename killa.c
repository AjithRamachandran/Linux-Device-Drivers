#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/device.h>

#define DEVICE_NAME "killa"
#define MAX_LENGTH 100

#define WR_VALUE _IOW('a', 1, int32_t*)
#define RD_VALUE _IOR('a', 2, int32_t*)

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ajith <ajithar204@gmail.com>");
MODULE_DESCRIPTION("A sample module");
MODULE_SUPPORTED_DEVICE("testdevice");
MODULE_VERSION("0.1");

static int major_number;
static struct class *dev_class;
static struct cdev *m_cdev;
dev_t dev_num;
int err;
int32_t value = 0;

char data[MAX_LENGTH];

static int device_open(struct inode *inode, struct file *filp) {
	printk(KERN_ALERT "you opened this device");
	return 0;
}

static int device_close(struct inode *inode, struct file *filp) {
	printk(KERN_INFO "closed device successfully");
	return 0;
}

// static ssize_t device_read(struct file *filp, char __user *buffer, size_t length, loff_t *off_t) {
// 	printk(KERN_INFO "reading from device");
// 	// return simple_read_from_buffer(buffer, length, off_t, data, MAX_LENGTH);
// 	return 0;
// }

// static ssize_t device_write(struct file *filp, const char __user *buffer, size_t length, loff_t *off_t) {
// 	printk(KERN_INFO "writing to device");
// 	// return simple_write_to_buffer(data, MAX_LENGTH, off_t, buffer, length);
// 	return 0;
// }


static long etx_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
         switch(cmd) {
                case WR_VALUE:
                        copy_from_user(&value ,(int32_t*) arg, sizeof(value));
                        printk(KERN_INFO "Value = %d\n", value);
                        break;
                case RD_VALUE:
                        copy_to_user((int32_t*) arg, &value, sizeof(value));
                        break;
        }
        return 0;
}

static struct file_operations fops = {
	.open = device_open,
	.release = device_close,
	// .read = device_read,
	// .write = device_write,
	.unlocked_ioctl = etx_ioctl,
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

	if ((dev_class = class_create(THIS_MODULE, "device_class")) == NULL) {
		printk(KERN_ALERT "cannot add class");
		goto class_err;
		}

	if (device_create(dev_class, NULL, dev_num, NULL, "killa_dev") == NULL) {
		printk(KERN_ALERT "cannot add device");
		goto dev_err;
		}
	return 0;

class_err:
	class_destroy(dev_class);

dev_err:
	unregister_chrdev(major_number, DEVICE_NAME);
	return -1;
}

static void __exit chdev_exit(void) {
	cdev_del(m_cdev);
	device_destroy(dev_class, dev_num);
	class_destroy(dev_class);
	unregister_chrdev_region(MKDEV(major_number, 0), 1);
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