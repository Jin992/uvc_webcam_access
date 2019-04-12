#ifndef __LIBV4L2_HW__
#define __LIBV4L2_HW__

struct v4l2hw {
  int fd; // file descriptor
  dev_t rdev; // device Id
  int busnum;
  int devnum;
  enum {
    NONE = 0,
    SKYPE_UVC,
    QUANTA,
    USB_UVC
  } type;
  int8_t unitId;
};

int v4l2_init(int fd, struct v4l2hw* info);


#endif
