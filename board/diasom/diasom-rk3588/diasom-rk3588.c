// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright (c) 2023-2024 Collabora Ltd.
 */

#include <fdtdec.h>
#include <fdt_support.h>
#include <asm-generic/gpio.h>
#include <env.h>
#include <i2c.h>
#include <stdlib.h>

#define BOARD_NAME "Diasom RK3588"

static int rk3588_detect_gpio_expandex(void)
{
	int ret;
	struct udevice *dev;

	int gpio_num = 125;
	int bus_num = 7;
	ret = gpio_request(gpio_num, "gpio-reset-expander");
	if (ret) {
		printf("%s: Cannot request from gpio %d", BOARD_NAME, gpio_num);
		return 1;
	}
	gpio_direction_output(gpio_num, 1);
	if (ret) {
		printf("%s: Cannot set direction to gpio %d", BOARD_NAME, gpio_num);
		return 1;
	}
	gpio_set_value(gpio_num, 1);
	if (ret) {
		printf("%s: Cannot set high value to gpio %d", BOARD_NAME, gpio_num);
		return 1;
	}

	ret = i2c_get_chip_for_busnum(bus_num, 0x50, 1, &dev);
	if (ret) {
		printf("%s: Cannot find udev for a bus %d\n", BOARD_NAME, bus_num);
		return 1;
	}

	printf("%s: Find udev for a bus %d\n", BOARD_NAME, bus_num);
	return 0;
}


int rk_board_late_init(void)
{
	int ret;
	ret = rk3588_detect_gpio_expandex();

	if(ret) {
		env_set("variant", "-btb-evb");
		env_set("fdt_board", "-v1");
		printf("%s: Detect version 1\n", BOARD_NAME);
	} else {
		env_set("variant", "-btb-evb");
		env_set("fdt_board", "");
		printf("%s: Detect version 2\n", BOARD_NAME);
	}
	return 0;
}


#ifdef CONFIG_OF_BOARD_SETUP
int ft_board_setup(void *blob, struct bd_info *bd)
{
	if (IS_ENABLED(CONFIG_TYPEC_FUSB302))
		fdt_status_okay_by_compatible(blob, "fcs,fusb302");
	return 0;
}
#endif
