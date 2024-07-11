#pragma once

#include <util.h>

namespace demae::PersonalData {
struct PACKED PersonalData {
  u8 _[44];
  char *postal_code;
  u8 no[16];
  char *address;
  u8 no_[40];
  char *email_address;
  char *phone_number;
  u8 _no_[4];
};

struct PACKED InfoBlock {
  wchar_t profile_name[32];
  wchar_t surname[32];
  wchar_t first_name[32];
  wchar_t postal_code[18];
  wchar_t state[32];
  wchar_t city[32];
  wchar_t address[128];
  wchar_t apartment_number[128];
  wchar_t phone_number[32];
  wchar_t email[128];
};

void LoadPersonalData(PersonalData *pd);

LONGCALL int Utf16ToUtf8(char *dst, u32 size, wchar_t *src, u32 _size)
    AT(0x8002444c);
LONGCALL void MoveAddress(PersonalData *, char *) AT(0x800bafdc);
LONGCALL void MovePostalCode(PersonalData *, char *) AT(0x800bacbc);
LONGCALL void MovePhoneNumber(PersonalData *, char *) AT(0x800bb3c4);
} // namespace demae::PersonalData