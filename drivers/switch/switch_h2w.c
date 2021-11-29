/* drivers/switch/switch_h2w.c
 *
 * Headset detection driver
 *
 * Copyright (C) 2016 The Ubports project
 * Author: Marius Gripsgard <mariogrip@ubports.com>
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

/* Interface is following:
 * HEADSET_UEVENT_MATCH = "DEVPATH=/sys/devices/virtual/switch/h2w"
 * HEADSET_STATE_PATH = /sys/class/switch/h2w/state
 * HEADSET_NAME_PATH = /sys/class/switch/h2w/name
 */
#include <linux/module.h>
#include <linux/switch.h>
#include <linux/slab.h>
#include <linux/switch_h2w.h>
#include <sound/jack.h>


enum {
	NO_DEVICE   = 0,
	HEADSET = (1 << 0),
	HEADSET_NO_MIC = (1 << 1),
};

static ssize_t print_name(struct switch_dev *sdev, char *buf)
{
	switch (switch_get_state(sdev)) {
		case NO_DEVICE:
			return sprintf(buf, "No Device");
		case HEADSET:
			return sprintf(buf, "Headset");
		case HEADSET_NO_MIC:
			return sprintf(buf, "Headset no mic");
		default:
			break;
	}
	return -EINVAL;
}

static ssize_t print_state(struct switch_dev *sdev, char *buf)
{
	return sprintf(buf, "%d\n", switch_get_state(sdev));
}

void switch_h2w_report(struct h2w_info *h, int state){
	printk(KERN_DEBUG "Switch_h2w: new report state: %d\n", state);
	switch (state) {
		case SND_JACK_HEADPHONE:
			h->state = HEADSET_NO_MIC;
			break;
		case SND_JACK_HEADSET:
		case 11:
			h->state = HEADSET;
			break;
		default:
			h->state = NO_DEVICE;
			break;
	}
	switch_set_state(&h->sdev, h->state);
}
EXPORT_SYMBOL(switch_h2w_report);

struct h2w_info *switch_h2w_proble(void){
	struct h2w_info *h;
	int ret;

	printk(KERN_DEBUG "Switch_h2w: Init\n");

	h = kzalloc(sizeof(struct h2w_info), GFP_KERNEL);

	/* initialize switch device */
	h->sdev.name = "h2w";
	h->sdev.print_state = print_state;
	h->sdev.print_name = print_name;

	h->state = 0;

	ret = switch_dev_register(&h->sdev);
	if (ret < 0)
		printk(KERN_ERR "%s: Failed to register switch device\n", __func__);

	return h;
}
EXPORT_SYMBOL(switch_h2w_proble);

MODULE_AUTHOR("Marius Gripsgard <mariogrip@ubports.com>");
MODULE_DESCRIPTION("Switch h2w Headset detection driver");
MODULE_LICENSE("GPL");
