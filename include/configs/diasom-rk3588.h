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


#define ENV_BOOTARGS_SETTINGS		\
	"bootargs=root=PARTLABEL=rootfs earlycon rootwait\0"

#define CFG_EXTRA_ENV_SETTINGS \
	ENV_MEM_LAYOUT_SETTINGS		\
	ENV_BOOTARGS_SETTINGS \
	"bootfile=Image\0" \
	"bootpart=2\0" \
	"loadkernel=load mmc ${devnum}:${bootpart} ${kernel_addr_r} /boot/${bootfile}\0" \
	"loadfdt=load mmc ${devnum}:${bootpart} ${fdt_addr_r} /boot/${fdtfile}\0" \
	"loadoverlay=load mmc ${devnum}:${bootpart} ${fdtoverlay_addr_r} /boot/${dtoverlay}\0" \
	"apply_overlay=" \
		"if test -n \"${dtoverlay}\"; then " \
			"echo Applying overlay ${dtoverlay}; " \
			"run loadoverlay; " \
			"fdt addr ${fdt_addr_r}; " \
			"fdt resize 8192; " \
			"fdt apply ${fdtoverlay_addr_r}; " \
		"fi\0" \
	"myboot=run loadkernel; run loadfdt; run apply_overlay; booti ${kernel_addr_r} - ${fdt_addr_r}\0"

#define CONFIG_BOOTCOMMAND "run myboot"
#endif /* __DIASOM_RK3588_H */
