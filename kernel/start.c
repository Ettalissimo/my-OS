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
void init_pic();  // déclaration de la fonction


// Déclaration du handler assembleur
extern void handler50(void);
//volatile void* force_link = (void*) handler50;

extern void handler32();
extern volatile int tick_flag;



void kernel_start(void)
{
    init_console();
    init_mem();



    //console_putbytes("uhxkjbascvasjxs \n", 18);
    /*char buf[64];
    sprintf(buf, "init_paging @ %x\n", (uint32_t)&initialise_paging);
    console_putbytes(buf, 64);

    initialise_paging();
    console_putbytes("Initialisation Paging \n", 24);

    init_timer();

    load_idt();
    extern void timer_handler();
    init_irq_entry(32, (uint32_t)timer_handler);

    init_pic();  // ✅ remappage et démasquage du PIC
    console_putbytes("Initialisation PIC \n", 21);
    sleep(3000);
    sleep(3000);


    sti();        // active les interruptions
    sleep(3000);

    sprintf(buf, "tick_flag = %d\n", tick_flag);
    console_putbytes(buf, 32);*/

    /*__asm__("int $32");  // test
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

*/


    /* TEST MEMOIRE

    init_mem();
    print_mem();  // devrait indiquer 0 pages utilisées

    uint32_t p1 = findfreePage();
    uint32_t p2 = findfreePage();
    setPage(0x1000);  // marque manuellement la page 0x1000
    clearPage(p2);    // libère la deuxième page allouée

    print_mem();  // compare le nombre de pages utilisées

    */

    /*
    //------------------------------------- TEST POUR CONSOLE------------------------------
-
    console_putbytes("Hello, World!\n", 14);
    console_putbytes("Ligne 2\tcolonne 9\n", 20);
    console_putbytes("Retour\rDébut\n", 13);
    console_putbytes("Efface en 3s...\n", 17);
    
    */

    // --------------------------------------TEST POUR PAGING-----------------------
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


    //-------------------------- TEST POUR Interruptions ----------------------------
    /*
     // Installer handler IT 50 dans l’IDT
    
    //extern void handler_50(void);
    //init_irq_entry(50, (uint32_t) handler_50);


    initialise_paging();
    console_putbytes("Initialisation Paging \n", 24);

    // Clear and initialize IDT entry for interrupt 50
    init_irq_entry(50, (uint32_t)handler50);
    console_putbytes("Handler 50 registered\n", 21);

    // Load IDT
    load_idt();
    console_putbytes("IDT loaded\n", 11);
    
    // Initialize PIC after IDT is set up
    init_pic();
    console_putbytes("PIC initialized\n", 16);

    // Enable interrupts
    console_putbytes("Enabling interrupts...\n", 22);
    //sti();
    sleep(3000);

    console_putbytes("Interrupts enabled\n", 19);
    
    // Test interrupt after a delay
    sleep(3000);
    console_putbytes("Triggering INT 50...\n", 21);
    sleep(3000);

    __asm__ volatile("int $50");
    console_putbytes("After INT 50\n", 13);
     */

    //-------------------------------------- Pour Tester Timer ------------------------
    /*
    
    initialise_paging();
    console_putbytes("Paging initialized\n", 18);
    
    // Initialize IDT and PIC
    init_pic();
    console_putbytes("PIC initialized\n", 16);
    
    // Setup timer
    // you should decomment the tick line in order to see the tick in timer_handler_C
    init_timer();
    console_putbytes("Timer initialized\n", 18);
    
    // Load IDT
    load_idt();
    console_putbytes("IDT loaded\n", 11);
    
    // Enable interrupts
    sti();
    console_putbytes("Interrupts enabled\n", 19);
    
    */

    //-------------------------------------- Pour Tester Keyboard ------------------------

    // /*
    
    initialise_paging();
    console_putbytes("Paging initialized\n", 18);
    
    // Initialize IDT and PIC
    init_pic();
    console_putbytes("PIC initialized\n", 16);
    
    // Setup timer
    init_timer();
    console_putbytes("Timer initialized\n", 18);
    
    // Setup keyboard
    init_keyboard();
    console_putbytes("Keyboard initialized\n", 21);
    
    // Load IDT
    load_idt();
    console_putbytes("IDT loaded\n", 11);
    
    // Enable interrupts
    sti();
    console_putbytes("Interrupts enabled\n", 19);
    
    // Test keyboard input
    console_putbytes("Type some characters (press ESC to exit):\n", 41);

    char c;
    while (1) {
        c = kgetch();  // Wait for and get a character
        if (c == 27) {  // ESC key
            console_putbytes("\nESC pressed - exiting...\n", 24);
            break;
        }
        // Echo the character
        console_putchar(c);
    }

    console_putbytes("Keyboard test completed!\n", 24);

    //  */

    for (int i = 0; i < 3000000; i++); // petit delay (mauvais mais simple)



    // on ne doit jamais sortir de kernel_start
    while (1) {
        // cette fonction arrete le processeur
        hlt();
    }
}
