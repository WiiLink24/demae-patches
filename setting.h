#pragma once

#include <util.h>

namespace sc {
    LONGCALL int GetSCLabel(const char* label, char* buffer, u32 size) AT(0x80237ee4);
}