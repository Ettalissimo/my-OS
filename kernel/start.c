#include <n7OS/cpu.h>
#include <inttypes.h>
#include <n7OS/processor_structs.h>
#include <n7OS/console.h>
#include <n7OS/paging.h>
#include <stdio.h>
#include <n7OS/kheap.h>
#include <n7OS/mem.h>  // pour init_mem, findfreePage, etc.
#include <n7OS/irq.h>
#include <unistd.h>  // Add this at the top with other includes
#include <n7OS/process.h>

void init_timer();  // déclaration manuelle de la fonction du timer
void init_pic();  // déclaration de la fonction

void process1(void) {
    int count = 0;
    while(1) {
        printf("Process 1: %d\n", count++);
        for(volatile int i = 0; i < 100000; i++); // Shorter delay
    }
}

void process2(void) {
    int count = 0;
    while(1) {
        printf("Process 2: %d\n", count++);
        for(volatile int i = 0; i < 100000; i++); // Shorter delay
    }
}

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
    sti();*/

    // console_putbytes("Interrupts enabled\n", 19);
    

    //-------------------------------------- Pour Tester SysCall ------------------------

    /*
    initialise_paging();
    console_putbytes("Paging initialized\n", 18);
    
    // Load IDT
    load_idt();
    console_putbytes("IDT loaded\n", 11);

    // Initialize IDT and PIC
    init_pic();
    // Mask all interrupts except syscall (INT 0x80)
    outb(0xFF, 0x21);  // Mask all interrupts on master PIC
    outb(0xFF, 0xA1);  // Mask all interrupts on slave PIC
    console_putbytes("PIC initialized with interrupts masked\n", 37);
   
 // Initialize syscall handler
    extern void handler_syscall(void);
    init_irq_entry(0x80, (uint32_t)handler_syscall);
    init_syscall();
    console_putbytes("Syscalls initialized\n", 22);
    

    sleep(1000);  // Wait for IDT to be set up
    // Enable interrupts
    //cli();  // Make sure interrupts are off
    sti();
    console_putbytes("Interrupts enabled, testing syscalls...\n", 41);
    
    // Add a delay and console output to track execution
    sleep(1000);
    console_putbytes("Testing syscalls in 2 seconds...\n", 34);
    sleep(2000);
    
    // Test example syscall with more debug output
    console_putbytes("Calling example syscall...\n", 28);
    int result = example();
    console_putbytes("Example syscall returned\n", 26);
    
    if (result == 1) {
        console_putbytes("Example syscall successful\n", 28);
    } else {
        console_putbytes("Example syscall failed!\n", 25);
    }

    // Test write syscall
    console_putbytes("Testing write syscall...\n", 24);
    
    // Test using printf (should now use write syscall)
    printf("Testing printf with write syscall\n");
    
    // Test direct write syscall
    write("Direct write syscall test\n", 25);
    
    // Test with different lengths
    write("Short\n", 6);
    write("This is a longer test string to verify buffer handling\n", 52);
    
    // Test return value
    int write_result = write("Test", 4);
    printf("\nWrite returned: %d (expected 4)\n", write_result);
    
    
    // Test shutdown with proper error handling
    console_putbytes("Testing shutdown syscall...\n", 29);
    console_putbytes("Calling shutdown(1)...\n", 24);

    sleep(3000);  // Wait before shutdown
    shutdown(1);
    console_putbytes("Shutdown syscall called\n", 25);

    
    // If we get here, shutdown failed
    console_putbytes("Shutdown failed!\n", 16);  
    
    
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

 //   */


    //-------------------------------------- Pour Tester Gestion des processus ------------------------
    
    /*
    initialise_paging();
    console_putbytes("Paging initialized\n", 18);
    

    sleep(3000);  // Wait for paging to be set up


    init_mem();    console_putbytes("Memory initialized\n", 19);

    init_pic();  // Initialize PIC
    console_putbytes("PIC initialized\n", 16);

    // Initialize timer for scheduling
    init_timer();
    console_putbytes("Timer initialized\n", 18);

    load_idt(); // Load IDT
    console_putbytes("IDT loaded\n", 11);

        // --- Process Management Code ---
    init_process_management();
    console_putbytes("Process management initialized\n", 32);

    pid_t pid1 = create_process(process1);
    printf("Process 1 created with PID: %d\n", pid1);
    pid_t pid2 = create_process(process2);
    printf("Process 2 created with PID: %d\n", pid2);

    sti();  // Enable interrupts
    schedule();
    // --- End Process Management Code ---
    /*init_pic();
    console_putbytes("PIC initialized\n", 16);
    
    // Set up timer and its handler
    extern void timer_handler();
    init_irq_entry(32, (uint32_t)timer_handler);
    init_timer();
    console_putbytes("Timer initialized\n", 18);
    
    // Load IDT after setting up all handlers
    load_idt();
    console_putbytes("IDT loaded\n", 11);
    
    // Create processes
    int pid1 = create_process(process1);
    console_putbytes("Process 1 created\n", 17);
    
    int pid2 = create_process(process2);
    console_putbytes("Process 2 created\n", 17);
    
    printf("Created processes: PID1=%d, PID2=%d\n", pid1, pid2);
    
    // Make sure timer interrupt is unmasked
    outb(inb(0x21) & ~1, 0x21);  // Unmask IRQ0 (timer)
    outb(0xFF, 0xA1);            // Mask all slave PIC interrupts
    
    // Enable interrupts
    sti();
    console_putbytes("Interrupts enabled\n", 19);
    */



    for (int i = 0; i < 3000000; i++); // petit delay (mauvais mais simple)



    // on ne doit jamais sortir de kernel_start
    while (1) {
        // cette fonction arrete le processeur
        hlt();
        //console_putbytes(".", 1);  // Debug output to show we're still running

    }
}
