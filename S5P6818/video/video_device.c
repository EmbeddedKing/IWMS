#include "video_device.h"

//设备描述符
video_buf_t *buffers;

/* 在初始化中主要是打开设备并设置传输的图片格式，读取设备信息不是必选项 */
int video_v4l2_init(int *fd)
{
    int ret;

    (*fd) = open(VIDEO_DEVICE, O_RDWR);
    if ((*fd) < 0)
    {
        perror("open");
        return -1;
    }

    /* 获取摄像头设备属性 */
    struct v4l2_capability capability;
    ret = ioctl((*fd), VIDIOC_QUERYCAP, &capability);
    if (ret < 0)
    {
        perror("VIDIOC_QUERYCAP");
        return -1;
    }

    //video_device_print(capability);

    /* 获取支持的图像格式，遍历支持格式表 */
    struct v4l2_fmtdesc fmtdesc;
    fmtdesc.index = 0;
    fmtdesc.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    while (1)
    {
        ret = ioctl((*fd), VIDIOC_ENUM_FMT, &fmtdesc);
        if (ret < 0)
            break;
        fmtdesc.index++;
    }

    /* 设置照片格式，分辨率640 * 480，使用YUYV格式压缩  */
    struct v4l2_format format;
    format.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    format.fmt.pix.height = VIDEO_HEIGHT;
    format.fmt.pix.width = VIDEO_HEIGHT;
    format.fmt.pix.pixelformat = VIDEO_FORMAT;
    format.fmt.pix.field = V4L2_FIELD_INTERLACED;
    ret = ioctl((*fd), VIDIOC_S_FMT, &format);
    if (ret < 0)
    {
        perror("VIDIOC_S_FMT");
        return -1;
    }

    /* 获取照片格式 */
    ret = ioctl((*fd), VIDIOC_G_FMT, &format);
    if (ret < 0)
    {
        perror("VIDIOC_G_FMT");
        return -1;
    }

    return 0;
}

int video_v4l2_grab(int fd)
{
    int ret, i;

    /* 创建缓冲区，在内存中已经存在了count个缓冲块，接下来我们应该把这些缓冲块假如到缓冲队列里面 */
    struct v4l2_requestbuffers requestbuffers;
    requestbuffers.count = VIDEO_BUFCOUNT;
    requestbuffers.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    requestbuffers.memory = V4L2_MEMORY_MMAP;
    ret = ioctl(fd, VIDIOC_REQBUFS, &requestbuffers);
    if (ret < 0)
    {
        perror("VIDIOC_REQBUFS");
        return -1;
    }

    buffers = (video_buf_t *)calloc(requestbuffers.count, sizeof(video_buf_t));
    if (buffers == NULL)
    {
        perror("calloc\n");
        return -1;
    }

    struct v4l2_buffer buffer;
    /* 内存缓冲块映射 */
    for (i = 0; i < requestbuffers.count; i++)
    {
        buffer.index = i;
        buffer.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buffer.memory = V4L2_MEMORY_MMAP;
        ret = ioctl(fd, VIDIOC_QUERYBUF, &buffer);
        if (ret < 0)
        {
            perror("VIDIOC_QUERYBUF");
            return -1;
        }

        buffers[i].start = mmap(NULL, buffer.length, PROT_READ | PROT_WRITE, MAP_SHARED, fd, buffer.m.offset);
        if (buffers[i].start == MAP_FAILED)
        {
            perror("mmap");
            return -1;
        }

        buffers[i].length = buffer.length;

        /* 将该缓冲块放入缓存队列 */
        buffer.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buffer.memory = V4L2_MEMORY_MMAP;
        ret = ioctl(fd, VIDIOC_QBUF, &buffer);
        if (ret < 0)
        {
            perror("VIDIOC_QBUF");
            return -1;
        }
    }

    /* 开始录制 */
    enum v4l2_buf_type type;
    type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    ret = ioctl(fd, VIDIOC_STREAMON, &type);
    if (ret < 0)
    {
        perror("VIDIOC_STREAMON");
        return -1;
    }

    return 0;
}

int video_v4l2_getframe(int fd, struct v4l2_buffer *buffer)
{
    int ret;
    (*buffer).type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    ret = ioctl(fd, VIDIOC_DQBUF, buffer);
    if (ret < 0)
    {
        perror("VIDIOC_DQBUF");
        return -1;
    }

    return 0;
}

