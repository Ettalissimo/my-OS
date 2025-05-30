#include <n7OS/process.h>

void processus1() {
    console_putbytes(">> PROCESSUS1 STARTED <<\n", 25);
    for (int i = 0; i < 5; i++) {
        // console_putbytes("P1\n", 3);  // Affiche "P1" pour indiquer que le processus 1 est actif (debug)
        for (volatile int j = 0; j < 1000000; j++); // petite attente
            //ordonnanceur();
    }

    exit();  // termine le processus proprement
}

