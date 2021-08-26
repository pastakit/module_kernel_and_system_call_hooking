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
void **open_system_call_table_addr;
asmlinkage int (*open_syscall) (const char __user *filename, int flags, umode_t mode);

asmlinkage int open_hook(const char __user *filename, int flags, umode_t mode) {
    printk(KERN_INFO "ten tien trinh mo file: %s",current->comm);
    printk(KERN_INFO "ten file duoc mo: %s",filename);
    return open_syscall(filename, flags, mode);
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
    printk(KERN_INFO "open_hooking Hook loaded successfully..\n");
    open_system_call_table_addr = (void*)0xffffffff81a001c0 ; // tuy tung may co the khac nhau
    open_syscall = open_system_call_table_addr[__NR_open];
    make_rw((unsigned long)open_system_call_table_addr);
    open_system_call_table_addr[__NR_open] = open_hook;
    return 0;
}
static int __exit exit_point(void){
        printk(KERN_INFO "Unloaded open_hooking successfully\n");
    open_system_call_table_addr[__NR_open] = open_syscall;
    make_ro((unsigned long)open_system_call_table_addr);
    return 0;
}
module_init(entry_point);
module_exit(exit_point);
