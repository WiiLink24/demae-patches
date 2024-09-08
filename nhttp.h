#pragma once

#include <util.h>

namespace nhttp {
LONGCALL int NHTTPAddHeaderField(int *nhttp_connection, const char *key,
                                 const char *value) AT(0x802476c0);
} // namespace nhttp