#include <n7OS/irq.h>
#include <n7OS/cpu.h>
#include <stdio.h>
#include <n7OS/console.h>

extern void handler_IT();


void init_irq() {

}

void handler_en_C() {

}

void handler50_C() {
    console_putbytes("Interruption 50 recue !\n", 25);
}

