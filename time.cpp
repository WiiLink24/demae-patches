#include <patch.h>

namespace demae::Time {
    LONGCALL u64 OSGetTime(void) AT(0x801a3f70);
    LONGCALL void StoreTime(u32, u32, u32*) AT(0x801a41bc);

    void FixTime(u32, u32, u32* time_buf) {
      // For some reason the time defaults to Japanese time even if your system is not set to that.
      // I hate Nintendo for the fact that I have to do this.
      u64 time = OSGetTime();
      StoreTime(time >> 32, time, time_buf);
    }

    // FixWiiConnect24MsgBoardTime changes the current time to local time so the Demae message appears in current day.
    void FixWiiConnect24MsgBoardTime(u32* time_buf) {
      u64 time = OSGetTime();
      StoreTime(time >> 32, time, time_buf);
    }

    DEMAE_DEFINE_PATCH = {
            Patch::WriteFunctionCall(0x8008c09c, FixTime),
            Patch::WriteFunctionCall(0x8008c0ac, FixTime),
            // Giant Switch-Case
            Patch::WriteFunctionCall(0x800aa874, FixTime),
            Patch::WriteFunctionCall(0x800aa884, FixTime),
            Patch::WriteFunctionCall(0x8024f2dc, FixWiiConnect24MsgBoardTime),

            // The channel wants the user to select the time from (waiting_time + 10) minutes after the current time.
            // Dominos wants an hour + the interval (15). We can just do waiting time + an hour and be fine.
            Patch::WriteASM(0x8008caa4, 1, ASM_LAMBDA(
                      li r5, 60;
                    )),

            Patch::WriteASM(0x8008cfe0, 1, ASM_LAMBDA(
                    li r5, 60;
            ))
    };
}