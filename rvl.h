#pragma once

#include <util.h>

namespace rvl {
    LONGCALL void OSReport( //
            const char* format, ...
    ) AT(0x8019c700);
}