#include <n7OS/console.h>
#include <n7OS/cpu.h> // pour outb()


uint16_t *scr_tab;
static int cursor_pos = 0;

void init_console() {
    scr_tab= (uint16_t *) SCREEN_ADDR;
}


void console_putchar(const char c) {
    switch (c) {
        case '\n':
            cursor_pos += 80 - (cursor_pos % 80);
            break;
        case '\b':
            if (cursor_pos > 0) {
                cursor_pos--;
                scr_tab[cursor_pos] = ' ' | (CHAR_COLOR << 8);
            }
            break;
        case '\t':
            cursor_pos += 8 - (cursor_pos % 8);
            break;
        case '\f':  // efface écran
            for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
                scr_tab[i] = ' ' | (CHAR_COLOR << 8);
            }
            cursor_pos = 0;
            break;
        case '\r':
            cursor_pos -= cursor_pos % 80;
            break;
        default:
            if (c >= 32 && c <= 127) {
                scr_tab[cursor_pos] = c | (CHAR_COLOR << 8);
                cursor_pos++;
            }
            break;
    }

    // Saut de ligne automatique
    if (cursor_pos >= VGA_WIDTH * VGA_HEIGHT) {
        cursor_pos = 0;  // tu peux implémenter un scroll ici si tu veux plus tard
    }

    console_set_cursor(cursor_pos);
}


void console_putbytes(const char *s, int len) {
    for (int i= 0; i<len; i++) {
        console_putchar(s[i]);
    }
}

void console_set_cursor(int pos) {
    outb(0x0F, 0x3D4);
    outb(pos & 0xFF, 0x3D5);
    outb(0x0E, 0x3D4);
    outb((pos >> 8) & 0xFF, 0x3D5);
}

void console_get_cursor(int* lig, int* col) {
    // Calculate row and column from current cursor position
    *lig = cursor_pos / VGA_WIDTH;
    *col = cursor_pos % VGA_WIDTH;
}

void print_hex(uint32_t val, int size) {
    static const char *hex_chars = "0123456789ABCDEF";
    char buf[9];
    buf[8] = '\0';

    for (int i = 7; i >= 0; i--) {
        buf[i] = hex_chars[val & 0xF];
        val >>= 4;
    }

    console_putbytes(buf + (8 - size), size);
}

// Add this new function for x,y cursor positioning
void console_set_cursor_xy(int lig, int col) {
    int pos = lig * VGA_WIDTH + col;
    console_set_cursor(pos);
}

