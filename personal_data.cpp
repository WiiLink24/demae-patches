#include <cstdlib.h>
#include <personal_data.h>
#include <rvl.h>

namespace demae::PersonalData {
void LoadPersonalData(PersonalData *pd) {
  // Check if PD is loaded
  if (*reinterpret_cast<u32 *>(0x8038a598) == 0)
    return;

  auto *info_block =
      reinterpret_cast<InfoBlock *>(*reinterpret_cast<u32 *>(0x8038a598));

  char *buffer = reinterpret_cast<char *>(cstdlib::malloc(256));
  Utf16ToUtf8(buffer, 256, info_block->address, 256);
  MoveAddress(pd, buffer);

  Utf16ToUtf8(buffer, 256, info_block->postal_code, 256);
  MovePostalCode(pd, buffer);
}
} // namespace demae::PersonalData
