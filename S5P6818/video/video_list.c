#include "video_list.h"

void video_list_init(video_head_t *head)
{
    /* 创建头结点 */
    (*head) = malloc(sizeof(video_node_t));
    /* 头结点中的fd为-1，指向NULL */
    (*head)->fd = -1;
    (*head)->next = NULL;
}

void video_list_add(video_head_t *head, int fd)
{
    /* 漫游指针指向头结点，尾增 */
    video_node_t *walk_pointer = (*head);

    /* 申请新节点的内存 */
    video_node_t *new_node = malloc(sizeof(video_node_t));
    new_node->fd = fd;
    new_node->next = NULL;

    /* 找到尾节点 */
    while (walk_pointer->next)
        walk_pointer = walk_pointer->next;

    /* 将新节点插入到尾部 */
    walk_pointer->next = new_node;
}

int video_list_del(video_head_t *head, int fd)
{
    /* 漫游指针指向头结点 */
    video_node_t *walk_pointer = *head;
    video_node_t *del_pointer;

    /* 如果头结点指针为NULL或者表为空则返回-1 */
    if (walk_pointer == NULL || walk_pointer->next == NULL)
        return -1;

    /* 找到要删除节点的前一个节点 */
    while (walk_pointer->next->fd != fd && walk_pointer->next->next)
        walk_pointer = walk_pointer->next;

    /* 如果 */
    if (walk_pointer->next->fd == fd) {
        del_pointer = walk_pointer->next;
        walk_pointer->next = walk_pointer->next->next;
        free(del_pointer);
        return 0;
    }

    return -1;
}

void video_list_print(video_head_t *head)
{
    if (!(*head))
        return;

    /* 指向第一个节点 */
    video_node_t *walk_pointer = (*head)->next; 

    while (walk_pointer) {
        printf("elem:%d\n", walk_pointer->fd);
        walk_pointer = walk_pointer->next;
    }
}

int video_list_isempty(video_head_t *head)
{
    if (!(*head))
        return -1;
    return (*head)->next == NULL ? 1 : 0;
}

int video_list_length(video_head_t *head)
{
    if (!(*head))
        return -1;

    int len = 0;
    /* 指向第一个节点 */
    video_node_t *walk_pointer = (*head)->next;

    while (walk_pointer) {
        walk_pointer = walk_pointer->next;
        len++;
    }
    return len;
}

int video_list_send(video_head_t *head, video_stream_t stream)
{
    if (!(*head))
        return -1;

    int ret;

    /* 指向第一个节点 */
    video_node_t *walk_pointer = (*head)->next;
    while (walk_pointer) {
        ret = send(walk_pointer->fd, &stream, sizeof(video_stream_t), 0);
        if (ret <= 0) {
            close(walk_pointer->fd);
            video_list_del(head, walk_pointer->fd);
        }
        walk_pointer = walk_pointer->next;
    }

    return 0;
}