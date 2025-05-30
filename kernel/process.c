#include <n7OS/process.h>
#include <n7OS/console.h>

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
            processus[i].pile = (unsigned int *)malloc(TAILLE_PILE * sizeof(unsigned int));
            // Initialiser la pile et le contexte ici
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
    pid_courant = pid_suivant;
    processus[pid_courant].etat = ELU;
    // Changer de contexte ici
}

void exit(void) {
    processus[pid_courant].etat = TERMINE;
    ordonnanceur();
}
