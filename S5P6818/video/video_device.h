#ifndef __VIDEO_DEVICE_H__
#define __VIDEO_DEVICE_H__

#include "video_types.h"
#include "video_list.h"

/* 映射结构体 */
typedef struct {
    void *start;
    int length;
} video_buf_t;

#define VIDEO_DEVICE   "/dev/video0"
#define VIDEO_HEIGHT   480
#define VIDEO_WIDTH    640
#define VIDEO_FORMAT   V4L2_PIX_FMT_YUYV
#define VIDEO_BUFCOUNT 1
#define VIDEO_BUFSIZE  640 * 480 * 3

extern video_buf_t *buffers;

int  video_v4l2_init(int *fd);
int  video_v4l2_grab(int fd);
int  video_v4l2_close(int fd);
int  video_v4l2_getframe(int fd, struct v4l2_buffer *buffer);
int  video_v4l2_addframe(int fd, struct v4l2_buffer *buffer);
void video_device_print(struct v4l2_capability capability);
void video_yuyv_to_rgb888(video_buf_t yuyv_buf, unsigned char *rgb_buf);

#endif