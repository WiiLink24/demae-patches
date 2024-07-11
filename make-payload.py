import time, os, subprocess
from sys import argv

devkitppc = os.environ.get('DEVKITPPC')
path_cc = os.path.join(devkitppc, "bin", "powerpc-eabi-gcc")
path_objcopy = os.path.join(devkitppc, "bin", "powerpc-eabi-objcopy")
path_as = os.path.join(devkitppc, "bin", "powerpc-eabi-as")
path_bin2s = "bin2s"
path_cert = "wiilinkca.pub"

extra_build_flags = []


def compile_ssl_cert():
    with open('cert.s', 'w') as f:
        subprocess.run([path_bin2s, "-H", "cert.h", path_cert], stdout=f).check_returncode()

    subprocess.run([path_as, "-o", "cert.o", "cert.s"]).check_returncode()


def build():
    print("Building payload...")

    flags = ["-g", "-Os", "-fPIE", "-std=c++20", '-Wall', '-Werror', "-Wsuggest-override", "-n", "-fno-rtti",
              "-fno-exceptions", "-fno-sized-deallocation", "-ffunction-sections", "-fdata-sections", "-fshort-wchar",
              "-Wl,--gc-sections", "-Wno-address-of-packed-member", "-DDEBUG"]
    flags += extra_build_flags

    out_path = os.path.join("build", "demae")
    binary_path = os.path.join("binary", "payload." + "demae" + ".bin")
    subprocess.run([path_cc, "-o" + out_path + ".elf", "demae.cpp", "cert.o", "-Tpayload.ld",
                    "-I.", "-Wl,--defsym=ORIGIN_ADDRESS=0x80001800"] + flags).check_returncode()
    subprocess.run([path_objcopy, out_path + ".elf", binary_path, "-O", "binary"]).check_returncode()

    with open("binary/00000001.app", "rb") as original:
        data = original.read()
    with open("binary/00000001_PATCH.app", "wb") as new:
        with open(binary_path, "rb") as patch:
            new.write(data)
            new.write(patch.read())
            new.write(b'\x00' * (6144 - os.stat(binary_path).st_size))

    subprocess.run(["binary/DolTranslator", "binary/00000001_PATCH.app", "binary/binary-en.json", "0x804758b4"]).check_returncode()


if __name__ == "__main__":
    try:
        os.mkdir("build")
    except:
        pass

    try:
        os.mkdir("binary")
    except:
        pass

    compile_ssl_cert()
    build()
