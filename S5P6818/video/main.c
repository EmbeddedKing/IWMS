#include "video_types.h"
#include "video_list.h"
#include "video_device.h"

video_head_t video_list_head;

void *video_collect(void *arg);

int main(int argc, char **argv)
{
    if (argc != 3) {
        fprintf(stderr, "usage: %s <ip> <port>\n", argv[0]);
        return -1;
    }

    /* 初始化表头 */
    video_list_init(&video_list_head);

    /* 忽略SIGPIPE信号，不禁用会造成客户端断开连接后会造成服务器进程退出 */
    struct sigaction sa;
    sa.sa_handler = SIG_IGN;
    sigaction(SIGPIPE, &sa, 0);

    /* 开启摄像头数据采集线程 */
    pthread_t video_collect_tid;
    pthread_create(&video_collect_tid, NULL, video_collect, NULL);

    /* 搭建网络 */
    int lis_sockfd, ret;
    lis_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (lis_sockfd < 0) {
        perror("socket");
        return -1;
    }

    struct sockaddr_in ser_addr;
    ser_addr.sin_family = AF_INET;
    ser_addr.sin_port = htons(atoi(argv[2]));
    ser_addr.sin_addr.s_addr = inet_addr(argv[1]);
    socklen_t ser_addrlen = sizeof(struct sockaddr_in);
    ret = bind(lis_sockfd, (struct sockaddr *)&ser_addr, ser_addrlen);
    if (ret < 0) {
        perror("bind");
        return -1;
    }

    ret = listen(lis_sockfd, 20);
    if (ret < 0) {
        perror("listen");
        return -1;
    }

    /* 循环读取侦听描述符并且将侦听描述符 */
    int cli_sockfd;
    struct sockaddr_in cli_addr;
    socklen_t cli_addrlen = sizeof(struct sockaddr_in);

    for (;;) {
        MEMCLEAR(cli_addr);
        cli_sockfd = accept(lis_sockfd, (struct sockaddr *)&cli_addr, &cli_addrlen);
        if (cli_sockfd < 0) {
            perror("accept");
            return -1;
        }

        /* 将文件描述符加入到表中 */
        video_list_add(&video_list_head, cli_sockfd);
    }
}

void *video_collect(void *arg)
{
    int fd, ret;
    video_stream_t stream;
    ret = video_v4l2_init(&fd);
    if (ret < 0)
        exit(-1);
    ret = video_v4l2_grab(fd);
    if (ret < 0)
        exit(-1);

    struct v4l2_buffer buf;
    for (;;) {
        ret = video_v4l2_getframe(fd, &buf);
        if (ret < 0)
            exit(-1);

        MEMCLEAR(stream);
        stream.video_width = VIDEO_WIDTH;
        stream.video_hight = VIDEO_HEIGHT;
        video_yuyv_to_rgb888(buffers[buf.index], stream.video_data);
        video_list_send(&video_list_head, stream);
        ret = video_v4l2_addframe(fd, &buf);
        if (ret < 0)
            exit(-1);
    }

    video_v4l2_close(fd);
    pthread_exit("线程退出\n");
}