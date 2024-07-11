#include <asm.h>
#include <patch.h>

namespace demae::Region {
DEMAE_DEFINE_PATCH = {
    // Change NTSC-U PD to JPN
    Patch::WriteASM(0x801a780c, 1, ASM_LAMBDA(addi r29, r3, 0x434a)),
    Patch::WriteASM(0x802345bc, 1, ASM_LAMBDA(addi r30, r4, 0x434a)),
    Patch::WriteASM(0x80234790, 1, ASM_LAMBDA(addi r3, r31, 0x84)),
    Patch::WriteASM(0x80234748, 1, ASM_LAMBDA(addi r3, r31, 0)),

    // Change PAL PD to JPN
    Patch::WriteASM(0x801a782c, 1, ASM_LAMBDA(addi r29, r3, 0x434a)),
    Patch::WriteASM(0x802345d0, 1, ASM_LAMBDA(addi r30, r4, 0x434a)),
    Patch::WriteASM(0x80234798, 1, ASM_LAMBDA(addi r3, r31, 0x84)),
    Patch::WriteASM(0x80234750, 1, ASM_LAMBDA(addi r3, r31, 0))};
}