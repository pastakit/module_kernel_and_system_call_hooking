#include <asm/unistd.h>
#include <asm/cacheflush.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <asm/pgtable_types.h>
#include <linux/highmem.h>
#include <linux/fs.h>
#include <linux/sched.h>
#include <linux/moduleparam.h>
#include <linux/unistd.h>
#include <asm/cacheflush.h>
MODULE_LICENSE("GPL");
MODULE_AUTHOR("1612163_1712513_1712529");
void **system_call_table_addr;
asmlinkage int (*write_syscall) (unsigned int fd, const char __user *buf, size_t count);
asmlinkage int write_hook(unsigned int fd, const char __user *buf, size_t count) {
    printk(KERN_INFO "Tien trinh da ghi file: %s  ",current->comm);
    printk(KERN_INFO "So byte duoc ghi: %d",count);
    return write_syscall(fd, buf, count);
}
int make_rw(unsigned long address){
    unsigned int level;
    pte_t *pte = lookup_address(address, &level);
    if(pte->pte &~_PAGE_RW){
        pte->pte |=_PAGE_RW;
    }
    return 0;
}
int make_ro(unsigned long address){
    unsigned int level;
    pte_t *pte = lookup_address(address, &level);
    pte->pte = pte->pte &~_PAGE_RW;
    return 0;
}
static int __init entry_point(void){
    printk(KERN_INFO "write_hooking loaded successfully..\n");
    system_call_table_addr = (void*)0xffffffff81a001c0;
    write_syscall = system_call_table_addr[__NR_write];
    make_rw((unsigned long)system_call_table_addr);
    system_call_table_addr[__NR_write] = write_hook;
    return 0;
}
static int __exit exit_point(void){
        printk(KERN_INFO "Unloaded write_hooking successfully\n");
    system_call_table_addr[__NR_write] = write_syscall;
    make_ro((unsigned long)system_call_table_addr);
    return 0;
}
module_init(entry_point);
module_exit(exit_point);
