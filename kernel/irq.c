#include <n7OS/irq.h>
#include <n7OS/processor_structs.h>
#include <inttypes.h>

/**
 * @brief Initialise une entrée dans l’IDT
 * 
 * @param num_line Numéro de l’interruption (0 à 255)
 * @param addr     Adresse du handler
 */
void init_irq_entry(int num_line, uint32_t addr) {
    // Encodage manuel d’une entrée IDT 64 bits (interruption gate 32 bits)
    uint64_t entry = 0;

    uint16_t offset_low  = addr & 0xFFFF;
    uint16_t offset_high = (addr >> 16) & 0xFFFF;

    uint64_t selector    = KERNEL_CS; // défini dans segment.h (0x10)
    uint64_t type_attr   = 0x8E;      // P=1, DPL=00, S=0, Type=1110 (32-bit interrupt gate)

    // Construction de l’entrée IDT en 64 bits
    entry = offset_low | (selector << 16) | ((uint64_t)type_attr << 40) | ((uint64_t)offset_high << 48);

    // On écrit dans la table IDT globale (déclarée dans processor_structs.h)
    idt[num_line] = entry;
}

void load_idt(void) {
    struct {
        uint16_t limit;
        uint32_t base;
    } __attribute__((packed)) idtr;

    idtr.limit = sizeof(idt) - 1;
    idtr.base = (uint32_t)idt;

    __asm__ volatile("lidt %0" :: "m"(idtr));
}


