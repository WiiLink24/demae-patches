#pragma once

#include "util.h"

constexpr u32 DEFAULT_DOLPHIN_ID = 0x2463b87c;

LONGCALL int ESInit() AT(0x801f9ec0);

LONGCALL int ES_GetDeviceId(u32* out) AT(0x801fa804);

LONGCALL void PPCHalt() AT(0x801bbf74);
