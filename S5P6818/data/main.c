#include "data_types.h"
#include "data_device.h"
#include "data_thread.h"

/* 全局的命令标志位 */
unsigned int cmd_flag;

int main(int argc, char **argv)
{
    if (argc != 3) {
        fprintf(stderr, "usage: %s <ip> <port>\n", argv[0]);
        return -1;
    }

    int uartfd;
    /* 打开串口设备 */
    uartfd = data_uart_open(DATA_DEVICE);
    if (uartfd < 0)
        exit(-1);

    /* 设置串口的波特率 */
    data_uart_config(uartfd, 115200, 0, 8, 'N', 1);

    /* 开启一个线程读数据到共享内存 */
    pthread_t tid;
    pthread_create(&tid, NULL, data_save_thread, &uartfd);

    /* 搭建网络 */
    int lis_sockfd, ret;
    lis_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (lis_sockfd < 0) {
        perror("socket");
        return -1;
    }

    struct sockaddr_in ser_addr;
    MEMCLEAR(ser_addr);
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

    pid_t child_pid;
    int cli_sockfd;
    struct sockaddr_in cli_addr;
    socklen_t cli_addrlen = sizeof(struct sockaddr_in);
    for(;;) {
        cli_sockfd = accept(lis_sockfd, (struct sockaddr *)&cli_addr, &cli_addrlen);
        if (cli_sockfd < 0) {
            perror("accept");
            return -1;
        }
        printf("ok\n");
        child_pid = fork();
        if (child_pid < 0) {
            perror("fork");
            return -1;
        } else if (child_pid == 0) {
            close(lis_sockfd);

            /* 找到共享内存 */
            int shmid;
            key_t key = ftok(".", '1');
            if (key < 0) {
                perror("ftok");
                exit(-1);
            }

            shmid = shmget(key, sizeof(message_t), IPC_CREAT | 0666);
            if (shmid < 0) {
                perror("shmget");
                exit(-1);
            }

            void *shmaddr = shmat(shmid, NULL, 0);
            if (shmaddr == NULL) {
                perror("shmat");
                exit(-1);
            }

            /* 开一个线程来接收命令 */
            pthread_create(&tid, NULL, data_cmd_recv_thread, (void *)&cli_sockfd);
            
            /* 打开数据库 */
            sqlite3 *sql_db;
            char **sql_res;
            int sql_row;
            int sql_column;
            char sql_cmd[100] = "select * from info";
            char *sql_err;
            sqlite3_open("warehouse.db", &sql_db);

            int count = 0, i, j;
            message_t msg;
            data_pake_t pake;
            /* 循环检测flag */
            for (;;) {
                if (cmd_flag & DATA_FLAG_STREAM) {
                    if(count++ >= 1000000) {
                        MEMCLEAR(pake);
                        pake.type = DATA_PAKE_TYPE_MSG;
                        memcpy(&pake.msg, shmaddr, sizeof(message_t));
                        ret = send(cli_sockfd, &pake, sizeof(data_pake_t), 0);
                        if (ret < 0) {
                            perror("send");
                            return -1;
                        }
                        count = 0;
                    }
                }
                if (cmd_flag & DATA_FLAG_ON_LED1) {
                    MEMCLEAR(msg);
                    msg.tag = command_tag_t;
                    msg.slave_address = 1;
                    msg.data_length = 36;
                    msg.data.command.operate_id = on_led1;
                    data_uart_write(uartfd, (char *)&msg, sizeof(message_t));
                    cmd_flag &= ~DATA_FLAG_ON_LED1;
                }
                if (cmd_flag & DATA_FLAG_OFF_LED1) {
                    MEMCLEAR(msg);
                    msg.tag = command_tag_t;
                    msg.slave_address = 1;
                    msg.data_length = 36;
                    msg.data.command.operate_id = off_led1;
                    data_uart_write(uartfd, (char *)&msg, sizeof(message_t));
                    cmd_flag &= ~DATA_FLAG_OFF_LED1;
                }
                if (cmd_flag & DATA_FLAG_ON_SPEAKER) {
                    MEMCLEAR(msg);
                    msg.tag = command_tag_t;
                    msg.slave_address = 1;
                    msg.data_length = 36;
                    msg.data.command.operate_id = on_speaker;
                    data_uart_write(uartfd, (char *)&msg, sizeof(message_t));
                    cmd_flag &= ~DATA_FLAG_ON_SPEAKER;
                }
                if (cmd_flag & DATA_FLAG_OFF_SPEAKER) {
                    MEMCLEAR(msg);
                    msg.tag = command_tag_t;
                    msg.slave_address = 1;
                    msg.data_length = 36;
                    msg.data.command.operate_id = off_speaker;
                    data_uart_write(uartfd, (char *)&msg, sizeof(message_t));
                    cmd_flag &= ~DATA_FLAG_OFF_SPEAKER;
                }
                if (cmd_flag & DATA_FLAG_ON_FAN) {
                    MEMCLEAR(msg);
                    msg.tag = command_tag_t;
                    msg.slave_address = 1;
                    msg.data_length = 36;
                    msg.data.command.operate_id = on_fan;
                    data_uart_write(uartfd, (char *)&msg, sizeof(message_t));
                    cmd_flag &= ~DATA_FLAG_ON_FAN;
                }
                if (cmd_flag & DATA_FLAG_OFF_FAN) {
                    MEMCLEAR(msg);
                    msg.tag = command_tag_t;
                    msg.slave_address = 1;
                    msg.data_length = 36;
                    msg.data.command.operate_id = off_fan;
                    data_uart_write(uartfd, (char *)&msg, sizeof(message_t));
                    cmd_flag &= ~DATA_FLAG_OFF_FAN;
                }
                if (cmd_flag & DATA_FLAG_ON_SEVEN_LED) {
                    MEMCLEAR(msg);
                    msg.tag = command_tag_t;
                    msg.slave_address = 1;
                    msg.data_length = 36;
                    msg.data.command.operate_id = on_seven_led;
                    data_uart_write(uartfd, (char *)&msg, sizeof(message_t));
                    cmd_flag &= ~DATA_FLAG_ON_SEVEN_LED;
                }
                if (cmd_flag & DATA_FLAG_OFF_SEVEN_LED) {
                    MEMCLEAR(msg);
                    msg.tag = command_tag_t;
                    msg.slave_address = 1;
                    msg.data_length = 36;
                    msg.data.command.operate_id = off_seven_led;
                    data_uart_write(uartfd, (char *)&msg, sizeof(message_t));
                    cmd_flag &= ~DATA_FLAG_OFF_SEVEN_LED;
                }
                if (cmd_flag & DATA_FLAG_GET_RECORD) {
                    MEMCLEAR(pake);
                    ret = sqlite3_get_table(sql_db, sql_cmd, &sql_res, &sql_row, &sql_column, &sql_err);
                    if (ret != 0)
                        fprintf(stderr, "sqlite3_get_table:%s\n", sql_err);
                    /* 循环发送数据包 */
                    for (i = 1; i <= sql_row; i++) {
                        printf("发送数据包\n");
                        MEMCLEAR(pake);
                        pake.type = DATA_PAKE_TYPE_RECORD;
                        pake.record.id = atoi(sql_res[i * sql_column + 0]);
                        pake.record.num = atoi(sql_res[i * sql_column + 1]);
                        pake.record.time = atoi(sql_res[i * sql_column + 2]);
                        strcpy(pake.record.dir, sql_res[i * sql_column + 3]);
                        pake.record.stock = atoi(sql_res[i * sql_column + 4]);
                        ret = send(cli_sockfd, &pake, sizeof(data_pake_t), 0);
                        if (ret < 0) {
                            perror("send");
                            return -1;
                        }
                        /* 延时一下发送 */
                        j = 10000000;
                        while (j--);
                    }
                    cmd_flag &= ~DATA_FLAG_GET_RECORD;
                }
            }
            sqlite3_close(sql_db);
            close(cli_sockfd);
        }
        close(cli_sockfd);
    }

    close(lis_sockfd);
    return 0;
}