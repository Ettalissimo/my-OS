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


.globl handler50

handler50:
    pushl %eax
    pushl %ecx
    pushl %edx

    call handler50_C

    popl %edx
    popl %ecx
    popl %eax
    iret



.globl handler32
handler32:
    iret




