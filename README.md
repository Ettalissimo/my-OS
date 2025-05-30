# my-OS

# Projet n7OS – Documentation technique

**Nom** : ETTAOUIL Oussama  
**Promotion** : ENSEEIHT 2SN - ASR  
**Fichier** : Documentation des tests, des résultats et des choix techniques.

---

## 1. La Console

- Affichage texte réalisé via la mémoire vidéo (0xB8000).
- Curseur géré manuellement via les ports 0x3D4/0x3D5.
- Affichage avec `console_putbytes()`.
- Gestion des caractères spéciaux : `\n`, `\t`, `\b`, `\f`, etc.
- Test : Messages affichés au démarrage et dans les processus.  
Fonctionnel.

---

## 2. La Pagination

- Activation du paging avec `initialise_paging()`.
- Mapping identity sur les 8 premiers Mo de mémoire.
- Utilisation de CR3 et CR0 pour activer le mode pagination.
- Test d’accès mémoire à l’adresse virtuelle `0x400000`.
- Test : Aucune page fault déclenchée.  
Fonctionnel.

---

## 3. Introduction aux Interruptions

- Table IDT configurée avec `init_idt_entry()`.
- Interruption logicielle `int 50` testée (non utilisée ensuite).
- Activation globale avec `sti()`.
- Test : IRQ0 (timer) et IRQ1 (clavier) actives.  
Fonctionnel.

---

## 4. Le Timer

- PIT configuré à 1000 Hz.
- Handler C (`timer_handler_C`) incrémente `tick_count`.
- Affichage du temps possible toutes les 100 interruptions.
- `ordonnanceur()` appelé automatiquement toutes les 10 interruptions.
- Test : Les processus changent automatiquement.  
Fonctionnel.

---

## 5. Appels Système

- Interruption logicielle `int 0x80` installée via `handler_syscall`.
- Table d'appels système remplie avec `init_syscall()`.
- Appels testés :
  - `example()` retourne 1
  - `write()` affiche à l'écran (remplace `console_putbytes`)
  - `shutdown(n)` arrête QEMU si `n == 1`
- Test : Tous les appels fonctionnent (vérifié via `start.c`).  
Fonctionnalité implémentée et testée.

---

## 6. Le Clavier

- IRQ1 traitée avec un handler assembleur et fonction C.
- Lecture du code touche via le port `0x60`.
- Table de correspondance vers caractères ASCII.
- Buffer circulaire implémenté pour stocker les caractères.
- `kgetch()` utilisée dans un test interactif avec la touche ESC.
- Test : Saisie clavier entièrement fonctionnelle.  
Fonctionnel.

---

## 7. Gestion des Processus

- `processus_t` contient PID, nom, pile, état, `regs[5]`.
- Table `processus[MAX_PROCESSUS]` globale.
- `creer_processus()` crée les processus.
- `processus1()` affiche "P1", puis se termine.
- `idle()` reste actif et affiche des points.
- `ctx_sw()` assure le changement de contexte (assembleur).
- Ordonnancement round-robin automatique via le timer.
- Test : Exécution alternée de processus observée.  
Fonctionnel.

---

## Choix Techniques

- Table des processus globale, accessible depuis tous les modules.
- Ordonnanceur cyclique simple (round-robin).
- Pile dédiée à chaque processus, contexte stocké dans `regs[5]`.
- Pas de gestion dynamique : structures statiques.
- Console et clavier en mode texte, gestion par interruptions.

---

## Remarque

Pour tester les fonctionnalités, ouvrez le fichier `start.c` et décommentez les blocs correspondant à chaque test.

---
