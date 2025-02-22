#pragma once

#include <util.h>

namespace demae::PersonalData {
struct PACKED PersonalData {
  u8 empty[4];
  char *first_name;
  char *last_name;
  char *first_name_kana;
  char *last_name_kana;
  wchar_t *first_name_utf16;
  wchar_t *last_name_utf16;
  wchar_t *first_name_kana_utf16;
  wchar_t *last_name_kana_utf16;
  u8 also_empty[8];
  char *postal_code;
  wchar_t *postal_code_utf16;
  char *state;
  wchar_t *state_utf16;
  char* city;
  char *address;
  u8 null[4];
  char *apartment_number;
  u8 no_[32];
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
LONGCALL void MoveApartmentNumber(PersonalData*, char*) AT(0x800bb16c);
} // namespace demae::PersonalData