int video_v4l2_addframe(int fd, struct v4l2_buffer *buffer)
{
    int ret;
    (*buffer).type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    ret = ioctl(fd, VIDIOC_QBUF, buffer);
    if (ret < 0)
    {
        perror("VIDIOC_QBUF");
        return -1;
    }
    return 0;
}

int video_v4l2_close(int fd)
{
    int ret, i;
    enum v4l2_buf_type type;
    ret = ioctl(fd, VIDIOC_STREAMOFF, &type);
    if (ret < 0)
    {
        perror("VIDIOC_STREAMOFF");
        return -1;
    }

    for (i = 0; i < VIDEO_BUFCOUNT; i++)
    {
        munmap(buffers[i].start, buffers[i].length);
    }

    return 0;
}

void video_yuyv_to_rgb888(video_buf_t yuyv_buf, unsigned char *rgb_buf)
{
    int i, j;
    unsigned char y1, y2, u, v;
    int r1, g1, b1, r2, g2, b2;
    char *pointer;

    pointer = yuyv_buf.start;

    for (i = 0; i < VIDEO_HEIGHT; i++)
    {
        for (j = 0; j < VIDEO_WIDTH / 2; j++) //每次取4个字节，也就是两个像素点，转换rgb，6个字节，还是两个像素点
        {
            y1 = *(pointer + (i * VIDEO_WIDTH / 2 + j) * 4);
            u = *(pointer + (i * VIDEO_WIDTH / 2 + j) * 4 + 1);
            y2 = *(pointer + (i * VIDEO_WIDTH / 2 + j) * 4 + 2);
            v = *(pointer + (i * VIDEO_WIDTH / 2 + j) * 4 + 3);

            r1 = y1 + 1.042 * (v - 128);
            g1 = y1 - 0.34414 * (u - 128) - 0.71414 * (v - 128);
            b1 = y1 + 1.772 * (u - 128);

            r2 = y2 + 1.042 * (v - 128);
            g2 = y2 - 0.34414 * (u - 128) - 0.71414 * (v - 128);
            b2 = y2 + 1.772 * (u - 128);

            if (r1 > 255)
                r1 = 255;
            else if (r1 < 0)
                r1 = 0;

            if (b1 > 255)
                b1 = 255;
            else if (b1 < 0)
                b1 = 0;

            if (g1 > 255)
                g1 = 255;
            else if (g1 < 0)
                g1 = 0;

            if (r2 > 255)
                r2 = 255;
            else if (r2 < 0)
                r2 = 0;

            if (b2 > 255)
                b2 = 255;
            else if (b2 < 0)
                b2 = 0;

            if (g2 > 255)
                g2 = 255;
            else if (g2 < 0)
                g2 = 0;

            *(rgb_buf + (i * VIDEO_WIDTH / 2 + j) * 6) = (unsigned char)b1;
            *(rgb_buf + (i * VIDEO_WIDTH / 2 + j) * 6 + 1) = (unsigned char)g1;
            *(rgb_buf + (i * VIDEO_WIDTH / 2 + j) * 6 + 2) = (unsigned char)r1;
            *(rgb_buf + (i * VIDEO_WIDTH / 2 + j) * 6 + 3) = (unsigned char)b2;
            *(rgb_buf + (i * VIDEO_WIDTH / 2 + j) * 6 + 4) = (unsigned char)g2;
            *(rgb_buf + (i * VIDEO_WIDTH / 2 + j) * 6 + 5) = (unsigned char)r2;
        }
    }

    return;
}

void video_device_print(struct v4l2_capability capability)
{
    printf("设备信息：\n");
    printf("-------------------------------\n"
           "Driver:       %s\n"
           "Card:         %s\n"
           "Bus:          %s\n"
           "Version:      %u.%u.%u\n"
           "Capabilities: %x\n"
           , capability.driver
           , capability.card
           , capability.bus_info
           , (capability.version >> 16) & 0xff
           , (capability.version >> 8) & 0xff
           , capability.version & 0xff
           , capability.capabilities);

    if (capability.capabilities & V4L2_CAP_VIDEO_CAPTURE)
    {
        printf("Camera device %s: support capture\n", VIDEO_DEVICE);
    }

    if (capability.capabilities & V4L2_CAP_STREAMING)
    {
        printf("Camera device %s: support streaming.\n", VIDEO_DEVICE);
    }
}