/*
//
// Created by jin on 12.04.19.
//

#include "libv4l2-hw.h"
#include "usbinfo.h"
#include <libudev.h>
#include <libusb-1.0/libusb.h>
#include "usbinfo.h"
#include "libv4l2-hw.h"

int v4l2_init(int fd, struct v4l2hw *info) {
    struct stat sinfo;
    if (fstat(fd, &sinfo) == -1) {
        ERROR_LOG("fstat: %s", strerror(errno));
        return -1;
    }
    memset((char*)info, 0, sizeof (struct v4l2hw));
    info->fd = fd;
    info->rdev = sinfo.st_rdev;
    info->type = NONE;

    struct udev* udevCtx = udev_new();
    struct udev_device* udevDevice = udev_device_new_from_devnum(udevCtx, 'c', sinfo.st_rdev);

    if (udevDevice == NULL) {
        ERROR_LOG("udev_device_new_from_devnum: %s", strerror(errno));
        return -1;
    }

    struct udev_device* parent = udev_device_get_parent_with_subsystem_devtype(udevDevice, "usb", "usb_device");

    if (parent == NULL) {
        ERROR_LOG("udev_device_get_parent_with_subsystem_devtype");
        udev_device_unref(udevDevice);
        udev_unref(udevCtx);
    }

    info->busnum = atoi(udev_device_get_sysattr_value(parent, "busnum"));
    info->devnum = atoi(udev_device_get_sysattr_value(parent, "devnum"));

    // discover devices
    libusb_device **list;
    libusb_context *ctx = NULL; //a libusb session
    libusb_init(&ctx); //initialize a library session

    ssize_t cnt = libusb_get_device_list(ctx, &list);
    int i;
    if (cnt < 0) {
        ERROR_LOG("no usb device found");
        cnt = 0;
    }
    libusb_device *found = NULL;
    for (i = 0; i < cnt; i++) {
        libusb_device *device = list[i];
        if (info->busnum == libusb_get_bus_number(device) && info->devnum == libusb_get_device_address(device)) {
            found = device;
            break;
        }
    }

    if (found != NULL) {
        dumpdev(found, info);
    }
    libusb_free_device_list(list, 1);
    libusb_exit(ctx);
    udev_device_unref(udevDevice);
    udev_unref(udevCtx);
    if (info->type != NONE)
        return 0;
    return -1;
}*/
