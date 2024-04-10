OUTPUT_FORMAT("elf32-powerpc", "elf32-powerpc", "elf32-powerpc");
OUTPUT_ARCH(powerpc:common);
ENTRY(None)

MEMORY
{
	code (rwx) : ORIGIN = ORIGIN_ADDRESS, LENGTH = 0x01800000
}

SECTIONS
{
	.text : {
        . = ALIGN(4);
        _G_IDCheckStart = .;
        KEEP(*(.id_check))
        _G_IDCheckEnd = .;

        _G_WiiIDHttpStart = .;
        KEEP(*(.wii_id_http))
        _G_WiiIDHttpEnd = .;

        . = ALIGN(4);
		*(.text*)
		*(.rodata*)
		*(.data*)
		*(.bss*)
		*(.sdata*)
		*(.sbss*)
		_G_End = .;
	} > code

	/DISCARD/ : {
		*(.comment*)
		*(.eh_frame*)
		*(.eh_frame_hdr*)
	}
}