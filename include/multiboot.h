/*
 * https://www.gnu.org/software/grub/manual/multiboot/multiboot.html#Header-layout
 *
 * Check bootstarm.[s|asm] for how we defined the structure
 */

#ifndef __MULTIBOOT_H__
#define __MULTIBOOT_H__
#include <types.h>

#define MULTIBOOT_SEARCH_SIZE	(8192)	/* How many bytes from the start of
										 * the file we search for the header.
										 */

#define MULTIBOOT_HEADER_MAGIC		(0x1BADB002)	/* The magic field should contain this. */
#define MULTIBOOT_BOOTLOADER_MAGIC	(0x2BADB002)	/* This should be in %eax. */

#define MULTIBOOT_UNSUPPORTED	(0x0000fffc)	/* The bits in the required part of
												 * flags field we don't support.
												 */
#define MULTIBOOT_MOD_ALIGN		(0x00001000)	/* Alignment of multiboot modules. */
#define MULTIBOOT_INFO_ALIGN	(0x00000004)	/* Alignment of the multiboot info structure. */

/*
 * Flags set in the 'flags' member of the multiboot header.
 */    
#define MULTIBOOT_PAGE_ALIGN	(0x00000001)	/* Align all boot modules on i386 page (4KB)
												 * boundaries.
												 */
#define MULTIBOOT_MEMORY_INFO	(0x00000002)	/* Must pass memory information to OS. */
#define MULTIBOOT_VIDEO_MODE	(0x00000004)	/* Must pass video information to OS. */
#define MULTIBOOT_AOUT_KLUDGE	(0x00010000)	/* This flag indicates the use
												 * of the address fields in the header.
												 */

/*
 * Flags to be set in the 'flags' member of the multiboot info structure.
 */
#define MULTIBOOT_INFO_MEMORY			(0x00000001)	/* is there basic lower/upper memory information? */
#define MULTIBOOT_INFO_BOOTDEV			(0x00000002)	/* is there a boot device set? */
#define MULTIBOOT_INFO_CMDLINE			(0x00000004)	/* is the command-line defined? */
#define MULTIBOOT_INFO_MODS				(0x00000008)	/* are there modules to do something with? */

#define MULTIBOOT_INFO_AOUT_SYMS		(0x00000010)	/* is there a symbol table loaded? */
#define MULTIBOOT_INFO_ELF_SHDR			(0X00000020)	/* is there an ELF section header table? */

#define MULTIBOOT_INFO_MEM_MAP			(0x00000040)	/* is there a full memory map? */
#define MULTIBOOT_INFO_DRIVE_INFO		(0x00000080)	/* Is there drive info? */
#define MULTIBOOT_INFO_CONFIG_TABLE		(0x00000100)	/* Is there a config table? */
#define MULTIBOOT_INFO_BOOT_LOADER_NAME	(0x00000200)	/* Is there a boot loader name? */
#define MULTIBOOT_INFO_APM_TABLE		(0x00000400)	/* Is there a APM table? BIOS Interface
														 * (Advanced Power Managemnet)
														 */
#define MULTIBOOT_INFO_VIDEO_INFO		(0x00000800)	/* Is there video information? */


typedef struct multiboot_header {
	/*
	 * Must items for Multiboot Headers
	 */
	uint32_t	magic;		/* required */
	uint32_t	flags;		/* required */
	uint32_t	checksum;	/* required */

	/*
	 * Optional Headers depends on flags bits
	 */
	uint32_t	header_addr;	/* If MULTIBOOT_AOUT_KLUDGE is set */
	uint32_t	load_addr; 		/* If MULTIBOOT_AOUT_KLUDGE is set */
	uint32_t	load_end_addr;	/* If MULTIBOOT_AOUT_KLUDGE is set */
	uint32_t	bss_end_addr;	/* If MULTIBOOT_AOUT_KLUDGE is set */
	uint32_t	entry_addr;		/* If MULTIBOOT_AOUT_KLUDGE is set */

	uint32_t	mode_type;		/* If MULTIBOOT_VIDEO_MODE is set */
	uint32_t	width;			/* If MULTIBOOT_VIDEO_MODE is set */
	uint32_t	height;			/* If MULTIBOOT_VIDEO_MODE is set */
	uint32_t	depth;			/* If MULTIBOOT_VIDEO_MODE is set */
} multiboot_header_t;

/*
 * The symbol table for a.out.
*/
typedef struct multiboot_aout_symbol_table {
	uint32_t tabsize;
	uint32_t strsize;
	uint32_t addr;
	uint32_t reserved;
} multiboot_aout_symbol_table_t;

/* The section header table for ELF. */
typedef struct multiboot_elf_section_header_table {
	uint32_t num;
	uint32_t size;
	uint32_t addr;
	uint32_t shndx;
} multiboot_elf_section_header_table_t;

typedef struct multiboot_info {
	uint32_t flags;				/* Multiboot info version number */

	uint32_t mem_lower;			/* Available memory from BIOS */
	uint32_t mem_upper;

	uint32_t boot_device;		/* "root" partition */
	uint32_t cmdline;			/* Kernel command line */

	uint32_t mods_count;		/* Boot-Module list */
	uint32_t mods_addr;

	union {
		multiboot_aout_symbol_table_t aout_sym;
		multiboot_elf_section_header_table_t elf_sec;
	} u;

	uint32_t mmap_length;		/* Memory Mapping buffer */
	uint32_t mmap_addr;

	uint32_t drives_length;		/* Drive Info buffer */
	uint32_t drives_addr;

	uint32_t config_table;		/* ROM configuration table */
	uint32_t boot_loader_name;	/* Boot Loader Name */
	uint32_t apm_table;			/* APM table */

	uint32_t vbe_control_info;	/* Video */
	uint32_t vbe_mode_info;
	uint16_t vbe_mode;
	uint16_t vbe_interface_seg;
	uint16_t vbe_interface_off;
	uint16_t vbe_interface_len;
} multiboot_info_t;

typedef struct multiboot_mmap_entry
{
	uint32_t size;
	uint64_t addr;
	uint64_t len;
#define MULTIBOOT_MEMORY_AVAILABLE	(1)
#define MULTIBOOT_MEMORY_RESERVED	(2)
	uint32_t type;
} __attribute__((packed)) multiboot_memory_map_t;

typedef struct multiboot_mod_list {
	/* the memory used goes from bytes 'mod_start'
     * to 'mod_end-1' inclusive */
	uint32_t mod_start;
	uint32_t mod_end;

	uint32_t cmdline;	/* Module command line */
	uint32_t pad;		/* padding to take it to 16 bytes (must be zero) */
} multiboot_module_t;

#endif /* __MULTIBOOT_H__ */
