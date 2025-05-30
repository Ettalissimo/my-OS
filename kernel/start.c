#include <n7OS/cpu.h>
#include <inttypes.h>
#include <n7OS/processor_structs.h>
#include <n7OS/console.h>
#include <n7OS/paging.h>
#include <stdio.h>
#include <n7OS/kheap.h>
#include <n7OS/mem.h>  
#include <n7OS/irq.h>
#include <unistd.h>  
#include <n7OS/process.h>
#include <stddef.h>



extern void processus1();
extern void idle();

extern processus_t processus[MAX_PROCESSUS];
extern int pid_courant;



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

    
    //------------------------------------- TEST POUR CONSOLE------------------------------
    /*

    console_putbytes("Hello, World!\n", 14);
    console_putbytes("Ligne 2\tcolonne 9\n", 20);
    console_putbytes("Retour\rDébut\n", 15);
    console_putbytes("Efface en 3s...\n", 17);
    
    */

    // --------------------------------------TEST POUR PAGING-----------------------
    /*

    initialise_paging();

    console_putbytes("Paging initialized\n", 19);
    sleep(3000);  // Attendre que la console soit prête

    // ✅ Test simple : écrire à une adresse mappée (ex: 4 Mo)
    int *ptr = (int*)0x400000;  // Adresse virtuelle 4 Mo
    *ptr = 12345;               // Écriture
    if (*ptr == 12345) {
        console_putbytes("Paging test OK\n", 16);
    } else {
        console_putbytes("Paging test FAILED\n", 20);
    }

    */


    //-------------------------- TEST POUR Interruptions ----------------------------
    /*

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
    console_putbytes("Paging initialized\n", 20);
    
    // Initialize IDT and PIC
    init_pic();
    console_putbytes("PIC initialized\n", 17);
    
    // Setup timer
    // you should decomment the tick line in order to see the tick in timer_handler_C
    init_timer();
    console_putbytes("Timer initialized\n", 19);
    
    // Load IDT
    load_idt();
    console_putbytes("IDT loaded\n", 12);
    
    // Enable interrupts
    sti();
    console_putbytes("Interrupts enabled\n", 19);

    */

    

    //-------------------------------------- Pour Tester SysCall ------------------------

    /*
    initialise_paging();
    console_putbytes("Paging initialized\n", 18);
    
    load_idt();
    console_putbytes("IDT loaded\n", 11);

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

    /*
    
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
    console_putbytes("Type some characters (press ESC to exit):\n", 43);

    char c;
    while (!is_escape_pressed()) {
        hlt();  // Wait for next interrupt
    }

    console_putbytes("\nKeyboard test completed!\n", 26);

    */


    //-------------------------------------- Pour Tester Gestion des processus ------------------------
    
    /*
    initialise_paging();
    console_putbytes("Paging initialized\n", 18);

    init_mem();
    console_putbytes("Memory initialized\n", 19);

    init_pic();
    console_putbytes("PIC initialized\n", 16);

    init_timer();
    console_putbytes("Timer initialized\n", 18);

    init_keyboard();
    console_putbytes("Keyboard initialized\n", 21);

    load_idt();
    console_putbytes("IDT loaded\n", 11);

    // Initialisation de la gestion des processus
    init_processus();
    console_putbytes("Process management initialized\n", 32);

    int pid1 = creer_processus("processus1", processus1);
    if (pid1 >= 0) {
        console_putbytes("Processus1 created\n", 20);
    } else {
        console_putbytes("Failed to create processus1\n", 29);
    }

    int pid2 = creer_processus("idle", idle);
    if (pid2 >= 0) {
        console_putbytes("Idle process created\n", 21);
    } else {
        console_putbytes("Failed to create idle process\n", 30);
    }

    sti();
    console_putbytes("Interrupts enabled\n", 19);

    // Lancement du premier processus
    ordonnanceur();

    */



    for (int i = 0; i < 3000000; i++); // petit delay 



    // on ne doit jamais sortir de kernel_start
    while (1) {
        hlt();
    }
}
