.text
# cette directive sert a rendre l'etiquette publique
    .globl handler_IT
# debut du traitant
handler_IT:
# sauvegarde des registres importants
    pushl %eax
    pushl %edx
    pushl %ecx
# appel a la fonction C realisant le traitant
    call handler_en_C
# restauration des registres importants
    popl %ecx
    popl %edx
    popl %eax
# fin du traitant
    iret


.text
.globl handler50
handler50:
    # Keep it simple first for testing
    pusha
    cld                    # Clear direction flag
    call c_handler_50      # Call C handler
    popa
    iret



.globl handler32
handler32:
    pushl %eax
    pushl %ecx
    pushl %edx
    call timer_handler_C    # Replace test_signal with timer_handler_C
    popl %edx
    popl %ecx
    popl %eax
    iret



.text
.globl timer_handler
timer_handler:
    pushl %eax
    pushl %ecx
    pushl %edx
    call timer_handler_C
    popl %edx
    popl %ecx
    popl %eax
    iret

.text
.globl keyboard_handler
keyboard_handler:
    pushl %eax
    pushl %ecx
    pushl %edx
    call keyboard_interrupt_handler
    popl %edx
    popl %ecx
    popl %eax
    iret

