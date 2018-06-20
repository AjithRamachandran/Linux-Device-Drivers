#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/init.h>
#include <linux/seq_file.h>
#include <linux/sysinfo.h>
#include <linux/mm.h>

#define PROC_NAME "aether"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ajith <ajithar204@gmail.com>");
MODULE_DESCRIPTION("A proc module to show the RAM info");
MODULE_VERSION("0.1");


static struct proc_dir_entry *success;

#define K(x) ((x) << (PAGE_SHIFT - 10))

static int raminfo_proc_show(struct seq_file *m, void *v) {
	static struct sysinfo info;
		si_meminfo(&info);
		unsigned long used;
		used = K(info.totalram) - K(info.freeram);
	seq_printf(m, "Total RAM : %lu kB\n", K(info.totalram));
	seq_printf(m, "Used RAM : %lu kB\n", used);
	seq_printf(m, "Free RAM : %lu kB\n", K(info.freeram));
	return 0;
}

static int raminfo_proc_open(struct inode *inode, struct file *file) {
	return single_open(file, raminfo_proc_show, NULL);
}

static struct file_operations raminfo_proc_fops = {
	.owner = THIS_MODULE,
	.open = raminfo_proc_open,
	.read = seq_read,
	.release = single_release,
};

static int __init raminfo_init(void) {
	success = proc_create(PROC_NAME, 0, NULL, &raminfo_proc_fops);
	if (!success) {
		return -ENOMEM;;
	}
	return 0;
}

static void __exit raminfo_exit(void) {
	remove_proc_entry(PROC_NAME, NULL);
}

module_init(raminfo_init);
module_exit(raminfo_exit);