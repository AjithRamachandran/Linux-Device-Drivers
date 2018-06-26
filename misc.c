#include <linux/miscdevice.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/module.h>
#include <linux/uaccess.h>

#define WR_VALUE _IOW('i', 'a', int32_t*)
#define RD_VALUE _IOR('i', 'b', int32_t*)

int dev_open = 0;
int32_t value = 0;

static int misc_open(struct inode *inode, struct file *filp) {
	if(dev_open) {
		printk(KERN_ALERT "DEvice already open");
		return EBUSY;
	}
	dev_open++;
	printk(KERN_INFO "Device opened successfully");
	return 0;
}

static int misc_close(struct inode *inode, struct file *filp) {
	dev_open--;
	printk(KERN_INFO "Device closed successfully");
	return 0;
}

static long misc_ioctl(struct file *filp, unsigned int cmd, unsigned long arg) {
	switch(cmd) {
		case RD_VALUE:
			copy_to_user((int32_t*)arg, &value, sizeof(value));
			break;
		case WR_VALUE:
			copy_from_user(&value, (int32_t*)arg, sizeof(value));
			printk(KERN_INFO "Value = %d\n", value);
			break;
	}
	return 0;
}

static struct file_operations misc_fops = {
	.owner = THIS_MODULE,
	.open = misc_open,
	.release = misc_close,
	.unlocked_ioctl = misc_ioctl,
};

static struct miscdevice misc_struct = {
	MISC_DYNAMIC_MINOR,
	"Misc",
	&misc_fops,
};

static int __init misc_init(void) {
	if(misc_register(&misc_struct) < 0) {
		printk(KERN_INFO "Cannot add the misc device");	
		return -1;
	}
	return 0;

}

static void __exit misc_exit(void) {
	misc_deregister(&misc_struct);
}

module_init(misc_init);
module_exit(misc_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ajith <ajithar204@gmail.com>");
MODULE_DESCRIPTION("A sample module");
MODULE_SUPPORTED_DEVICE("testdevice");
MODULE_VERSION("0.1"