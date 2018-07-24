#ifndef __DATA_DEVICE_H__
#define __DATA_DEVICE_H__

#include "data_types.h"

#define DATA_DEVICE "/dev/ttyUSB0"

int data_uart_open(const char *pathname);
int data_uart_config(int fd, int baude, int c_flow, int bits, char parity, int stop);
int data_uart_read(int fd, char *r_buf, unsigned int len);
int data_uart_write(int fd, const char *w_buf, unsigned int len);
int data_uart_close(int fd);

#endif