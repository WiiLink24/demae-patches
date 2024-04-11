#pragma once

#include <util.h>
#include <cstdlib.h>

namespace sc {
    LONGCALL int GetSCLabel(const char* label, char* buffer, u32 size) AT(0x80237ee4);

    LONGCALL void FindByteArrayItem(void* buffer, int offset, size_t size) AT(0x80236f24);

    LONGCALL u8 GetLanguage() AT(0x80237b1c);

    u32 GetCountry() {
      void* ptr = cstdlib::malloc(0x1009);

      FindByteArrayItem(ptr, 0x1008,0x10);
      u32 val = *reinterpret_cast<u32*>(ptr);

      cstdlib::freeMem(ptr);
      return val >> 0x18;
    }
}