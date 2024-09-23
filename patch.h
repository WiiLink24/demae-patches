#pragma once

#include <util.h>
#include <asm.h>

extern "C" {
enum PatchType { FUNCTION_CALL, WRITE, WRITE_U32, BRANCH_CTR_LINK };

typedef struct {
  u32 address;
  PatchType patch_type;

  union {
    u32 arg0;
    const void *arg0p;
    const u32 *arg0p32;
  };

  u32 arg1;
} PACKED patch;
}

namespace demae::Patch {

void ApplyPatch(patch &patch);

constexpr patch WriteFunctionCall(u32 address, auto function) {
  return patch{
      .address = address, .patch_type = FUNCTION_CALL, .arg0 = u32(+function)};
}

template <u32 N>
constexpr patch WriteString(u32 address, const char (&string)[N]) {
  return patch{.address = address,
               .patch_type = WRITE,
               .arg0p = string,
               .arg1 = sizeof(string)};
}

constexpr patch WriteASM(u32 address, u32 instruction_count, auto function) {
  return patch{
      .address = address,
      .patch_type = WRITE,
      .arg0 = u32(+function),
      .arg1 = instruction_count * sizeof(u32),
  };
}

constexpr patch WriteU32(u32 address, u32 value) {
  return patch{.address = address,
               .patch_type = WRITE_U32,
               .arg0 = value,
               .arg1 = sizeof(u32)};
}

constexpr patch CallWithCTR(u32 address, auto function, u32 tempReg = r12) {
      return patch {
              .address = address,
              .patch_type = BRANCH_CTR_LINK,
              .arg0 = u32(+function),
              .arg1 = tempReg,
      };
}

#define _DEMAE_DEFINE_PATCH2(NUM)                                              \
  __attribute__((__section__(".demae_patch"))) patch __demae_patch_##NUM[]

#define _DEMAE_DEFINE_PATCH1(NUM) _DEMAE_DEFINE_PATCH2(NUM)
#define DEMAE_DEFINE_PATCH _DEMAE_DEFINE_PATCH1(__COUNTER__)
} // namespace demae::Patch