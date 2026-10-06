/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright (c) 2022 Collabora Ltd.
 */

#ifndef __DIASOM_RK3588_H
#define __DIASOM_RK3588_H

#define ROCKCHIP_DEVICE_SETTINGS \
		"stdout=serial,vidconsole\0" \
		"stderr=serial,vidconsole\0"

#include "rockchip-common.h"

#define CFG_IRAM_BASE			0xff000000

#define CFG_SYS_SDRAM_BASE		0
#define SDRAM_MAX_SIZE			0xf0000000

#ifndef ROCKCHIP_DEVICE_SETTINGS
#define ROCKCHIP_DEVICE_SETTINGS
#endif

#define ENV_MEM_LAYOUT_SETTINGS		\
	"scriptaddr=0x00c00000\0"	\
	"script_offset_f=0xffe000\0"	\
	"script_size_f=0x2000\0"	\
	"pxefile_addr_r=0x00e00000\0"	\
	"kernel_addr_r=0x02000000\0"	\
	"kernel_comp_addr_r=0x0a000000\0"	\
	"fdt_addr_r=0x12000000\0"	\
	"fdtoverlay_addr_r=0x12100000\0"	\
	"ramdisk_addr_r=0x12180000\0"	\
	"kernel_comp_size=0x8000000\0"


#define ENV_BOOTARGS_SETTINGS \
    "bootargs_base=earlycon rootwait drm.edid_firmware=HDMI-A-1:edid/display-edid.bin video=HDMI-A-1:1920x1080@60e\0"

/*
 * Boot media. U-Boot numbering follows the DTS aliases (mmc0 = &sdhci = eMMC,
 * mmc1 = &sdmmc = SD). Linux root device names are NOT derived from U-Boot
 * numbers: they depend on the aliases of the Linux DTB and are set here
 * explicitly per medium.
 */
#define ENV_BOOTMEDIA_SETTINGS \
	"emmc_devnum=0\0" \
	"emmc_bootpart=2\0" \
	"emmc_rootdev=/dev/mmcblk0p2\0" \
	"sd_devnum=1\0" \
	"sd_bootpart=2\0" \
	"sd_rootdev=/dev/mmcblk1p2\0" \
	"nvme_devnum=0\0" \
	"nvme_bootpart=2\0" \
	"nvme_rootdev=/dev/nvme0n1p2\0" \
	"bootorder_spi=nvme emmc sd\0" \
	"set_emmc=setenv devtype mmc; setenv devnum ${emmc_devnum}; " \
		"setenv bootpart ${emmc_bootpart}; setenv rootdev ${emmc_rootdev}\0" \
	"set_sd=setenv devtype mmc; setenv devnum ${sd_devnum}; " \
		"setenv bootpart ${sd_bootpart}; setenv rootdev ${sd_rootdev}\0" \
	"set_nvme=setenv devtype nvme; setenv devnum ${nvme_devnum}; " \
		"setenv bootpart ${nvme_bootpart}; setenv rootdev ${nvme_rootdev}\0" \
	"prep_mmc=mmc dev ${devnum}\0" \
	"prep_nvme=pci enum && nvme scan && nvme dev ${devnum}\0"

#define CFG_EXTRA_ENV_SETTINGS \
	ENV_MEM_LAYOUT_SETTINGS		\
	ENV_BOOTARGS_SETTINGS \
	ENV_BOOTMEDIA_SETTINGS \
	"bootfile=Image\0" \
	"devtype=mmc\0" \
	"bootpart=2\0" \
	"rootpart=2\0" \
	"loadkernel=load ${devtype} ${devnum}:${bootpart} ${kernel_addr_r} /boot/${bootfile}\0" \
	"loadfdt=load ${devtype} ${devnum}:${bootpart} ${fdt_addr_r} /boot/${fdtfile}\0" \
	"loadoverlay=load ${devtype} ${devnum}:${bootpart} ${fdtoverlay_addr_r} /boot/${dtoverlay}\0" \
	"setroot=test -n \"${rootdev}\" && setenv bootargs \"root=${rootdev} ${bootargs_base}\"\0" \
	"apply_overlay=" \
		"if test -z \"${dtoverlay}\"; then " \
			"true; " \
		"else " \
			"echo Applying overlay ${dtoverlay}; " \
			"run loadoverlay && " \
			"fdt addr ${fdt_addr_r} && " \
			"fdt resize 8192 && " \
			"fdt apply ${fdtoverlay_addr_r}; " \
		"fi\0" \
	"boot_target=" \
		"run set_${target} && " \
		"test -n \"${devnum}\" && test -n \"${bootpart}\" && " \
		"echo \"## Trying ${target}: ${devtype} ${devnum}:${bootpart} root=${rootdev}\" && " \
		"run prep_${devtype} && " \
		"run loadkernel && " \
		"run loadfdt && " \
		"run apply_overlay && " \
		"run setroot && " \
		"booti ${kernel_addr_r} - ${fdt_addr_r}\0" \
	"boot_search=" \
		"for target in ${bootorder}; do " \
			"run boot_target || echo \"## ${target}: boot failed\"; " \
		"done; " \
		"echo \"## ERROR: no bootable media (tried: ${bootorder}), staying in console\"; " \
		"false\0" \
	"myboot=" \
		"if test \"${boot_source}\" = emmc; then " \
			"setenv bootorder emmc; " \
		"elif test \"${boot_source}\" = sd; then " \
			"setenv bootorder sd; " \
		"else " \
			"setenv bootorder ${bootorder_spi}; " \
		"fi; " \
		"if test -n \"${bootorder}\"; then " \
			"echo \"## Boot source: ${boot_source}, candidates: ${bootorder}\"; " \
			"run boot_search; " \
		"else " \
			"echo \"## ERROR: unknown boot source or empty boot order\"; " \
			"false; " \
		"fi\0"

#define CONFIG_BOOTCOMMAND "run myboot"
#endif /* __DIASOM_RK3588_H */
