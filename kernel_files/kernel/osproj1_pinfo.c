#include <linux/linkage.h>
#include <linux/kernel.h>
#include <linux/timer.h>

asmlinkage void sys_osproj1_pinfo(int pid, long t1, long t2, long t3, long t4) {
	printk(KERN_INFO "[Project1] %d %ld.%09ld %ld.%09ld\n", pid, t1, t2, t3, t4);
}

