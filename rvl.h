#pragma once

#include <util.h>

namespace RVL {
    LONGCALL void OSReport( //
            const char* format, ...
    ) AT(0x8019c700);

    LONGCALL void OSWritePanic(void*, void*, const char* message) AT(0x8019e8d0);

    LONGCALL void DCFlushRange(u32* address, u32 size) AT(0x8019bac0);
    LONGCALL void ICInvalidateRange(u32* address, u32 size) AT(0x8019bb78);
}