#ifndef __CONFIG_R8Q_H
#define __CONFIG_R8Q_H

#include <linux/sizes.h>

/*
 * 1. MEMORY LAYOUT
 * The Snapdragon 865 (SM8250) DRAM starts at 0x80000000.
 * We place the initial stack pointer at the very bottom of RAM.
 */
#define CFG_SYS_INIT_RAM_ADDR       0x80000000
#define CFG_SYS_INIT_RAM_SIZE       0x2000

/*
 * 2. GENERIC SETTINGS
 */
#define CFG_SYS_BOOTM_LEN           SZ_64M  /* Allow large kernel images */
#define CFG_SYS_BAUDRATE_TABLE      { 115200, 230400, 460800, 921600 }

/*
 * 3. DEFAULT ENVIRONMENT
 * We set up the load addresses to avoid the Framebuffer (0x9c000000).
 * - Kernel Load: 0x80008000 (Standard)
 * - FDT (DTB) Load: 0x84000000 (Safe gap)
 * - Ramdisk Load: 0x86000000 (Safe gap)
 */
#define CFG_EXTRA_ENV_SETTINGS \
    "loadaddr=0x80008000\0" \
    "fdtaddr=0x84000000\0" \
    "fdtfile=r8q.dtb\0" \
    "console=ttyMSM0,115200n8\0" \
    "bootargs=console=ttyMSM0,115200n8 earlycon=msm_geni_serial,0x880000 androidboot.hardware=qcom\0"

#endif

