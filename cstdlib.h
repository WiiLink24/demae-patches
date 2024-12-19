#pragma once

#include <cctype>
#include <util.h>

namespace cstdlib {
LONGCALL size_t strlen(const char *s) AT(0x800fb34c);
LONGCALL void *malloc(size_t len) AT(0x80025b80);
LONGCALL void memcpy(void *dst, void *src, size_t n) AT(0x80004000);
LONGCALL int sprintf(char *buffer, const char *format, ...) AT(0x80101e30);
LONGCALL void freeMem(void *ptr) AT(0x80025be8);
LONGCALL void memset(void* ptr, int value, size_t size) AT(0x80004134);
LONGCALL void wsnprintf(wchar_t* buf, u32 size, const wchar_t *format, ...) AT(0x80104ff4);
LONGCALL size_t wcslen(wchar_t* str) AT(0x801050d0);
} // namespace cstdlib

// Not in cstdlib so I am putting this in the global namespace
LONGCALL void ShiftJISToUTF(void *dst, u32 *size, char *src, u32 *_size) AT(0x801f5db0);