#pragma once

#include <cstdlib.h>
#include <util.h>

namespace sc {
LONGCALL void FindByteArrayItem(void *buffer, int offset, size_t size)
    AT(0x80236f24);


u32 GetCountry() {
  void *ptr = cstdlib::malloc(0x1009);

  FindByteArrayItem(ptr, 0x1008, 0x10);
  u32 val = *reinterpret_cast<u32 *>(ptr);

  cstdlib::freeMem(ptr);
  return val >> 0x18;
}
} // namespace sc