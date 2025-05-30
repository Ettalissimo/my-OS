#include <n7OS/process.h>
#include <n7OS/console.h>
#include <n7OS/ctx_sw.h>


#define MAX_PROCESSUS 10
#define TAILLE_PILE 4096

static processus_t processus[MAX_PROCESSUS];
static int pid_courant = -1;

void init_processus(void) {
    for (int i = 0; i < MAX_PROCESSUS; i++) {
        processus[i].etat = TERMINE;
    }
}

int creer_processus(char *nom, void (*fonction)(void)) {
    for (int i = 0; i < MAX_PROCESSUS; i++) {
        if (processus[i].etat == TERMINE) {
            processus[i].pid = i;
            strncpy(processus[i].nom, nom, 31);
            processus[i].nom[31] = '\0';
            processus[i].etat = PRET;
            processus[i].fonction = fonction;

            // Initialisation de la pile
            uint32_t *stack = processus[i].pile;
            stack[STACK_SIZE - 1] = (uint32_t)fonction;
            processus[i].regs[0] = 0; // ebx
            processus[i].regs[1] = (uint32_t)&stack[STACK_SIZE - 1]; // esp
            processus[i].regs[2] = 0; // ebp
            processus[i].regs[3] = 0; // esi
            processus[i].regs[4] = 0; // edi

            return i;
        }
    }
    return -1;
}


void ordonnanceur(void) {
    int pid_suivant = (pid_courant + 1) % MAX_PROCESSUS;
    while (processus[pid_suivant].etat != PRET) {
        pid_suivant = (pid_suivant + 1) % MAX_PROCESSUS;
        if (pid_suivant == pid_courant) {
            return; // Aucun processus prêt
        }
    }

    int ancien = pid_courant;
    pid_courant = pid_suivant;
    processus[pid_courant].etat = ELU;

    if (ancien != -1 && processus[ancien].etat != TERMINE)
        processus[ancien].etat = PRET;

    ctx_sw(processus[ancien].regs, processus[pid_courant].regs);
}


void exit(void) {
    processus[pid_courant].etat = TERMINE;
    ordonnanceur();
}
