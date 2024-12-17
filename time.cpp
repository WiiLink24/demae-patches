#include <patch.h>

namespace demae::Time {
    void FixTime(u32, u32, u32* time_buf) {
      // For some reason the time defaults to Japanese time even if your system is not set to that.
      // I hate Nintendo for the fact that I have to do this.
      LONGCALL u64 OSGetTime(void) AT(0x801a3f70);
      LONGCALL void StoreTime(u32, u32, u32*) AT(0x801a41bc);

      u64 time = OSGetTime();
      StoreTime(time >> 32, time, time_buf);
    }

    DEMAE_DEFINE_PATCH = {
            Patch::WriteFunctionCall(0x8008c09c, FixTime),
            Patch::WriteFunctionCall(0x8008c0ac, FixTime),
            // Giant Switch-Case
            Patch::WriteFunctionCall(0x800aa874, FixTime),
            Patch::WriteFunctionCall(0x800aa884, FixTime),
    };
}