#pragma once

#include <util.h>
#include <cstdlib.h>

namespace sc {
    LONGCALL int GetSCLabel(const char* label, char* buffer, u32 size) AT(0x80237ee4);

    LONGCALL void FindByteArrayItem(void* buffer, int offset, size_t size) AT(0x80236f24);

    u32 GetCountryLanguage() {
      void* ptr = cstdlib::malloc(0x1009);

      FindByteArrayItem(ptr, 0x1008,0x10);
      return *reinterpret_cast<u32*>(ptr) >> 0x18;
    }
}