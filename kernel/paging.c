#include <n7OS/paging.h>
#include <n7OS/mem.h>
#include <stddef.h> // pour NULL

// Répertoire de pages global du noyau
static PageDirectory kernel_directory;

void sleep(unsigned int ticks) {
    for (unsigned int i = 0; i < ticks * 100000; i++) {
        __asm__ __volatile__("nop");
    }
}

void initialise_paging() {

    console_putbytes("START\n", 6);

    // Initialise les pointeurs à NULL
    for (int i = 0; i < PAGE_DIRECTORY_ENTRIES; i++) {
        kernel_directory.tables[i] = NULL;
        kernel_directory.tables_physical[i] = 0;
    }

    // Mapper les 8 premiers Mo (2 tables de pages = 2 * 4 Mo)
    for (int table_idx = 0; table_idx < 8; table_idx++) {
        PageTable table = (PageTable) findfreePage();
        setPage((uint32_t)table);

        for (int i = 0; i < PAGE_TABLE_ENTRIES; i++) {
            uint32_t phys_addr = (table_idx * PAGE_TABLE_ENTRIES + i) * PAGE_SIZE;
            table[i].page_entry.present = 1;
            table[i].page_entry.rw = 1;
            table[i].page_entry.user = 0;
            table[i].page_entry.frame_addr = phys_addr >> 12;
        }

        kernel_directory.tables[table_idx] = table;
        kernel_directory.tables_physical[table_idx] = (uint32_t)table;
    }

    // ⚠️ Mapper aussi l'adresse de la structure elle-même
    alloc_page_entry((uint32_t)&kernel_directory, 1, 1);

    extern void kernel_start();
    extern void handler32();
    extern void timer_handler_C();
    extern void console_putbytes();
    extern const char tick_msg[];
    extern uint64_t idt[];
    // extern void test_signal();

    extern void timer_handler();
    extern void handler50();


    alloc_page_entry((uint32_t)&timer_handler, 1, 1);


    alloc_page_entry((uint32_t)&kernel_start, 1, 1);
    alloc_page_entry((uint32_t)&handler32, 1, 1);
    alloc_page_entry((uint32_t)&timer_handler_C, 1, 1);
    alloc_page_entry((uint32_t)&console_putbytes, 1, 1);
    alloc_page_entry((uint32_t)tick_msg, 1, 1);
    alloc_page_entry((uint32_t)&idt[32], 1, 1);
    alloc_page_entry((uint32_t)&handler50, 1, 1);

    
    alloc_page_entry(0x000B8000, 1, 1);  // ✅ VGA text mode (écriture directe à l'écran)

    alloc_page_entry(0xB8000, 1, 1);
    //alloc_page_entry((uint32_t)&test_signal, 1, 1);

    // 🔍 Vérifier et mapper la stack
    uint32_t esp;
    __asm__ volatile("mov %%esp, %0" : "=r"(esp));

    char buf[32];
    sprintf(buf, "ESP = %x\n", esp);
    console_putbytes(buf, 32);


    //alloc_page_entry(0x00112000, 1, 1);  // aligne sur 4 Ko = 0x112000
    alloc_page_entry(esp & 0xFFFFF000, 1, 1);

        // Activation de la pagination
    console_putbytes("BEFORE CR3\n", 11);
    __asm__ volatile("mov %0, %%cr3" :: "r"(kernel_directory.physical_addr));

    console_putbytes("BEFORE CR0\n", 11);
    sleep(3000); 


    uint32_t cr0;
    __asm__ volatile("mov %%cr0, %0" : "=r"(cr0));
    cr0 |= 0x80000000; // bit PG (enable paging)
    __asm__ volatile("mov %0, %%cr0" :: "r"(cr0));

    console_putbytes("AFTER CR0\n", 10);

    // Test immédiat (instruction sans pile)
    //__asm__ volatile("nop");
    sleep(3000); 
    sleep(3000);
    console_putbytes("AFTER PAGING\n", 15);



}


PageTable alloc_page_entry(uint32_t address, int is_writeable, int is_kernel) {
    // Calcule les indices du répertoire et de la table
    uint32_t dir_idx = (address >> 22) & 0x3FF;
    uint32_t tab_idx = (address >> 12) & 0x3FF;

    PageTable table = kernel_directory.tables[dir_idx];

    // Si la table n'existe pas encore, on l'alloue
    if (table == NULL) {
        table = (PageTable) findfreePage();
        setPage((uint32_t)table);

        for (int i = 0; i < PAGE_TABLE_ENTRIES; i++) {
            table[i].value = 0;
        }

        kernel_directory.tables[dir_idx] = table;
        kernel_directory.tables_physical[dir_idx] = (uint32_t)table;
    }

    // Alloue une page physique pour l’entrée de table
    uint32_t phys = findfreePage();
    setPage(phys);

    table[tab_idx].page_entry.present = 1;
    table[tab_idx].page_entry.rw = is_writeable ? 1 : 0;
    table[tab_idx].page_entry.user = is_kernel ? 0 : 1;
    table[tab_idx].page_entry.frame_addr = phys >> 12;

    return table;
}
