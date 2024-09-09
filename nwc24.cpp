#include <cstdlib.h>
#include <nwc24.h>
#include <patch.h>
#include <personal_data.h>
#include <util.h>
#include <setting.h>

namespace demae {
constexpr char msg[] = "Order successfully placed.\nTo track your order, click "
                       "this link.\nhttp://tracker.wiilink24.com?phone=%s&country=%s";

/*
 * SetCustomMessageBoardText sets the order confirmation message to one where you can track
 * your order straight from the Wii Internet Channel.
 */
int SetCustomMessageBoardText(void *ctx, const char *_msg, u32 msg_size,
                              int one, int two) {
  // Get phone number for the tracker URL
  auto *info_block = reinterpret_cast<PersonalData::InfoBlock *>(
      *reinterpret_cast<u32 *>(0x8038a598));
  char *buffer = reinterpret_cast<char *>(cstdlib::malloc(256));

  // Move phone number into the region it needs to be in
  PersonalData::PersonalData pd{};
  PersonalData::Utf16ToUtf8(buffer, 256, info_block->phone_number, 256);
  PersonalData::MovePhoneNumber(&pd, buffer);

  // Get the country string then copy the full string
  char *msg_buffer = reinterpret_cast<char *>(cstdlib::malloc(160));
  if (sc::GetCountry() == 18)
    cstdlib::sprintf(msg_buffer, msg, pd.phone_number, "ca");
  else
    cstdlib::sprintf(msg_buffer, msg, pd.phone_number, "us");

  // Finally, convert to UTF16 and send to the SetMsg function.
  u32 size = cstdlib::strlen(msg_buffer);
  void *dst = cstdlib::malloc((size + 1) * 2);
  ShiftJISToUTF(dst, &size, msg_buffer, &size);
  // Null terminator
  *(reinterpret_cast<wchar_t *>(dst) + size) = L'\0';

  return nwc24::NWC24SetMsgText(ctx, static_cast<const char *>(dst),
                                (size + 1) * 2, one, two);
}

DEMAE_DEFINE_PATCH = {
    Patch::WriteFunctionCall(0x800ca170, SetCustomMessageBoardText)};
} // namespace demae