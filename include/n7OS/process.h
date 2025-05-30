#ifndef _PROCESSUS_H_
#define _PROCESSUS_H_

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
    unsigned int *pile;
} processus_t;

void init_processus(void);
int creer_processus(char *nom, void (*fonction)(void));
void ordonnanceur(void);
void fin_processus(void);

#endif
