#include <patch.h>
#include <util.h>
#include <rvl.h>
#include <algorithm>

namespace demae::Patch {
    extern patch PatchStart asm("_G_DemaePatchStart");
    extern patch PatchEnd asm("_G_DemaePatchEnd");

    SECTION(".apply_patches") void ApplyPatches() {
      u32 patch_count = std::distance(&PatchStart, &PatchEnd);
      for (u32 i = 0; i < patch_count; i++) {
        ApplyPatch((&PatchStart)[i]);
      }
    }

    void ApplyPatch(patch& patch) {
      u32* address = reinterpret_cast<u32*>(patch.address);

      u32 baseArg = patch.arg & 0x80000000 ? patch.arg : 0x80476150 + patch.arg;

      // Calculate call (bl) value
      *address = 0x48000001 | ((baseArg - patch.address) & 0x3FFFFFC);
      RVL::DCFlushRange(address, sizeof(u32));
      RVL::ICInvalidateRange(address, sizeof(u32));
    }
}
