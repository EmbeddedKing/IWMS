#ifndef __DATA_THREAD_H__
#define __DATA_THREAD_H__

#include "data_types.h"

extern unsigned int cmd_flag;

void *data_save_thread(void *arg);
void *data_cmd_recv_thread(void *arg);

#endif
