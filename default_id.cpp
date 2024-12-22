#include <es.h>
#include <nwc24.h>
#include <rvl.h>

#if DOMINOS
#include <patch.h>
#endif

namespace demae {
constexpr const char error_message[] = "Please use a NAND dump from a real console.\nFor instructions on how to dump your NAND, visit \nhttps://wii.hacks.guide/bootmii.";

/*
 * DefaultIDCheck checks if the ID is the default Dolphin ID.
 * If it is, it displays a message telling the user to dump their NAND.
 */
void DefaultIDCheck() {
  es::ESInit();

  u32 console_id{};
  es::ES_GetDeviceId(&console_id);

#if (!BETA && !DEV)
  if (console_id == DEFAULT_DOLPHIN_ID) {
    RVL::OSWritePanic(reinterpret_cast<void *>(0x804725a0),
                      reinterpret_cast<void *>(0x80474580),
                      error_message);
    __builtin_unreachable();
  }
#endif

  // If we are good, copy the Wii Number into an area we can use for later.
  u64 friend_code{};
  nwc24::NWC24GetMyUserId(&friend_code);
  nwc24::NWC24iConvIDToStr(friend_code, reinterpret_cast<char *>(0x800017F0));
}
#if DOMINOS
DEMAE_DEFINE_PATCH = {Patch::WriteFunctionCall(0x801baf64, DefaultIDCheck)};
#endif
} // namespace demae
