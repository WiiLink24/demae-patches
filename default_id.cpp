#include <es.h>
#include <nwc24.h>
#include <rvl.h>
#include <patch.h>

namespace demae {
constexpr char error_message[] = "Please use a NAND dump from a real console.\nFor instructions on how to dump your NAND, visit \nhttps://wii.hacks.guide/bootmii.";

/*
 * DefaultIDCheck checks if the ID is the default Dolphin ID.
 * If it is, it displays a message telling the user to dump their NAND.
 */
void DefaultIDCheck() {
  // If we are good, copy the Wii Number into an area we can use for later.
  u64 friend_code{};
  nwc24::NWC24GetMyUserId(&friend_code);
  nwc24::NWC24iConvIDToStr(friend_code, reinterpret_cast<char *>(0x800017F0));
}
DEMAE_DEFINE_PATCH = {Patch::WriteFunctionCall(0x801baf64, DefaultIDCheck)};
} // namespace demae
