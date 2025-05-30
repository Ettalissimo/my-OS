#include <n7OS/keyboard.h>
#include <n7OS/irq.h>
#include <n7OS/cpu.h>
#include <n7OS/console.h>

#define BUFFER_SIZE 256

static char circular_buffer[BUFFER_SIZE];
static int buffer_read = 0;
static int buffer_write = 0;

static int escape_pressed = 0; 


// Table de conversion scancode -> ASCII (layout QWERTY)
static char kbdmap[] = {
    0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
    0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0,
    '*', 0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    '-', 0, 0, 0, '+', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

void keyboard_interrupt_handler() {
    uint8_t scancode = inb(0x60);
    
    if (!(scancode & 0x80)) {
        char c = kbdmap[scancode];
        if (c) {
            if (c == 27) {  // ESC key
                escape_pressed = 1;
            } else  if (!escape_pressed) {
                // Add to circular buffer
                int next_write = (buffer_write + 1) % BUFFER_SIZE;
                if (next_write != buffer_read) {
                    circular_buffer[buffer_write] = c;
                    buffer_write = next_write;
                }
                console_putchar(c);  // Echo character
            }
        }
    }
    outb(0x20, 0x20);  // EOI
}

int is_escape_pressed(void) {
    return escape_pressed;
}

void init_keyboard() {
    // Installation du handler sur IRQ1 (INT 33)
    extern void keyboard_handler();
    init_irq_entry(33, (uint32_t)keyboard_handler);
    
    // Démasquage de l'IRQ1 sur le PIC
    outb(inb(0x21) & ~(1 << 1), 0x21);
}

char kgetch(void) {
    while (buffer_read == buffer_write) {
        // Wait for character
        hlt();
    }
    char c = circular_buffer[buffer_read];
    buffer_read = (buffer_read + 1) % BUFFER_SIZE;
    return c;
}