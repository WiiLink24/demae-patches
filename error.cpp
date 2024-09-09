#include <patch.h>
#include <string>
#include <cstdlib.h>
#include <rvl.h>
#include <asm.h>

namespace demae::Error {
    constexpr u32 ERROR_STRING_ADDRESS = 0x800014a0;

    /*
     * CopyErrorString copies the error string from the XML is available.
     * This function is inserted right before the ClearXMLObject is called in order to preserve the string.
     */
    void CopyErrorString(int _xml_object) {
      LONGCALL void* GetNode(void* buf, const char* name, void* parent) AT(0x800c43e0);
      LONGCALL void* GetChildNodeAtIndex(void* parent, int index) AT(0x800c4afc);
      LONGCALL char* GetNodeValue(void* node) AT(0x800c4870);
      LONGCALL void ClearXMLObject(int xml_object) AT(0x800c4a30);
      LONGCALL void* GetNodeParent(void* node) AT(0x800c4b28);

      u32 xml_object = *reinterpret_cast<u32*>(0x8038a440);
      // GetNode(xml_object, "response", nullptr);
      void* parent = GetNode(reinterpret_cast<void*>(xml_object), reinterpret_cast<const char*>(0x802d65b0), nullptr);
      if (parent == nullptr)
        return ClearXMLObject(_xml_object);

      void* child = GetChildNodeAtIndex(parent, 0);
      if (child == nullptr)
        return ClearXMLObject(_xml_object);

      void* txt_node = GetNode(reinterpret_cast<void*>(xml_object), "error", child);
      if (txt_node == nullptr)
        return ClearXMLObject(_xml_object);

      void* txt_node_parent = GetNodeParent(txt_node);
      if (txt_node_parent == nullptr)
        return ClearXMLObject(_xml_object);

      char* txt = GetNodeValue(txt_node_parent);
      u32 size = cstdlib::strlen(txt);
      cstdlib::memcpy(reinterpret_cast<void*>(ERROR_STRING_ADDRESS), txt, size);

      ClearXMLObject(_xml_object);
    }

    void RenderCustomError(int* gx_obj, int err) {
      LONGCALL void RenderErrorBox(int* gx_obj, const wchar_t* text, int zero) AT(0x80285bd4);
      LONGCALL void PrepErrorBox(int* gx_obj, int err) AT(0x8001aca8);
      LONGCALL void SomeGXInit(int* gx_obj, int val) AT(0x8001adc0);
      LONGCALL void SendToGX(int* gx_obj) AT(0x8001b4a0);

      // See if we have a custom error first
      char* txt = reinterpret_cast<char *>(ERROR_STRING_ADDRESS);
      u32 size = cstdlib::strlen(txt);
      if (size == 0)
        // Default to the embedded string
        return PrepErrorBox(gx_obj, err);

      // Init the GX object
      int uVar5 = reinterpret_cast<int*>(0x8034496c)[err * 3];
      int uVar3 = reinterpret_cast<int*>(0x80344968)[err * 3];

      gx_obj[1] = 1;
      *gx_obj = 0;
      gx_obj[0x5a] = *reinterpret_cast<int*>(0x804724d4);
      gx_obj[2] = uVar5;
      gx_obj[4] = 0;
      gx_obj[0x5c] = -1;

      SomeGXInit(gx_obj, uVar3);
      // Convert to UTF16
      void *dst = cstdlib::malloc((size + 1) * 2);
      ShiftJISToUTF(dst, &size, static_cast<char *>(txt), &size);
      *(reinterpret_cast<u16 *>(dst) + size) = L'\0';

      // Now send to the renderer object
      RenderErrorBox(reinterpret_cast<int *>(gx_obj[0xa0]), reinterpret_cast<const wchar_t*>(dst), 0);

      // Clear the string so if another error occurs in which we don't send a string, it will render what's in the DOL.
      cstdlib::memset(txt, 0, size);

      SendToGX(gx_obj);
    }

    DEMAE_DEFINE_PATCH = {
            Patch::WriteFunctionCall(0x800ab0d4, RenderCustomError),
            Patch::WriteFunctionCall(0x800eedc0, RenderCustomError),
            Patch::WriteFunctionCall(0x800c3b60, CopyErrorString),
    };
}