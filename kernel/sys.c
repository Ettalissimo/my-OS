#include <n7OS/sys.h>
#include <n7OS/syscall_defs.h>
#include <n7OS/console.h>
#include <n7OS/irq.h>
#include <unistd.h>
#include <n7OS/cpu.h>    // Add this for outw


extern void handler_syscall();

void init_syscall(void) {
    // Install syscall handler
    init_irq_entry(0x80, (uint32_t)handler_syscall);
    
    // Initialize syscall table
    add_syscall(NR_example, sys_example);
    add_syscall(NR_shutdown, sys_shutdown);
    add_syscall(NR_write, sys_write);
}

// code de la fonction de traitement de l'appel systeme example
int sys_example() {
  // on ne fait que retourner 1
  return 1;
}

// Simplified shutdown implementation
int sys_shutdown(int n) {
    if (n == 1) {
        // Try QEMU newer versions first
        outw(0x2000, 0x604);   // Standard QEMU shutdown
        
        // Try older versions of QEMU/Bochs
        outw(0x2000, 0xB004);  // Older QEMU versions
        outw(0x5307, 0x4004);  // APM power off
        
        // Try triple fault approach
        __asm__ volatile(
            "movl $0, %esp\n"      // Invalid stack pointer
            "movl $0, %ebp\n"      // Invalid frame pointer
            "pushl $0\n"           // Push invalid return address
            "ret\n"                // Try to return - will triple fault
        );
        
        // If triple fault fails, try halting
        __asm__ volatile(
            "cli\n"                // Disable interrupts
            "hlt\n"                // Halt CPU
        );
        
        return 0;  // Should never reach here
    }
    return -1;
}

int sys_write(const char *s, int len) {
    console_putbytes(s, len);
    return len;
}
