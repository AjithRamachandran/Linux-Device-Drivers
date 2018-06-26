#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/usb.h>

#define VENDOR_ID 0x8564
#define PRODUCT_ID 0x1000

#define DEVICE_NAME "Aether"

static int usb_probe(struct usb_interface *intf, const struct usb_device_id *id) {
	printk(KERN_INFO "aether: pendrive plugged");
	return 0;
}

static void usb_disconnect(struct usb_interface *intf) {
	printk(KERN_INFO "aether: pendrive disconnected");
}

static struct usb_device_id usb_table[] = {
	{USB_DEVICE(VENDOR_ID, PRODUCT_ID)},
	{},
};
MODULE_DEVICE_TABLE(usb, usb_table);

static struct usb_driver usb_aether = {
	.name = DEVICE_NAME,
	.id_table = usb_table,
	.probe = usb_probe,
	.disconnect = usb_disconnect,
};

static int __init usb_init(void) {
	usb_register(&usb_aether);
	printk(KERN_INFO "aether: Successfuly registered");
	return 0;	
}	

static void __exit usb_exit(void) {
	usb_deregister(&usb_aether);
	printk(KERN_INFO "aether: Successfuly unregistered");
}

module_init(usb_init);
module_exit(usb_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ajith <ajithar204@gmail.com>");
MODULE_DESCRIPTION("A sample module");
MODULE_SUPPORTED_DEVICE("testdevice");
MODULE_VERSION("0.1");