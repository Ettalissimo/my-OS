#include <stdint.h>  // ✅ à ajouter tout en haut

#ifndef _PROCESSUS_H_
#define _PROCESSUS_H_

#define STACK_SIZE 1024
#define MAX_PROCESSUS 10


typedef enum {
    ELU,
    PRET,
    BLOQUE,
    TERMINE
} etat_t;

typedef struct {
    int pid;
    char nom[32];
    etat_t etat;
    void (*fonction)(void);
    uint32_t regs[5];             // Contexte : ebx, esp, ebp, esi, edi
    uint32_t pile[STACK_SIZE];    // vraie pile du processus
} processus_t;

void init_processus(void);
int creer_processus(char *nom, void (*fonction)(void));
void ordonnanceur(void);
void fin_processus(void);

#endif
