#include <n7OS/cpu.h>
#include <inttypes.h>
#include <n7OS/processor_structs.h>
#include <n7OS/console.h>
#include <n7OS/paging.h>
#include <stdio.h>
#include <n7OS/kheap.h>
#include <n7OS/mem.h>  // pour init_mem, findfreePage, etc.
#include <n7OS/irq.h>

void init_timer();  // déclaration manuelle de la fonction du timer

// Déclaration du handler assembleur
extern void handler50();
extern void handler32();



void kernel_start(void)
{
    init_console();
    init_mem();

    //console_putbytes("uhxkjbascvasjxs \n", 18);
    char buf[64];
    sprintf(buf, "init_paging @ %x\n", (uint32_t)&initialise_paging);
    console_putbytes(buf, 64);

    initialise_paging();
    console_putbytes("Initialisation Paging \n", 24);

    init_timer();

    load_idt();
    __asm__("int $32");  // test
    console_putbytes("BEFORE STI\n", 12);
    sleep(3000); // délai pour tester stabilité

    extern uint64_t idt[];

    //char buf[64];
    sprintf(buf, "IDT[32]=%x %x\n", (uint32_t)(idt[32] >> 32), (uint32_t)(idt[32]));
    console_putbytes(buf, 64);

    sprintf(buf, "handler32 = %x\n", (uint32_t)&handler32);
    console_putbytes(buf, 64);

    // Assure que seul IRQ0 est autorisé (bit 0 = 0)
    outb(inb(0x21) & ~1, 0x21);  // Master PIC : active IRQ0
    outb(0xFF, 0xA1);            // Slave PIC : masque tout


    //sti(); // cette fois, tu peux l’activer 💪
    sleep(3000); // délai pour tester stabilité
    console_putbytes("AFTER STI\n", 11);

    //__asm__ volatile("int $32");  // Déclenche manuellement l’IT 32

    console_putbytes("MANUAL IRQ32\n", 14);
    __asm__ volatile("int $32");
    console_putbytes("AFTER INT32\n", 15);




    /* TEST MEMOIRE

    init_mem();
    print_mem();  // devrait indiquer 0 pages utilisées

    uint32_t p1 = findfreePage();
    uint32_t p2 = findfreePage();
    setPage(0x1000);  // marque manuellement la page 0x1000
    clearPage(p2);    // libère la deuxième page allouée

    print_mem();  // compare le nombre de pages utilisées

    */

    /*  TEST POUR CONSOLE

    console_putbytes("Hello, World!\n", 14);
    console_putbytes("Ligne 2\tcolonne 9\n", 20);
    console_putbytes("Retour\rDébut\n", 13);
    console_putbytes("Efface en 3s...\n", 17);
    
    */

    // TEST POUR PAGING
    /*

    initialise_paging();
    alloc_page_entry(0x400000, 1, 1);
    uint32_t* test = (uint32_t*) 0x400000;
    *test = 0xDEADBEEF;

    char* vga = (char*) 0xB8000;
    vga[0] = ((*test >> 28) & 0xF) + '0';  // Affiche premier chiffre (C)
    vga[1] = 0x0F;  // Couleur blanche sur fond noir

    printf("Valeur écrite : %x\n", *test);
    */


    // TEST POUR Interruptions
    /*
     // Installer handler IT 50 dans l’IDT
    

    init_irq_entry(50, (uint32_t) handler50);
    console_putbytes("bef Load\n", 10);

    load_idt();
    console_putbytes("IDT OK\n", 7);

    //sti();
    sleep(3000); // délai pour tester stabilité

    __asm__ volatile("int $50"); // déclenchement
    */


    for (int i = 0; i < 3000000; i++); // petit delay (mauvais mais simple)


    // lancement des interruptions
    //sti();
    // Masque l'interruption IRQ0 (timer) via le PIC maître (port 0x21)
    outb(inb(0x21) | 0x01, 0x21);


    // on ne doit jamais sortir de kernel_start
    while (1) {
        // cette fonction arrete le processeur
        hlt();
    }
}
