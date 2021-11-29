/* include/linux/switch_h2w.h
 *
 * Headset detection driver
 *
 * Copyright (C) 2016 The Ubports project
 * Author: Marius Gripsgard <mariogrip@ubuntu.com>
 *
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#ifndef __SWITCH_H2W_H_
#define __SWITCH_H2W_H_

#include <linux/switch.h>

struct h2w_info {
	struct switch_dev sdev;
	int state;
	int fd;
};

extern void switch_h2w_report(struct h2w_info *h, int state);
extern struct h2w_info *switch_h2w_proble(void);

#endif
