/**
 * @file mem.c
 * @brief Gestion de l'allocation des pages de la mémoire physique
 */

#include <n7OS/mem.h>
#include <stdio.h>    // pour print_mem()

/* Nombre total de pages (16 Mo / 4 Ko) */
#define NB_PAGES ((LAST_MEMORY_INDEX + 1) / PAGE_SIZE)
/* Taille du bitmap en mots de 32 bits */
#define BITMAP_SIZE (NB_PAGES / 32)

static uint32_t page_bitmap[BITMAP_SIZE];

/**
 * @brief Initialise le gestionnaire de mémoire physique
 */
void init_mem() {
    for (int i = 0; i < BITMAP_SIZE; i++) {
        page_bitmap[i] = 0;
    }
}

/**
 * @brief Marque la page allouée
 */
void setPage(uint32_t addr) {
    uint32_t page = addr / PAGE_SIZE;
    uint32_t idx  = page / 32;
    uint32_t bit  = page % 32;
    page_bitmap[idx] |= (1u << bit);
}

/**
 * @brief Désalloue la page
 */
void clearPage(uint32_t addr) {
    uint32_t page = addr / PAGE_SIZE;
    uint32_t idx  = page / 32;
    uint32_t bit  = page % 32;
    page_bitmap[idx] &= ~(1u << bit);
}

/**
 * @brief Trouve et alloue la première page libre
 * @return Adresse de début de page allouée, ou 0 si plus de place
 */
uint32_t findfreePage() {
    for (uint32_t page = 0; page < NB_PAGES; page++) {
        uint32_t idx = page / 32;
        uint32_t bit = page % 32;
        if ((page_bitmap[idx] & (1u << bit)) == 0) {
            /* on marque la page comme allouée */
            page_bitmap[idx] |= (1u << bit);
            return page * PAGE_SIZE;
        }
    }
    return 0;  /* plus de page libre */
}

/**
 * @brief Affiche l'état de la mémoire physique
 */
void print_mem() {
    uint32_t used = 0;
    for (uint32_t i = 0; i < NB_PAGES; i++) {
        uint32_t idx = i / 32;
        uint32_t bit = i % 32;
        if (page_bitmap[idx] & (1u << bit)) {
            used++;
        }
    }
    printf("Mémoire physique : %u pages totales, %u pages utilisées, %u pages libres\n",
           NB_PAGES, used, NB_PAGES - used);
}
