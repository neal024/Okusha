/* OUTPUT_FORMAT(elf32-i386) */
OUTPUT_FORMAT("binary") /* Set KLUDGE Flag in Multi-Boot Header */
OUTPUT_ARCH(i386:i386)

ENTRY(__start)
phys = 0x00100000;
SECTIONS
{
  .text phys : AT(phys) {
    code = .;
    *(.text)
    *(.rodata*)
    . = ALIGN(4096);
  }
  .data : AT(phys + (data - code))
  {
    data = .;
    *(.data)
    . = ALIGN(4096);
  }
  .bss : AT(phys + (bss - code))
  {
    bss = .;
    *(.bss)
    . = ALIGN(4096);
  }
  end = .;
}
