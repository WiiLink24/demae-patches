import time, os, subprocess
from sys import argv

devkitppc = os.environ.get('DEVKITPPC')
path_cc = os.path.join(devkitppc, "bin", "powerpc-eabi-gcc")
path_objcopy = os.path.join(devkitppc, "bin", "powerpc-eabi-objcopy")
path_as = os.path.join(devkitppc, "bin", "powerpc-eabi-as")
path_bin2s = "bin2s"
path_cert = "wiilinkca.pub"

extra_build_flags = []

editions = ["REGULAR", "DOMINOS", "EATER"]
dev = True
beta = False


def compile_ssl_cert():
    with open('cert.s', 'w') as f:
        subprocess.run([path_bin2s, "-H", "cert.h", path_cert], stdout=f).check_returncode()

    subprocess.run([path_as, "-o", "cert.o", "cert.s"]).check_returncode()


def build(_edition):
    print("Building payload...")

    flags = ["-g", "-Os", "-fPIE", "-std=c++20", '-Wall', '-Werror', "-Wsuggest-override", "-n", "-fno-rtti",
              "-fno-exceptions", "-fno-sized-deallocation", "-ffunction-sections", "-fdata-sections", "-fshort-wchar",
              "-Wl,--gc-sections", "-Wno-address-of-packed-member", f"-D{_edition}", "-Wno-unused-variable"]
    flags += extra_build_flags

    # Prioritize development
    if dev:
        flags.append("-DDEV")
    elif beta:
        flags.append("-DBETA")

    out_path = os.path.join("build", f"demae_{_edition}")
    binary_path = os.path.join("binary", f"payload.demae_{_edition}.bin")
    subprocess.run([path_cc, "-o" + out_path + ".elf", "demae.cpp", "cert.o", "-Tpayload.ld",
                    "-I.", "-Wl,--defsym=ORIGIN_ADDRESS=0x80001800"] + flags).check_returncode()
    subprocess.run([path_objcopy, out_path + ".elf", binary_path, "-O", "binary"]).check_returncode()

    with open("binary/00000001.app", "rb") as original:
        data = original.read()
    with open(f"binary/00000001_{_edition}.app", "wb") as new:
        with open(binary_path, "rb") as patch:
            new.write(data)
            new.write(patch.read())
            new.write(b'\x00' * (6144 - os.stat(binary_path).st_size))

            if patch.tell() > 0x1800:
                print(f"WARNING: Payload larger than allowed size: {patch.tell()}\nVersion: {_edition}")

    (subprocess.run(["binary/DolTranslator", f"binary/00000001_{_edition}.app", "binary/binary-en.json", "0x80475d80"])
     .check_returncode())
    os.rename("translated.dol", f"binary/00000001_{_edition}.app")


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

    for edition in editions:
        build(edition)
