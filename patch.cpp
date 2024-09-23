#include <algorithm>
#include <cstdlib.h>
#include <patch.h>
#include <rvl.h>
#include <util.h>

namespace demae::Patch {
extern patch PatchStart asm("_G_DemaePatchStart");
extern patch PatchEnd asm("_G_DemaePatchEnd");

SECTION(".apply_patches") void ApplyPatches() {
  u32 patch_count = std::distance(&PatchStart, &PatchEnd);
  for (u32 i = 0; i < patch_count; i++) {
    ApplyPatch((&PatchStart)[i]);
  }
}

void ApplyPatch(patch &patch) {
  u32 *address = reinterpret_cast<u32 *>(patch.address);

  u32 baseArg0 = patch.arg0 & 0x80000000 ? patch.arg0 : 0x80001800 + patch.arg0;
  u32 *arg0Ptr = reinterpret_cast<u32 *>(baseArg0);
  u32 flush_size{};

  switch (patch.patch_type) {
  case WRITE:
    // Write directly to memory
    cstdlib::memcpy(address, arg0Ptr, patch.arg1);
    flush_size = patch.arg1;
    break;

  case WRITE_U32:
    *address = patch.arg0;
    flush_size = sizeof(u32);
    break;

  case FUNCTION_CALL:
    // Calculate call (bl) value
    *address = 0x48000001 | ((baseArg0 - patch.address) & 0x3FFFFFC);
    flush_size = sizeof(u32);
    break;

  case BRANCH_CTR_LINK:
    if (patch.arg1 > 31) {
      return;
    }

    address[0] = 0x3C000000 | (baseArg0 >> 16) | (patch.arg1 << 21);
    address[1] = 0x60000000 | (baseArg0 & 0xFFFF) | (patch.arg1 << 21) |
                 (patch.arg1 << 16);
    address[2] = 0x7C0903A6 | (patch.arg1 << 21);
    address[3] = 0x4E800421;
    flush_size = sizeof(u32) * 4;
    break;
  }

  RVL::DCFlushRange(address, flush_size);
  RVL::ICInvalidateRange(address, flush_size);
}
} // namespace demae::Patch
