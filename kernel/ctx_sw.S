.text
.globl ctx_sw

# void ctx_sw(uint32_t* old_esp, uint32_t new_esp)
ctx_sw:
    # Save registers
    pushf                   # Save flags
    pusha                  # Save all registers

    # Save old ESP
    mov 40(%esp), %eax     # First argument (old_esp)
    mov %esp, (%eax)       # Save current ESP

    # Load new ESP
    mov 44(%esp), %esp     # Second argument (new_esp)

    # Restore registers for new process
    popa                   # Restore all registers
    popf                   # Restore flags
    ret                    # Return to new process

.globl hlt
hlt:
    hlt
    ret
