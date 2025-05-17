#include <n7OS/console.h>
#include <n7OS/cpu.h> // pour outb()

uint16_t *scr_tab;
static int cursor_x = 0, cursor_y = 0;

void init_console() {
    scr_tab= (uint16_t *) SCREEN_ADDR;
}

void console_putchar(const char c) {
    //if (c == 'a')  scr_tab[0]= CHAR_COLOR<<8|c;

    if (c>31 && c<127) {
        scr_tab[cursor_y * VGA_WIDTH + cursor_x] = (CHAR_COLOR << 8) | c;
        cursor_x++;
    }else if (c == '\n'){
        cursor_x = 0;
        cursor_y++;
    }else if (c == '\t'){
        cursor_x = (cursor_x + 8) & ~(8 - 1);
        if (cursor_x >= VGA_WIDTH) {
                cursor_x = 0;
                cursor_y++;
        }
    }else if (c == '\b'){
        if (cursor_x > 0) {
            cursor_x--;
        } else if (cursor_y > 0) {
            cursor_y--;
            cursor_x = VGA_WIDTH - 1;
        }
        scr_tab[cursor_y * VGA_WIDTH + cursor_x] = (CHAR_COLOR << 8) | ' ';
    }else if (c == '\r'){
        cursor_x = 0;
    }else if (c == '\f'){
        for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
            scr_tab[i] = (CHAR_COLOR << 8) | ' ';
        }
        cursor_x = 0;
        cursor_y = 0;
        console_set_cursor(cursor_y, cursor_x);
    }

    if (cursor_x >= VGA_WIDTH) {  // Passage automatique à la ligne
        cursor_x = 0;
        cursor_y++;
    }

    if (cursor_y >= VGA_HEIGHT) {  // Gestion du scrolling
        console_scroll();
    }

    console_cursor(cursor_y, cursor_x);
}

void console_putbytes(const char *s, int len) {
    for (int i= 0; i<len; i++) {
        console_putchar(s[i]);
    }
}


/** Déplace le curseur à la position donnée */
void console_cursor (int ligne, int colonne ){
    uint16_t pos = ligne * VGA_WIDTH + colonne ;

    outb(0x0F, 0x3D4);  // Commande pour poids faible
    outb((uint8_t)(pos & 0xFF), 0x3D5);

    outb(0x0E, 0x3D4);  // Commande pour poids fort
    outb((uint8_t)((pos >> 8) & 0xFF), 0x3D5);
}

/** Fait défiler l'écran d'une ligne vers le haut */
void console_scroll() {
    for (int i = 0; i < (VGA_HEIGHT - 1) * VGA_WIDTH; i++) {
        scr_tab[i] = scr_tab[i + VGA_WIDTH];
    }
    // Efface la dernière ligne
    for (int i = (VGA_HEIGHT - 1) * VGA_WIDTH; i < VGA_HEIGHT * VGA_WIDTH; i++) {
        scr_tab[i] = (CHAR_COLOR << 8) | ' ';
    }
    cursor_y--;
}