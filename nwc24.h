#pragma once

#include <util.h>

namespace nwc24 {
    LONGCALL int NWC24GetMyUserId(u32 *user_id) AT(0x8024a3b0);

    LONGCALL void NWC24iConvIdToStr(u32 lower, u32 upper, char* buffer) AT(0x8024aaa8);
}