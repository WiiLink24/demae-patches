#pragma once

#include <util.h>

constexpr u32 DEFAULT_DOLPHIN_ID = 0x0403AC68;

namespace es {
LONGCALL int ESInit() AT(0x801f9ec0);

LONGCALL int ES_GetDeviceId(u32 *out) AT(0x801fa804);
} // namespace es
