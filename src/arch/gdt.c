//
// Created by grsan on 5/23/23.
//

#include <arch/gdt.h>

gdt_entry gdt[3];
gdt_ptr gdtr;

void gdt_set_entry(uint8_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t granularity) {
    gdt[num].limit_low = limit & 0xFFFF;
    gdt[num].flags = (limit >> 16) & 0x0F;

    gdt[num].base_low = base & 0xFFFF;
    gdt[num].base_mid = (base >> 16) & 0xFF;
    gdt[num].base_high = (base >> 24) & 0xFF;

    gdt[num].access = access;
    gdt[num].flags |= granularity & 0xF0;
}

void gdt_install() {
    gdtr.limit = (sizeof(gdt_entry) * 3) - 1;
    gdtr.base = (uint32_t) &gdt;

    // NULL entry
    gdt_set_entry(0,0,0,0,0);

    // Code Segment
    gdt_set_entry(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);

    // Data Segment
    gdt_set_entry(2, 0, 0xFFFFFFFF, 0x92, 0xCF);

    gdt_flush();
}