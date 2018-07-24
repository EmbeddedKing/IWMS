#ifndef __VIDEO_LIST_H__
#define __VIDEO_LIST_H__

#include "video_types.h"
#include "video_device.h"

typedef struct video_node {
    int fd;
    struct video_node *next;
} video_node_t, *video_head_t;

void video_list_init(video_head_t *head);
void video_list_add(video_head_t *head, int fd);
int  video_list_del(video_head_t *head, int fd);
int  video_list_isempty(video_head_t *head);
int  video_list_length(video_head_t *head);
void video_list_print(video_head_t *head);
int video_list_send(video_head_t *head, video_stream_t stream);

#endif