#include <asm.h>
#include <patch.h>
#include <util.h>
#include <cstdlib.h>
#include <personal_data.h>

namespace demae::Localization {
    void WhatTheFuck(wchar_t* buf) {
      // I was furious while figuring this out, so the function name stays. This formats the address in a proper way.
      auto *info_block = reinterpret_cast<PersonalData::InfoBlock *>(
              *reinterpret_cast<u32 *>(0x8038a598));


      if (cstdlib::wcslen(info_block->apartment_number) == 0)
        cstdlib::wsnprintf(buf, 128, L"%ls, %ls, %ls", info_block->address, info_block->city, info_block->state);
      else
        cstdlib::wsnprintf(buf, 128, L"%ls %ls, %ls, %ls", info_block->address, info_block->apartment_number, info_block->city, info_block->state);
    }

    // TranslateSaveDataTitle exists because implementing an edge case into DolTranslator for a single call is
    // absurd.
    void TranslateSaveDataTitle(void* ctx, int one, const wchar_t* title, void* ptr) {
      LONGCALL void SetSaveDataTitle(void* ctx, int one, const wchar_t* title, void* ptr) AT(0x80233bac);
      SetSaveDataTitle(ctx, one, L"Food Channel", ptr);
    }

DEMAE_DEFINE_PATCH = {
    // Disable validating phone number
    Patch::WriteASM(0x80024620, 2, ASM_LAMBDA(li r3, 1; blr;)),

    // Expected XML Nodes
    Patch::WriteString(0x802d65cb, "Meal"),
    Patch::WriteString(0x802d6776, "Meal"),
    Patch::WriteString(0x802d67fd, "Fox Card"),
    Patch::WriteString(0x802d698a, "Fox Card"),

    // Hardcoded text. Unlike majority of hardcoded text which has an address table for easy switching of text location,
    // these do not and instead load the address they are located in. Rather than having DOLTranslater edit instructions
    // which I don't know how feasible that is due to nuances in how it could have been linked, we will just edit the
    // string using the patch system.
    Patch::WriteString(0x80470816, "\0\0"),
    Patch::WriteString(0x80470ac8, "\0\0"),
    Patch::WriteString(0x804707b2, "\0\0"),
    Patch::WriteString(0x802dbd67, "%s\0\0"),

    // Instead of calling sprintf like a reasonable person, Nintendo decided to implement their own weird integer
    // conversion along with bounds check in both UTF-8 and UTF-16 BE. Due to this they manually insert the byte
    // value of yen (0x5186) at the end of the string rather than with the 3 instances of the formatted yen symbol
    // embedded in the DOL.
    // We simply set the yen value to 0.
    Patch::WriteASM(0x8004f674, 1, ASM_LAMBDA(li r24, 0x0;)),
    Patch::WriteASM(0x8007bd50, 1, ASM_LAMBDA(li r24, 0x0;)),
    Patch::WriteASM(0x80061400, 1, ASM_LAMBDA(li r6, 0x0;)),
    Patch::WriteASM(0x800614a0, 1, ASM_LAMBDA(li r22, 0x0;)),
    Patch::WriteASM(0x8007b7c0, 1, ASM_LAMBDA(li r5, 0x0;)),
    Patch::WriteASM(0x8007b308, 1, ASM_LAMBDA(li r5, 0x0;)),
    Patch::WriteASM(0x8007b0e8, 1, ASM_LAMBDA(li r5, 0x0;)),

    // In their ever infinite wisdom, rather than using wsnprintf to format the address string, Nintendo
    // essentially made their own wscat which reads the PD file sequentially, copying every character into a buffer.
    // This is absolutely ridiculous and makes no sense at all when they already populated a structure they can call
    // on at this point. In order to make the address look like how it would overseas, we do the following:

    // Load the string buffer into r3 (First argument in a function call).
    Patch::WriteASM(0x800f5a5c, 1, ASM_LAMBDA(or r3, r6, r6)),
    // Call the function.
    Patch::WriteFunctionCall(0x800f5a60, WhatTheFuck),
    // Branch to the end of string manipulation so our work isn't overwritten.
    Patch::WriteASM(0x800f5a64, 1, ASM_LAMBDA(b +392)),

    // The initial address verification suffers from the same fate.
    // In this case, the value of r3 needs to persist. r27 isn't in use so store there.
    Patch::WriteASM(0x800f6704, 1, ASM_LAMBDA(or r27, r3, r3)),
    Patch::WriteASM(0x800f6708, 1, ASM_LAMBDA(or r3, r8, r8)),
    Patch::WriteFunctionCall(0x800f670c, WhatTheFuck),
    // Restore r3.
    Patch::WriteASM(0x800f6710, 1, ASM_LAMBDA(or r3, r27, r27)),
    Patch::WriteASM(0x800f6714, 1, ASM_LAMBDA(b +384)),

    // Save data
    Patch::WriteFunctionCall(0x800bd1e4, TranslateSaveDataTitle),

    // URLs
#if DEV
    Patch::WriteString(0x802d63c7, "dev.wiilink24.com/nwapi.php?action=\0\0"),
    Patch::WriteString(0x802d643d, "dev.wiilink24.com/itemimg/\0"),
    Patch::WriteString(0x802d647b, "dev.wiilink24.com/logoimg2/\0\0"),
    Patch::WriteString(0x802d64c1, "dev.wiilink24.com/nwapi.php\0\0"),

    // Disable HTTPS for dev
    Patch::WriteString(0x802d63b6, "http://\0"),
#elif BETA
    Patch::WriteString(0x802d63c7, "bom.wiilink24.com/nwapi.php?action=\0\0"),
    Patch::WriteString(0x802d643d, "bom.wiilink24.com/itemimg/\0"),
    Patch::WriteString(0x802d647b, "bom.wiilink24.com/logoimg2/\0\0"),
    Patch::WriteString(0x802d64c1, "bom.wiilink24.com/nwapi.php\0\0"),
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