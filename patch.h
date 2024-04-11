#pragma once

#include <util.h>

extern "C" {
typedef struct  {
    u32 address;

    union {
        u32 arg;
    };

} PACKED patch;
}

namespace demae::Patch {

void ApplyPatch(patch& patch);

constexpr patch MakePatch(u32 address, auto function) {
  return patch{
    .address = address,
    .arg = u32(+function)
  };
}

#define _DEMAE_DEFINE_PATCH2(NUM)                                               \
    __attribute__((__section__(".demae_patch"))) patch __demae_patch_##NUM[]

#define _DEMAE_DEFINE_PATCH1(NUM) _DEMAE_DEFINE_PATCH2(NUM)
#define DEMAE_DEFINE_PATCH _DEMAE_DEFINE_PATCH1(__COUNTER__)
}