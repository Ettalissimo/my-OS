#include <n7OS/cpu.h>
#include <n7OS/irq.h>
#include <n7OS/console.h>
#include <inttypes.h>

#define PIT_COMMAND_PORT 0x43
#define PIT_CHANNEL0_PORT 0x40
#define PIT_FREQUENCY 1193180
#define TIMER_FREQ 1000 // 1kHz = 1ms
#define PIT_DIVISOR (PIT_FREQUENCY / TIMER_FREQ)

// Compteur système en millisecondes
volatile uint32_t tick_count = 0;

// ✅ GLOBAL pour le paging
const char tick_msg[] = "TICK\n";

void timer_handler_C() {
    tick_count++;
    outb(0x20, 0x20);

    if (tick_count % 1000 == 0) {
        console_putbytes(tick_msg, 6);  // ✅ n'accède plus à un littéral .rodata inconnu
    }
}

void test_signal() {
    uint16_t *vga = (uint16_t*) 0xB8000;
    vga[0] = 'X' | (0x4F << 8);  // Rouge clair
}

// Fonction d’initialisation du timer
void init_timer() {
    #define PIT_COMMAND_PORT 0x43
    #define PIT_CHANNEL0_PORT 0x40
    #define PIT_FREQUENCY 1193180
    #define TIMER_FREQ 1000
    #define PIT_DIVISOR (PIT_FREQUENCY / TIMER_FREQ)

    // Configure le PIT
    outb(0x34, PIT_COMMAND_PORT);
    outb(PIT_DIVISOR & 0xFF, PIT_CHANNEL0_PORT);
    outb((PIT_DIVISOR >> 8) & 0xFF, PIT_CHANNEL0_PORT);

    // Dé-masque IRQ0
    outb(inb(0x21) & ~1, 0x21);

    extern void handler32();
    init_irq_entry(32, (uint32_t)handler32);
}

