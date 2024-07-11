#pragma once

#include <util.h>

typedef int (*func)(int param_1, int param_2, void *param_3);

namespace nhttp {
// It is extremely important to note that I don't know if any of these field
// names are correct except for connection.
struct PACKED NHTTPContext {
  u8 unknown[8];
  int *connection;
  void *response;
  void *something;
  u32 one;
  s32 status_code;
  s32 fail;
  u32 some;
  u32 thing;
  u32 zero;
  u8 unknown_2[8];
  char *req_data;
};

LONGCALL int NHTTPMemFree(void *memory) AT(0x800229d4);

LONGCALL void NHTTPDeleteConnection(int *nhttp_connection) AT(0x80247104);

LONGCALL int *NHTTPCreateConnection(char *url, int _, void *memory,
                                    void *something, func headers_func,
                                    NHTTPContext *ctx) AT(0x80247644);

LONGCALL int MakeNHTTPHeaders(int param_1, int param_2, void *param_3)
    AT(0x8002b9f0);

LONGCALL int NHTTPAddHeaderField(int *nhttp_connection, const char *key,
                                 const char *value) AT(0x802476c0);

LONGCALL void NHTTPSetProxy(int *nhttp_connection) AT(0x80247b7c);

LONGCALL int NHTTPStartConnection(int *nhttp_connection) AT(0x802472a0);

LONGCALL int NHTTPCheckConnection(int *nhttp_connection) AT(0x80247460);

LONGCALL int NHTTPSetBuiltinRootCA(int *connection, int param_2) AT(0x80247ca4);

LONGCALL int NHTTPSetVerifyOption(int *connection, int param_2) AT(0x802479b0);

LONGCALL int NHTTPGetFail(NHTTPContext *ctx) AT(0x8002c254);

LONGCALL int NHTTPGetStatusCode(NHTTPContext *ctx) AT(0x8002c24c);

LONGCALL int NHTTPGetOne(NHTTPContext *ctx) AT(0x8002c234);

struct PACKED ActionType {
  u32 query_param_count;
  u32 action_type;
  char action_type_string[64];
};

ActionType *action_types = reinterpret_cast<ActionType *>(0x80327be4);

struct PACKED URLParam {
  u32 unknown;
  u32 index;
  char param_name[64];
};

URLParam *url_params = reinterpret_cast<URLParam *>(0x803282a4);
} // namespace nhttp