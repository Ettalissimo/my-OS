#include <n7OS/process.h>
#include "n7OS/console.h"
#include "n7OS/process.h"
#include "n7OS/ctx_sw.h"


void idle() {
    console_putbytes(">> IDLE STARTED <<\n", 19);
    while (1) {
        //console_putbytes(".", 1);  // un point toutes les itérations
        hlt();
        ordonnanceur();

    }
}
