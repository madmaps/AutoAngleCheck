#ifndef CAMERA_H
#define CAMERA_H

#include <errno.h>
#include <fcntl.h>
#include <linux/videodev2.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <unistd.h>


class Camera
{
public:
    Camera();
private:
    int videoDevice;
    uint8_t* buffer;
};

#endif // CAMERA_H
