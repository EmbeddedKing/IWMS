#ifndef __VIDEO_TYPES_H__
#define __VIDEO_TYPES_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <linux/videodev2.h>
#include <pthread.h>
#include <signal.h>

#define MEMCLEAR(x)     memset(&x, 0, sizeof(x))
#define VIDEO_DATASIZE  640 * 480 * 3

#define VIDEO_WIDTH     640
#define VIDEO_HIGHT     480

typedef struct video_stream {
    int video_width;
    int video_hight;
    unsigned char video_data[VIDEO_DATASIZE];
} video_stream_t;

#endif