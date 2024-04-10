#include <default_id.h>

namespace demae {
    /*
    * DefaultIDCheck checks if the ID is the default Dolphin ID.
    */
    SECTION(".id_check") void DefaultIDCheck() {
      ESInit();

      u32 console_id{};
      ES_GetDeviceId(&console_id);
      if (console_id == DEFAULT_DOLPHIN_ID)
        PPCHalt();
    }
}

