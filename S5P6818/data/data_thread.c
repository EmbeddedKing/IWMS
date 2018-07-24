#include "data_thread.h"
#include "data_device.h"

void *data_save_thread(void *arg)
{
    /* 打开串口 */
    int fd = *(int *)arg;
    int ret, num;
    char sql_cmd[100];
    char *sql_err;
    char **sql_res;
    int sql_row;
    int sql_column;
    message_t msg;

    /* 创建串口数据共享内存 */
    int shmid;
    key_t key = ftok(".", '1');
    if (key < 0) {
        perror("ftok");
        goto err3;
    }

    shmid = shmget(key, sizeof(message_t), IPC_CREAT | 0666);
    if (shmid < 0) {
        perror("shmget");
        goto err3;
    }

    void *shmaddr = shmat(shmid, NULL, 0);
    if (shmaddr == NULL) {
        perror("shmat");
        goto err3;
    }

    /* 打开数据库 */
    sqlite3 *sql_db;
    ret = sqlite3_open("warehouse.db", &sql_db);
    if (ret < 0) {
        fprintf(stderr, "sqlite3_open\n");
        goto err2;
    }

    /* 循环处理信息转换成数据包 */
    num = 0;
    for (;;)
    {
        /* 读取包的数据 */
        ret = data_uart_read(fd, (char *)&msg + num, sizeof(message_t) - num);
        if (ret == 0)
            num = 0;
        else if (ret < 0)
            goto err1;
        
        num += ret;
        if (num == sizeof(message_t))
        {
            printf("%x\n", msg.tag);
            num = 0;
            /* 判断包的类型 */
            switch (msg.tag) {
            /* 如果是采集数据包则将包的数据放入共享内存 */
            case data_flow_t:
                memcpy(shmaddr, &msg, sizeof(message_t));
                break;
            /* 如果是RFID数据则将数据放入数据库 */
            case rfid_msg_t:
                /* 查询数据库 */
                bzero(sql_cmd, sizeof(sql_cmd));
                sprintf(sql_cmd, "select * from info where id = %d", msg.data.rfid.id);
                ret = sqlite3_get_table(sql_db, sql_cmd, &sql_res, &sql_row, &sql_column, &sql_err);
                if (ret < 0) {
                    fprintf(stderr, "sqlite3_get_table:%s\n", sql_err);
                    goto err1;
                }

                if (!sql_row) {
                    /* 存入数据库 */
                    bzero(sql_cmd, sizeof(sql_cmd));
                    sprintf(sql_cmd, "insert into info values(%d, 1, %d, '入库', 1)", msg.data.rfid.id, (unsigned int)time(NULL));
                    ret = sqlite3_exec(sql_db, sql_cmd, NULL, NULL, &sql_err);
                    if (ret != 0) {
                        fprintf(stderr, "sqlite3_exec:%s\n", sql_err);
                        goto err;
                    }
                } else {
                    /* 存入数据库 */
                    bzero(sql_cmd, sizeof(sql_cmd));
                    sprintf(sql_cmd, "insert into info values(%d, 1, %d, '入库', %d)", msg.data.rfid.id, (unsigned int)time(NULL), atoi(sql_res[sql_column * sql_row + 4]) + 1);
                    ret = sqlite3_exec(sql_db, sql_cmd, NULL, NULL, &sql_err);
                    if (ret != 0) {
                        fprintf(stderr, "sqlite3_exec:%s\n", sql_err);
                        goto err;
                    }
                }
                
                sqlite3_free_table(sql_res);
                break;
            default:
                break;
            }
        }
    }

    pthread_exit("正常退出");

err:
    sqlite3_free_table(sql_res);
err1:
    sqlite3_close(sql_db);
err2:
    shmdt(shmaddr);
err3:
    close(fd);
    exit(-1);

}

void *data_cmd_recv_thread(void *arg)
{
    int fd = *(int *)arg;
    int ret;
    data_cmd_t cmd;

    for (;;) {
        ret = recv(fd, &cmd, sizeof(cmd), 0);
        if (ret < 0)
            exit(-1);
        
        switch (cmd.cmd) {
        case DATA_CMD_ON_STREAM:
            cmd_flag |= DATA_FLAG_STREAM;
            break;
        case DATA_CMD_OFF_STREAM:
            cmd_flag &= ~DATA_FLAG_STREAM;
            break;
        case DATA_CMD_ON_LED1:
            cmd_flag |= DATA_FLAG_ON_LED1;
            break;
        case DATA_CMD_OFF_LED1:
            cmd_flag |= DATA_FLAG_OFF_LED1;
            break;
        case DATA_CMD_ON_SPEAKER:
            cmd_flag |= DATA_FLAG_ON_SPEAKER;
            break;
        case DATA_CMD_OFF_SPEAKER:
            cmd_flag |= DATA_FLAG_OFF_SPEAKER;
            break;
        case DATA_CMD_ON_FAN:
            cmd_flag |= DATA_FLAG_ON_FAN;
            break;
        case DATA_CMD_FAN_LOW:
            cmd_flag |= DATA_FLAG_FAN_LOW;
            break;
        case DATA_CMD_FAN_MID:
            cmd_flag |= DATA_FLAG_FAN_MID;
            break;
        case DATA_CMD_FAN_HIGH:
            cmd_flag |= DATA_FLAG_FAN_HIGH;
            break;
        case DATA_CMD_OFF_FAN:
            cmd_flag |= DATA_FLAG_OFF_FAN;
            break;
        case DATA_CMD_ON_SEVEN_LED:
            cmd_flag |= DATA_FLAG_ON_SEVEN_LED;
            break;
        case DATA_CMD_OFF_SEVEN_LED:
            cmd_flag |= DATA_FLAG_OFF_SEVEN_LED;
            break;
        case DATA_CMD_OFF_MACHINE:
            cmd_flag |= DATA_FLAG_OFF_MACHINE;
            break;
        case DATA_CMD_GET_RECORD:
            cmd_flag |= DATA_FLAG_GET_RECORD;
            break;
        default:
            printf("未知的命令");
        }
    }
}