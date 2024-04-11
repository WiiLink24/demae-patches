#include <nhttp.h>
#include <default_id.h>
#include <cstdlib.h>
#include <setting.h>
#include <rvl.h>

namespace demae {
    SECTION(".wii_id_http") int HttpGet(nhttp::NHTTPContext* ctx, char* url, bool is_https)
    {
      if (ctx->some_memory != nullptr)
      {
        nhttp::NHTTPMemFree(ctx->some_memory);
        ctx->some_memory = nullptr;
      }

      ctx->something = nullptr;
      if (ctx->connection != nullptr)
      {
        nhttp::NHTTPDeleteConnection(ctx->connection);
        ctx->connection = nullptr;
      }

      ctx->status_code = 200;
      ctx->fail = 0;
      ctx->some = 0;
      ctx->thing = 0;

      ctx->connection = nhttp::NHTTPCreateConnection(url, 0, ctx->some_memory, ctx->something,
                                              reinterpret_cast<func>(nhttp::MakeNHTTPHeaders), ctx);

      if (ctx->connection == nullptr)
      {
        ctx->fail = -1;
        // failed NHTTPCcreateConnection
        rvl::OSReport(reinterpret_cast<const char*>(0x802d64e8));
        return 0;
      }

      // Quick clz
      auto clz = [](int num){
          int result;
          asm ("cntlzw %0, %1" : "=r" (result) : "r" (num));
          return result;
      };

      // We don't use HTTPS, otherwise it would be hooked here.
      // Get the user's Wii ID
      u32 console_id{};
      ES_GetDeviceId(&console_id);

      // Max length is 10 characters, length of (2^31) + null terminator
      char str[11];
      cstdlib::sprintf(str, "%d", console_id);

      int res = nhttp::NHTTPAddHeaderField(ctx->connection, "X-WiiID", str);
      if (clz(res))
        return 0;

      // Finally the serial number. 9 + null terminator
      char serial_number[10];
      sc::GetSCLabel("SERNO", serial_number, 10);

      res = nhttp::NHTTPAddHeaderField(ctx->connection, "X-WiiSerial", serial_number);
      if (clz(res))
        return 0;

      char country_code[4];
      cstdlib::sprintf(country_code, "%d", sc::GetCountryLanguage());
      res = nhttp::NHTTPAddHeaderField(ctx->connection, "X-WiiCountryCode", country_code);
      if (clz(res))
        return 0;

      nhttp::NHTTPSetProxy(ctx->connection);
      res = nhttp::NHTTPStartConnection(ctx->connection);
      if (res == 0xe)
      {
        res = nhttp::NHTTPCheckConnection(ctx->connection);
        ctx->some = res;
      }
      else if (res < 0)
      {
        // failed NHTTPStartConnection : %d
        rvl::OSReport(reinterpret_cast<const char*>(0x802d64e8), res);
        nhttp::NHTTPDeleteConnection(ctx->connection);
        ctx->connection = nullptr;
      }

      if (ctx->response != nullptr)
      {
        freeMem(ctx->response);
        ctx->response = nullptr;
      }

      size_t n = cstdlib::strlen(url);
      if (url != nullptr && n != 0)
      {
        void* dst = cstdlib::malloc(n + 2);
        ctx->response = dst;
        cstdlib::memcpy(dst, url, n);
      }

      return 1;
    }
}