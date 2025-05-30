.text
# nombre total d'appels systeme
nr_syscall = 3

.extern syscall_table

.globl handler_syscall
handler_syscall:
    # First save critical registers
    pushl %eax
    pushl %ebx
    pushl %ecx
    pushl %edx
    
    # Check syscall number (using the saved eax)
    cmpl $nr_syscall-1, 12(%esp)  # Original eax is at offset 12
    ja bad_sys_call
    
    # Get syscall arguments from user stack
    movl 12(%esp), %eax    # syscall number
    movl 20(%esp), %ebx    # first argument
    
    # Make the call
    call *syscall_table(,%eax,4)
    
    # Save return value in original eax position
    movl %eax, 12(%esp)
    
    # Restore registers
    popl %edx
    popl %ecx
    popl %ebx
    popl %eax
    iret

bad_sys_call:
    # Restore registers before returning
    popl %edx
    popl %ecx
    popl %ebx
    movl $-1, (%esp)  # Replace saved eax with -1
    popl %eax
    iret