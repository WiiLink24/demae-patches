#include <nhttp.h>
#include <default_id.h>
#include <cstdlib.h>

namespace demae {
    __attribute__((__section__(".wii_id_http"))) int HttpGet(nhttp::NHTTPContext* ctx, char* url, bool is_https)
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
        // OSReport
        return 0;
      }

      // We don't use HTTPS, otherwise it would be hooked here.
      // Get the user's Wii ID
      int console_id{};
      ES_GetDeviceId(&console_id);

      char str[20];
      cstdlib::sprintf(str, "%d", console_id);

      // TODO: Check for error
      nhttp::NHTTPAddHeaderField(ctx->connection, "X-WiiID", str);

      nhttp::NHTTPSetProxy(ctx->connection);
      int res = nhttp::NHTTPStartConnection(ctx->connection);
      if (res == 0xe)
      {
        res = nhttp::NHTTPCheckConnection(ctx->connection);
        ctx->some = res;
      }
      else if (res < 0)
      {
        // OSReport
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