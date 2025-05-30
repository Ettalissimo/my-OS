#include <n7OS/irq.h>
#include <n7OS/processor_structs.h>
#include <inttypes.h>
#include <n7OS/cpu.h>
#include <n7OS/console.h>

volatile uint32_t system_ticks = 0;  // Definition of system_ticks
extern uint64_t idt[256] __attribute__((aligned(8)));

/**
 * @brief Initialise une entrée dans l’IDT
 * 
 * @param num_line Numéro de l’interruption (0 à 255)
 * @param addr     Adresse du handler
 */

void init_pic(void) {
    // ICW1: start initialization sequence
    outb(0x11, 0x20);  // Master PIC
    outb(0x11, 0xA0);  // Slave PIC
    
    // ICW2: interrupt vector offset
    outb(0x20, 0x21);  // Master offset = 32 (0x20)
    outb(0x28, 0xA1);  // Slave offset = 40 (0x28)
    
    // ICW3: IRQ line 2 connection between master and slave
    outb(0x04, 0x21);  // Tell Master PIC slave is at IRQ2
    outb(0x02, 0xA1);  // Tell Slave PIC its cascade identity
    
    // ICW4: 8086 mode
    outb(0x01, 0x21);
    outb(0x01, 0xA1);
    
    // OCW1: Set interrupt masks
    outb(0x0, 0x21);   // Enable all interrupts on master
    outb(0x0, 0xA1);   // Enable all interrupts on slave
}

void init_irq_entry(int num_line, uint32_t addr) {
    // Clear the entry first
    idt[num_line] = 0;
    
    uint16_t selector = 0x08;    // Kernel code segment
    uint8_t type_attr = 0x8E;    // Present (1), Ring0 (00), Int Gate (1110)
    
    // Split address into high and low parts
    uint16_t offset_low = addr & 0xFFFF;
    uint16_t offset_high = (addr >> 16) & 0xFFFF;
    
    // Construct the 64-bit IDT entry
    uint64_t entry = ((uint64_t)offset_low) |
                    ((uint64_t)selector << 16) |
                    ((uint64_t)type_attr << 40) |
                    ((uint64_t)offset_high << 48);
    
    idt[num_line] = entry;
    
    char buf[64];
    sprintf(buf, "Handler address: 0x%08x\n", addr);
    console_putbytes(buf, strlen(buf));
}


void load_idt(void) {
    struct {
        uint16_t limit;
        uint32_t base;
    } __attribute__((packed)) idtr;

    idtr.limit = sizeof(idt) - 1;
    idtr.base = (uint32_t)idt;

    __asm__ volatile("lidt %0" :: "m"(idtr));
}


void c_handler_50(void) {
    console_putbytes("Handler 50 called!\n", 20);
    // Only send EOI if this was a hardware interrupt
    // For software interrupts (like int $50), we don't need EOI
    // outb(0x20, 0x20);
}
