/**
 * @file paging.h
 * @brief Gestion de la pagination dans le noyau
 */
#ifndef _PAGING_H
#define _PAGING_H

#include <inttypes.h>

/* Taille d'une page : 4 Ko */
#define PAGE_SIZE 0x1000

/* Nombre d'entrées dans une table de pages ou un répertoire de pages */
#define PAGE_TABLE_ENTRIES 1024
#define PAGE_DIRECTORY_ENTRIES 1024

/**
 * @brief Description d'une ligne de la table de page (PTE : Page Table Entry)
 */
typedef struct {
    uint32_t present    : 1;   // 1 = page présente en mémoire
    uint32_t rw         : 1;   // 1 = page accessible en écriture
    uint32_t user       : 1;   // 1 = accessible depuis espace utilisateur
    uint32_t accessed   : 1;   // 1 = déjà accédée
    uint32_t dirty      : 1;   // 1 = déjà modifiée
    uint32_t reserved   : 7;   // inutilisé
    uint32_t frame_addr : 20;  // adresse de la page physique >> 12
} page_table_entry_t;

/**
 * @brief Une entrée dans la table de page peut être manipulée en utilisant
 *        la structure page_table_entry_t ou directement la valeur brute
 */
typedef union {
    page_table_entry_t page_entry;
    uint32_t value;
} PTE; // Page Table Entry

/**
 * @brief Une table de page est un tableau de 1024 PTE
 */
typedef PTE * PageTable;

/**
 * @brief Le répertoire de pages contient les pointeurs vers les tables de pages
 */
typedef struct {
    PageTable tables[PAGE_DIRECTORY_ENTRIES];       // pointeurs vers tables
    uint32_t tables_physical[PAGE_DIRECTORY_ENTRIES]; // adresses physiques
    uint32_t physical_addr;                          // adresse physique du répertoire
} PageDirectory;

/**
 * @brief Initialise la pagination : répertoire + table + activation
 */
void initialise_paging();

/**
 * @brief Alloue une page physique et l’associe à une adresse virtuelle
 * 
 * @param address Adresse virtuelle à mapper (doit être page-aligned)
 * @param is_writeable 1 si accessible en écriture
 * @param is_kernel 1 si réservé noyau, 0 si accessible utilisateur
 * @return PageTable La table de page modifiée (ou NULL si échec)
 */
PageTable alloc_page_entry(uint32_t address, int is_writeable, int is_kernel);

#endif /* _PAGING_H */
