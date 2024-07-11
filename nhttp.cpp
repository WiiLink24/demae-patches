#include <cstdlib.h>
#include <nhttp.h>
#include <nwc24.h>
#include <patch.h>
#include <personal_data.h>
#include <rvl.h>
#include <setting.h>

namespace demae {

    /*
     * HttpGet overwrites the original HTTP GET function and adds a few extra headers that are required
     * for us to retrieve restaurants.
     */
int HttpGet(nhttp::NHTTPContext *ctx, char *url, bool is_https) {
  if (ctx->response != nullptr) {
    nhttp::NHTTPMemFree(ctx->response);
    ctx->response = nullptr;
  }

  ctx->something = nullptr;
  if (ctx->connection != nullptr) {
    nhttp::NHTTPDeleteConnection(ctx->connection);
    ctx->connection = nullptr;
  }

  ctx->status_code = 200;
  ctx->fail = 0;
  ctx->some = 0;
  ctx->thing = 0;

  ctx->connection = nhttp::NHTTPCreateConnection(
      url, 0, ctx->response, ctx->something, nhttp::MakeNHTTPHeaders, ctx);

  if (ctx->connection == nullptr) {
    ctx->fail = -1;
    // failed NHTTPCcreateConnection
    RVL::OSReport(reinterpret_cast<const char *>(0x802d64e8));
    return 0;
  }

  if (is_https) {
    int res = nhttp::NHTTPSetBuiltinRootCA(ctx->connection, 1);
    if (res < 0) {
      RVL::OSReport(reinterpret_cast<const char *>(0x802d6526));
      return 0;
    }

    res = nhttp::NHTTPSetVerifyOption(ctx->connection, 0x10b);
    if (res < 0) {
      RVL::OSReport(reinterpret_cast<const char *>(0x802d6544));
      return 0;
    }
  }

  constexpr auto clz = [](int num) -> int {
    int result;
    asm("cntlzw %0, %1" : "=r"(result) : "r"(num));
    return result;
  };

  // Attempt and load personal data
  PersonalData::PersonalData pd{};
  PersonalData::LoadPersonalData(&pd);

  // First request (webApi_documentRequest) does not have pd.dat loaded at
  // request time.
  if (pd.address != nullptr) {
    int res =
        nhttp::NHTTPAddHeaderField(ctx->connection, "X-Address", pd.address);
    if (clz(res) >> 5 == 0)
      return 0;

    res = nhttp::NHTTPAddHeaderField(ctx->connection, "X-PostalCode",
                                     pd.postal_code);
    if (clz(res) >> 5 == 0)
      return 0;
  }

  // Add Wii Number
  int res = nhttp::NHTTPAddHeaderField(ctx->connection, "X-WiiNo",
                                       reinterpret_cast<char *>(0x800017F0));
  if (clz(res) >> 5 == 0)
    return 0;

  // Now country code
  char *country_code = static_cast<char *>(cstdlib::malloc(4));
  cstdlib::sprintf(country_code, "%d", sc::GetCountry());
  res = nhttp::NHTTPAddHeaderField(ctx->connection, "X-WiiCountryCode",
                                   country_code);
  if (clz(res) >> 5 == 0)
    return 0;

  nhttp::NHTTPSetProxy(ctx->connection);
  res = nhttp::NHTTPStartConnection(ctx->connection);
  ctx->fail = res;
  if (res == 0xe) {
    res = nhttp::NHTTPCheckConnection(ctx->connection);
    ctx->some = res;
  } else if (res < 0) {
    // failed NHTTPStartConnection : %d
    RVL::OSReport(reinterpret_cast<const char *>(0x802d64e8), res);
    nhttp::NHTTPDeleteConnection(ctx->connection);
    ctx->connection = nullptr;
  }

  if (ctx->req_data != nullptr) {
    cstdlib::freeMem(ctx->req_data);
    ctx->req_data = nullptr;
  }

  size_t n = cstdlib::strlen(url);
  if (url != nullptr && n != 0) {
    char *dst = reinterpret_cast<char *>(cstdlib::malloc(n + 2));
    ctx->req_data = dst;
    cstdlib::memcpy(dst, url, n);
    *(reinterpret_cast<u8 *>(ctx->req_data) + n) = 0;
  }

  return 1;
}

/*
 * AlterPOSTRequestHeaders adds extra request headers to a POST request.
 */
int AlterPOSTRequestHeaders(int* connection, const char* key, const char* value) {
  constexpr auto clz = [](int num) -> int {
      int result;
      asm("cntlzw %0, %1" : "=r"(result) : "r"(num));
      return result;
  };

  int res = nhttp::NHTTPAddHeaderField(connection, key, value);
  if (clz(res) >> 5 == 0)
    return res;

  // Add country code
  char *country_code = static_cast<char *>(cstdlib::malloc(4));
  cstdlib::sprintf(country_code, "%d", sc::GetCountry());
  return nhttp::NHTTPAddHeaderField(connection, "X-WiiCountryCode", country_code);
}

DEMAE_DEFINE_PATCH = {Patch::WriteFunctionCall(0x8002c42c, HttpGet),
                      Patch::WriteFunctionCall(0x8002c550, HttpGet),
                      Patch::WriteFunctionCall(0x8002c65c, HttpGet),
                      Patch::WriteFunctionCall(0x8002ca98, AlterPOSTRequestHeaders),

                      // Overwrite X-APPVER header.
                      Patch::WriteString(0x802d6300, "X-WiiNo\0"),
                      // Change pointer to header value to the one with the Wii Number.
                      Patch::WriteU32(0x804706dc, 0x800017F0)};
} // namespace demae