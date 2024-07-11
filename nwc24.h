#pragma once

#include <util.h>

namespace nwc24 {
LONGCALL void NWC24iConvIDToStr(u64 id, char *out) AT(0x8024aaa8);

LONGCALL int NWC24GetMyUserId(u64 *id) AT(0x8024a3b0);

LONGCALL int NWC24SetMsgText(void *ctx, const char *msg, u32 msg_size, int, int)
    AT(0x8024b268);
} // namespace nwc24