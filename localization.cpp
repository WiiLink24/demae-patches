#include <asm.h>
#include <patch.h>
#include <util.h>

namespace demae::Localization {
DEMAE_DEFINE_PATCH = {
    // Disable validating phone number
    Patch::WriteASM(0x80024620, 2, ASM_LAMBDA(li r3, 1; blr;)),

    // Expected XML Nodes
    Patch::WriteString(0x802d65cb, "Meal"),
    Patch::WriteString(0x802d6776, "Meal"),
    Patch::WriteString(0x802d67fd, "Fox Card"),
    Patch::WriteString(0x802d698a, "Fox Card"),

    // Hardcoded text

    // URLs
#if DEBUG
    Patch::WriteString(0x802d63c7, "dev.wiilink24.com/nwapi.php?action=\0\0"),
    Patch::WriteString(0x802d643d, "dev.wiilink24.com/itemimg/\0"),
    Patch::WriteString(0x802d647b, "dev.wiilink24.com/logoimg2/\0\0"),
    Patch::WriteString(0x802d64c1, "dev.wiilink24.com/nwapi.php\0\0"),

    // Disable HTTPS for debug
    Patch::WriteString(0x802d63b6, "http://\0"),
#elif DOMINOS
    Patch::WriteString(0x802d63c7, "dom.wiilink24.com/nwapi.php?action=\0\0"),
    Patch::WriteString(0x802d643d, "dom.wiilink24.com/itemimg/\0"),
    Patch::WriteString(0x802d647b, "dom.wiilink24.com/logoimg2/\0\0"),
    Patch::WriteString(0x802d64c1, "dom.wiilink24.com/nwapi.php\0\0"),
#elif REGULAR
    Patch::WriteString(0x802d63c7, "food.wiilink24.com/nwapi.php?action=\0"),
    Patch::WriteString(0x802d643d, "food.wiilink24.com/itemimg/"),
    Patch::WriteString(0x802d647b, "food.wiilink24.com/logoimg2/\0"),
    Patch::WriteString(0x802d64c1, "food.wiilink24.com/nwapi.php\0"),

    // TODO: Remove this when the new version releases
    Patch::WriteString(0x802d63b6, "http://\0"),
#endif
};
} // namespace demae::Localization