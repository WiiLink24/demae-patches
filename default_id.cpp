#include <default_id.h>
#include <rvl.h>
#include <patch.h>
#include <setting.h>

namespace demae {
    constexpr const char* error_message[7] = {
            // Japanese
            "Japan",

            // English
            "Please use a NAND dump from a real console.\nFor instructions on how to dump your NAND, visit \nhttps://wii.hacks.guide/bootmii.",

            // German
            "German",

            // French
            "c'est ne pas bon",

            // Spanish
            "Spanish",

            // Italian
            "Italy",

            // Dutch
            "Dutch"
    };

    /*
    * DefaultIDCheck checks if the ID is the default Dolphin ID.
    */
    void DefaultIDCheck() {
      ESInit();

      u32 console_id{};
      ES_GetDeviceId(&console_id);
      if (console_id == DEFAULT_DOLPHIN_ID)
      {
        u8 language_code = sc::GetLanguage();
        RVL::OSWritePanic(reinterpret_cast<void*>(0x804725a0), reinterpret_cast<void*>(0x80474580), error_message[language_code]);
      }
    }

    DEMAE_DEFINE_PATCH = {
            Patch::MakePatch(0x80233298, DefaultIDCheck)
    };
}

