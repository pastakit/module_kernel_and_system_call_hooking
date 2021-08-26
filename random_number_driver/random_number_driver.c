#include <linux/module.h>
#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/random.h> //dung ham :get_random_bytes 
 
static dev_t first; // Global variable for the first device number 
static struct cdev c_dev; // Global variable for the character device structure
static struct class *cl; // Global variable for the device class
static unsigned short my_random_number; // luu ket qua so ngau nhien
static char temp1[6] = {0}; // temp1 cho viec tach chuoi so
static char temp2[6] = {0};  // temp2 cho viec tach chuoi so
static int i = 0; // cho viec tach chuoi so	
static int j = 0; // cho viec tach chuoi so
static int my_open(struct inode *i, struct file *f)
{
  printk(KERN_INFO "Random_Number_Driver: open()\n");
  return 0;
}
  static int my_close(struct inode *i, struct file *f)
{
  printk(KERN_INFO "Random_Number_Driver: close()\n");
  return 0;
}
  static ssize_t my_read(struct file *f, char __user *buf, size_t
  len, loff_t *off)
{
  printk(KERN_INFO "Random_Number_Driver: read()\n");
	i = 0;

    get_random_bytes(&my_random_number, sizeof(int));

    printk(KERN_INFO "Random_Number_Driver: Random number is %d\n",(my_random_number));
	// tach tung chu so
	 while (my_random_number)

        {

            temp1[i] = my_random_number % 10 + '0';

            my_random_number /= 10;
	
            i++;

        }
        temp1[i] = '\0';
	// dao nguoc lai
	j=i-1;
	while(i>=0)
	{
		temp2[j-i] = temp1[i];
		i--;
	}
	
	// copy to buf in user space
	copy_to_user(buf, temp2, sizeof(temp2));
  return 0;
}

  static struct file_operations pugs_fops =
{
  .owner = THIS_MODULE,
  .open = my_open,
  .release = my_close,
  .read = my_read,
  
};
 
static int __init ofcd_init(void) /* Constructor */
{
  printk(KERN_INFO "Random_Number_Driver: registered");
  if (alloc_chrdev_region(&first, 0, 1, "Shweta") < 0)
  {
    return -1;
  }
    if ((cl = class_create(THIS_MODULE, "chardrv")) == NULL)
  {
    unregister_chrdev_region(first, 1);
    return -1;
  }
    if (device_create(cl, NULL, first, NULL, "random_number_driver") == NULL)
  {
    class_destroy(cl);
    unregister_chrdev_region(first, 1);
    return -1;
  }
    cdev_init(&c_dev, &pugs_fops);
    if (cdev_add(&c_dev, first, 1) == -1)
  {
    device_destroy(cl, first);
    class_destroy(cl);
    unregister_chrdev_region(first, 1);
    return -1;
  }
  return 0;
}
 
static void __exit ofcd_exit(void) /* Destructor */
{
  cdev_del(&c_dev);
  device_destroy(cl, first);
  class_destroy(cl);
  unregister_chrdev_region(first, 1);
  printk(KERN_INFO "Random_Number_Driver: unregistered");
}
 
module_init(ofcd_init);
module_exit(ofcd_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("1612163_1712513_1712529");
MODULE_DESCRIPTION("Our First Character Driver");