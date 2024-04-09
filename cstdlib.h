#pragma once

#include <util.h>
#include <cctype>

namespace cstdlib {
    LONGCALL size_t strlen(const char* s) AT(0x800fb34c);
    LONGCALL void* malloc(size_t len) AT(0x80025b80);
    LONGCALL void memcpy(void *dst,void *src,size_t n) AT(0x80004000);
    LONGCALL int sprintf(char* buffer, const char* format, ...) AT(0x80101e30);
}