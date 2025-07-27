#include <cstdlib.h>
#include <nwc24.h>
#include <patch.h>
#include <personal_data.h>
#include <setting.h>
#include <util.h>

namespace demae {
constexpr char msg[] =
    "Order successfully placed.\nTo track your order, click "
    "this link.\nhttp://tracker.wiilink24.com?phone=%ls&country=%s";

/*
 * SetCustomMessageBoardText sets the order confirmation message to one where
 * you can track your order straight from the Wii Internet Channel.
 */
int SetCustomMessageBoardText(void *ctx, const char *_msg, u32 msg_size,
                              int one, int two) {
  // Get phone number for the tracker URL
  auto *info_block = reinterpret_cast<PersonalData::InfoBlock *>(
      *reinterpret_cast<u32 *>(0x8038a598));

  // Get the country string then copy the full string
  char *msg_buffer = static_cast<char *>(cstdlib::malloc(160));
  if (sc::GetCountry() == 18)
    cstdlib::sprintf(msg_buffer, msg, info_block->phone_number, "ca");
  else
    cstdlib::sprintf(msg_buffer, msg, info_block->phone_number, "us");

  // Finally, convert to UTF16 and send to the SetMsg function.
  u32 size = cstdlib::strlen(msg_buffer);
  void *dst = cstdlib::malloc((size + 1) * 2);
  ShiftJISToUTF(dst, &size, msg_buffer, &size);
  // Null terminator
  *(static_cast<wchar_t *>(dst) + size) = L'\0';

  return nwc24::NWC24SetMsgText(ctx, static_cast<const char *>(dst),
                                (size + 1) * 2, one, two);
}

DEMAE_DEFINE_PATCH = {
    Patch::WriteFunctionCall(0x800ca170, SetCustomMessageBoardText)};
} // namespace demae