#include <n7OS/cpu.h>
#include <n7OS/irq.h>
#include <n7OS/console.h>
#include <inttypes.h>

#define PIT_COMMAND_PORT 0x43
#define PIT_CHANNEL0_PORT 0x40
#define PIT_FREQUENCY 1193180
#define TIMER_FREQ 1000 // 1kHz = 1ms
#define PIT_DIVISOR (PIT_FREQUENCY / TIMER_FREQ)

#define VGA_WIDTH 80
#define VGA_ATTR 0x0F  // White on black

// Compteur système en millisecondes
volatile uint32_t tick_count = 0;

// Message global pour le paging
const char tick_msg[] = "TICK\n";

void timer_handler_C() {
    tick_count++;
    
    // Mise à jour de l'affichage toutes les 100ms pour réduire la charge
    if (tick_count % 100 == 0) {
        display_time();
    }
    
    // Affichage du TICK chaque seconde
    if (tick_count % 1000 == 0) {
        //console_putbytes(tick_msg, 6);
    }


    outb(0x20, 0x20);  // EOI
}

void init_timer() {
    // Configure le PIT
    outb(0x34, PIT_COMMAND_PORT);  // Mode 2, LSB/MSB, Canal 0
    outb(PIT_DIVISOR & 0xFF, PIT_CHANNEL0_PORT);  // LSB
    outb((PIT_DIVISOR >> 8) & 0xFF, PIT_CHANNEL0_PORT);  // MSB

    // Installation du handler pour l'interruption 32
    extern void timer_handler();
    init_irq_entry(32, (uint32_t)timer_handler);

    // Active l'IRQ0 sur le PIC maître
    outb(inb(0x21) & ~1, 0x21);
}

void convert_time(uint32_t ticks, char* buf) {
    uint32_t total_seconds = ticks / 1000;  // Convert ms to seconds
    uint32_t hours = total_seconds / 3600;
    uint32_t minutes = (total_seconds % 3600) / 60;
    uint32_t seconds = total_seconds % 60;
    
    sprintf(buf, "%02u:%02u:%02u", hours, minutes, seconds);
}

void display_time() {
    char time_buf[9];  // HH:MM:SS + null terminator
    convert_time(tick_count, time_buf);
    
    // Accès direct à la mémoire vidéo
    volatile uint16_t *video = (uint16_t*)0xB8000;
    
    // Position en haut à droite (80 - 8 caractères)
    const int right_offset = VGA_WIDTH - 8;
    
    // Écriture de l'heure caractère par caractère avec un attribut plus visible
    for (int i = 0; i < 8; i++) {
        // Calcul de la position dans la mémoire vidéo
        int pos = right_offset + i;
        // Utiliser un attribut plus visible (par exemple, blanc brillant sur bleu)
        uint8_t attr = (BLUE << 4) | (WHITE);  // Fond bleu, texte blanc
        video[pos] = (time_buf[i] & 0xFF) | (attr << 8);
    }
}