#include <cstdlib.h>
#include <nhttp.h>
#include <nwc24.h>
#include <patch.h>
#include <personal_data.h>
#include <rvl.h>
#include <setting.h>

namespace demae {
    /*
     * AlterHTTPGetHeaders is inserted where HTTPGet attempts to create the X-APPVER header.
     * That header however has been replaced by X-WiiNo and our Wii Number.
     */
int AlterHTTPGETHeaders(int* connection, const char* key, const char* value) {
  // Add Wii Number
  int res = nhttp::NHTTPAddHeaderField(connection, key, value);
  if (res != 0)
    return 0;

  // Attempt and load personal data
  PersonalData::PersonalData pd{};
  PersonalData::LoadPersonalData(&pd);

  // First request (webApi_documentRequest) does not have pd.dat loaded at
  // request time. We don't need it so this isn't an issue.
  if (pd.address != nullptr) {
    res =
        nhttp::NHTTPAddHeaderField(connection, "X-Address", pd.address);
    if (res != 0)
      return 0;

    res = nhttp::NHTTPAddHeaderField(connection, "X-PostalCode",
                                     pd.postal_code);
    if (res != 0)
      return 0;
  }

  // Now country code
  char *country_code = static_cast<char *>(cstdlib::malloc(4));
  cstdlib::sprintf(country_code, "%d", sc::GetCountry());
  return nhttp::NHTTPAddHeaderField(connection, "X-WiiCountryCode",
                                   country_code);
}

/*
 * AlterPOSTRequestHeaders adds extra request headers to a POST request.
 */
int AlterPOSTRequestHeaders(int* connection, const char* key, const char* value) {
  int res = nhttp::NHTTPAddHeaderField(connection, key, value);
  if (res != 0)
    return res;

  // Add country code
  char *country_code = static_cast<char *>(cstdlib::malloc(4));
  cstdlib::sprintf(country_code, "%d", sc::GetCountry());
  return nhttp::NHTTPAddHeaderField(connection, "X-WiiCountryCode", country_code);
}

DEMAE_DEFINE_PATCH = {Patch::WriteFunctionCall(0x8002ce98, AlterHTTPGETHeaders),
                      Patch::WriteFunctionCall(0x8002ca98, AlterPOSTRequestHeaders),

                      // Overwrite X-APPVER header.
                      Patch::WriteString(0x802d6300, "X-WiiNo\0"),
                      // Change pointer to header value to the one with the Wii Number.
                      Patch::WriteU32(0x804706dc, 0x800017F0)};
} // namespace demae