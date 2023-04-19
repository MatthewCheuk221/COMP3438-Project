#include <linux/module.h>
#include <linux/init.h>
#include <linux/types.h>
#include <linux/printk.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/string.h>
#include <linux/kdev_t.h>

#define DEVICE_NAME "comp3438_exam"
#define N_D 1 
#define S_N 1 
//static char msg[] = "";
static int major;
static dev_t devno;
static struct cdev dev_exam;

static int exam_open(struct inode *inode, struct file *fp){
  printk("Device " DEVICE_NAME " open.\n");
  return 0;
}
static ssize_t exam_write(struct file *fp, const char *buf, size_t count, loff_t *position){
  char string[100];
  int ret;
  ret = copy_from_user(&string, buf, sizeof(string));
  if (ret){
    printk("Fail to copy data from the user space to the kernel space.\n");
    return -1;
  }
  printk("%s\n",string);
  return sizeof(string);
}
static int exam_release(struct inode* inode, struct file *fp){
  printk("Device " DEVICE_NAME " released.\n");
  return 0;
}
static struct file_operations exam_fops = {
  owner: THIS_MODULE,
  open: exam_open,
  write: exam_write,
  release: exam_release
};
static int __init exam_init(void){
  int ret;
  ret = alloc_chrdev_region(&devno, S_N, N_D, DEVICE_NAME);
  if (ret < 0){
    printk("Device " DEVICE_NAME " cannot get major number.\n");
    return ret;
  }
  major = MAJOR(devno);
  printk("Device " DEVICE_NAME " initialized with major number = %d.\n", major);
  cdev_init(&dev_exam, &exam_fops);
  dev_exam.owner = THIS_MODULE;
  dev_exam.ops = &exam_fops;
  ret = cdev_add(&dev_exam, devno, N_D);
  
  if (ret){
    printk("Device " DEVICE_NAME " register fail.\n");
    return ret;
  }
  printk("Device " DEVICE_NAME " loaded.\n");
  return 0;
}

static void __exit exam_exit(void){
  cdev_del(&dev_exam);
  unregister_chrdev_region(devno, N_D);
  printk("Device " DEVICE_NAME " unloaded.\n");
}


module_init(exam_init);
module_exit(exam_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Matthew Cheuk");
MODULE_DESCRIPTION("Char exam driver");
