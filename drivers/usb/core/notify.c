/*
 * All the USB notify logic
 *
 * (C) Copyright 2005 Greg Kroah-Hartman <gregkh@suse.de>
 *
 * notifier functions originally based on those in kernel/sys.c
 * but fixed up to not be so broken.
 *
 */


#include <linux/kernel.h>
#include <linux/export.h>
#include <linux/notifier.h>
#include <linux/usb.h>
#include <linux/mutex.h>
#include "usb.h"

static BLOCKING_NOTIFIER_HEAD(usb_notifier_list);

/**
 * usb_register_notify - register a notifier callback whenever a usb change happens
 * @nb: pointer to the notifier block for the callback events.
 *
 * These changes are either USB devices or busses being added or removed.
 */
void usb_register_notify(struct notifier_block *nb)
{
	blocking_notifier_chain_register(&usb_notifier_list, nb);
}
EXPORT_SYMBOL_GPL(usb_register_notify);

/**
 * usb_unregister_notify - unregister a notifier callback
 * @nb: pointer to the notifier block for the callback events.
 *
 * usb_register_notify() must have been previously called for this function
 * to work properly.
 */
void usb_unregister_notify(struct notifier_block *nb)
{
	blocking_notifier_chain_unregister(&usb_notifier_list, nb);
}
EXPORT_SYMBOL_GPL(usb_unregister_notify);


void usb_notify_add_device(struct usb_device *udev)
{
	blocking_notifier_call_chain(&usb_notifier_list, USB_DEVICE_ADD, udev);
}

void usb_notify_remove_device(struct usb_device *udev)
{
	/* Protect against simultaneous usbfs open */
	mutex_lock(&usbfs_mutex);
	blocking_notifier_call_chain(&usb_notifier_list,
			USB_DEVICE_REMOVE, udev);
	mutex_unlock(&usbfs_mutex);
}

void usb_notify_add_bus(struct usb_bus *ubus)
{
	blocking_notifier_call_chain(&usb_notifier_list, USB_BUS_ADD, ubus);
}

void usb_notify_remove_bus(struct usb_bus *ubus)
{
	blocking_notifier_call_chain(&usb_notifier_list, USB_BUS_REMOVE, ubus);
}

void usb_notify_warning(struct usb_device *udev, int warning_code)
{
	char *notsupport[2] = { "USB_WARNING=DEVICE_NOT_SUPPORT", NULL };
	char *notrespond[2] = { "USB_WARNING=DEVICE_NOT_RESPONDING", NULL };
	char *vbusinvalid[2] = { "USB_WARNING=VBUS_INVALID", NULL };
	char *hubmaxtier[2] = { "USB_WARNING=HUB_MAX_TIER", NULL };
	char *insuffpower[2] = { "USB_WARNING=INSUFF_POWER", NULL };
	char **uevent_envp = NULL;

	switch (warning_code) {
	case USB_WARNING_NOT_SUPPORT:
		uevent_envp = notsupport;
		break;
	case USB_WARNING_NO_RESPONSE:
		uevent_envp = notrespond;
		break;
	case USB_WARNING_VBUS_INVALID:
		uevent_envp = vbusinvalid;
		break;
	case USB_WARNING_HUB_MAX_TIER:
		uevent_envp = hubmaxtier;
		break;
	case USB_WARNING_INSUFF_POWER:
		uevent_envp = insuffpower;
		break;
	default:
		return;
	}

	kobject_uevent_env(&udev->dev.kobj, KOBJ_CHANGE, uevent_envp);
}
