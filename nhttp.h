#pragma once

#include <util.h>

typedef int (*func)();

namespace nhttp {
    // It is extremely important to note that I don't know if any of these field names are correct except for connection.
    struct __attribute__((packed)) NHTTPContext {
        u8 unknown[8];
        int* connection;
        void* some_memory;
        void* something;
        u32 _;
        s32 status_code;
        s32 fail;
        u32 some;
        u32 thing;
        u8 unknown_2[12];
        void* response;
    };

    LONGCALL int NHTTPMemFree(void* memory) AT(0x800229d4);

    LONGCALL void NHTTPDeleteConnection(int* nhttp_connection) AT(0x80247104);

    LONGCALL int* NHTTPCreateConnection(char* url, int _, void* memory, void* something, func headers_func, NHTTPContext* ctx) AT(0x80247644);

    LONGCALL int MakeNHTTPHeaders(int param_1, int param_2, void* param_3) AT(0x8002b9f0);

    LONGCALL int NHTTPAddHeaderField(int* nhttp_connection, const char* key, const char* value) AT(0x802476c0);

    LONGCALL void NHTTPSetProxy(int* nhttp_connection) AT(0x80247b7c);

    LONGCALL int NHTTPStartConnection(int* nhttp_connection) AT(0x802472a0);

    LONGCALL int NHTTPCheckConnection(int* nhttp_connection) AT(0x80247460);
}

LONGCALL void freeMem(void* ptr) AT(0x80025be8);