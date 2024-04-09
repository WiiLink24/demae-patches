#include <default_id.h>

namespace demae {
    /*
    * DefaultIDCheck checks if the ID is the default Dolphin ID.
    */
    __attribute__((__section__(".id_check"))) void DefaultIDCheck() {
      ESInit();

      int console_id{};
      ES_GetDeviceId(&console_id);
      if (console_id == DEFAULT_DOLPHIN_ID)
        PPCHalt();
    }
}

