#include "data_device.h"

int data_uart_open(const char *pathname)
{
    /* 打开串口 */
    int fd;
    fd = open(pathname, O_RDWR | O_NOCTTY | O_NDELAY);
    if (fd < 0)
    {
        perror("open");
        return -1;
    }

    /* 设置串口为阻塞状态 */
    if (fcntl(fd, F_SETFL, 0) < 0)
    {
        fprintf(stderr, "F_SETFL\n");
        return -1;
    }

    /*  */
    if (isatty(STDIN_FILENO) == 0)
    {
        perror("standard input is not a terminal device");
        return -1;
    }

    return fd;
}

int data_uart_config(int fd, int baude, int c_flow, int bits, char parity, int stop)
{
    struct termios options;
    /*获取终端属性*/
    if (tcgetattr(fd, &options) < 0)
    {
        perror("tcgetattr");
        return -1;
    }
    cfmakeraw(&options);
    /*设置输入输出波特率，两者保持一致*/
    switch (baude)
    {
    case 115200:
        cfsetispeed(&options, B115200);
        cfsetospeed(&options, B115200);
        break;
    case 9600:
        cfsetispeed(&options, B9600);
        cfsetospeed(&options, B9600);
        break;
    case 19200:
        cfsetispeed(&options, B19200);
        cfsetospeed(&options, B19200);
        break;
    default:
        return -1;
    }

    /*设置控制模   */
    options.c_cflag |= (CLOCAL) | (CREAD); //保证程序不占用串口 //保证程序可以从串口中读取数据

    /*设置数据流控制*/
    switch (c_flow)
    {
    case 0: //不进行流控制
        options.c_cflag &= ~CRTSCTS;
        break;
    case 1: //进行硬件流控制
        options.c_cflag |= CRTSCTS;
        break;
    case 2: //进行软件流控制
        options.c_cflag |= IXON | IXOFF | IXANY;
        break;
    default:
        fprintf(stderr, "Unkown c_flow!\n");
        return -1;
    }

    /*设置数据位*/
    switch (bits)
    {
    case 5:
        options.c_cflag &= ~CSIZE; //屏蔽其它标志位
        options.c_cflag |= CS5;
        break;
    case 6:
        options.c_cflag &= ~CSIZE; //屏蔽其它标志位
        options.c_cflag |= CS6;
        break;
    case 7:
        options.c_cflag &= ~CSIZE; //屏蔽其它标志位
        options.c_cflag |= CS7;
        break;
    case 8:
        options.c_cflag &= ~CSIZE; //屏蔽其它标志位
        options.c_cflag |= CS8;
        break;
    default:
        fprintf(stderr, "Unkown bits!\n");
        return -1;
    }

    /*设置校验位*/
    switch (parity)
    {
    /*无奇偶校验位*/
    case 'n':
    case 'N':
        options.c_cflag &= ~PARENB; //PARENB：产生奇偶位，执行奇偶校验
        options.c_cflag &= ~INPCK;  //INPCK：使奇偶校验起作用
        break;
    /*设为空格,即停止位为2位*/
    case 's':
    case 'S':
        options.c_cflag &= ~PARENB; //PARENB：产生奇偶位，执行奇偶校验
        options.c_cflag &= ~CSTOPB; //CSTOPB：使用两位停止位
        break;
    /*设置奇校验*/
    case 'o':
    case 'O':
        options.c_cflag |= PARENB; //PARENB：产生奇偶位，执行奇偶校验
        options.c_cflag |= PARODD; //PARODD：若设置则为奇校验,否则为偶校验
        options.c_cflag |= INPCK;  //INPCK：使奇偶校验起作用
        options.c_cflag |= ISTRIP; //ISTRIP：若设置则有效输入数字被剥离7个字节，否则保留全部8位
        break;
    /*设置偶校验*/
    case 'e':
    case 'E':
        options.c_cflag |= PARENB;  //PARENB：产生奇偶位，执行奇偶校验
        options.c_cflag &= ~PARODD; //PARODD：若设置则为奇校验,否则为偶校验
        options.c_cflag |= INPCK;   //INPCK：使奇偶校验起作用
        options.c_cflag |= ISTRIP;  //ISTRIP：若设置则有效输入数字被剥离7个字节，否则保留全部8位
        break;
    default:
        fprintf(stderr, "Unkown parity!\n");
        return -1;
    }

    /*设置停止位*/
    switch (stop)
    {
    case 1:
        options.c_cflag &= ~CSTOPB; //CSTOPB：使用1   位停止位
        break;
    case 2:
        options.c_cflag |= CSTOPB; //CSTOPB：使用两位停止位
        break;
    default:
        fprintf(stderr, "Unkown stop!\n");
        return -1;
    }

    /*设置输出模式为原始输出*/
    options.c_oflag &= ~OPOST; //OPOST：若设置则按定义的输出处理，否则所有c_oflag失效

    /*设置本地模式为原始模式*/
    options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    /*
     *ICANON：允许规范模式进行输入处理
     *ECHO：允许输入字符的本地回显
     *ECHOE：在接收EPASE时执行Backspace,Space,Backspace组合
     *ISIG：允许信号
     */

    /*设置等待时间和最小接受字符*/
    options.c_cc[VTIME] = 0; //可以在select中设置
    options.c_cc[VMIN] = 1;  //最少读取一个字符

    /*如果发生数据溢出，只接受数据，但是不进行读操作*/
    tcflush(fd, TCIFLUSH);

    /*激活配置*/
    if (tcsetattr(fd, TCSANOW, &options) < 0)
    {
        perror("tcsetattr failed");
        return -1;
    }

    return 0;
}

int data_uart_read(int fd, char *r_buf, uint32_t len)
{
    int ret;
    ssize_t cnt = 0;
    fd_set rfds;
    struct timeval time;

    /*将文件描述符加入读描述符集合*/
    FD_ZERO(&rfds);
    FD_SET(fd, &rfds);

    /*设置超时为5 s*/
    time.tv_sec = 5;
    time.tv_usec = 0;
    ret = select(fd + 1, &rfds, NULL, NULL, &time);
    switch (ret)
    {
    case -1:
        fprintf(stderr, "select error!\n");
        return -1;
    case 0:
        fprintf(stderr, "time over!\n");
        return 0;
    default:
        cnt = read(fd, r_buf, len);
        if (cnt == -1)
        {
            fprintf(stderr, "read error!\n");
            return -1;
        }
        return cnt;
    }
}

int data_uart_write(int fd, const char *w_buf, uint32_t len)
{
    ssize_t cnt = 0;

    cnt = write(fd, w_buf, len);
    if (cnt == -1)
    {
        fprintf(stderr, "write error!\n");
        return -1;
    }

    return cnt;
}

int data_uart_close(int fd)
{
    assert(fd);
    close(fd);
    return 0;
}

void send_data(int fd, message_t *message)
{
    /*	
	message_t w_message;
	while(1){
		memset(&w_message,0,sizeof(message_t));
		printf("1111\n");
		sleep(1);
		w_message.tag = command_tag_t;
		w_message.data_length = len;
		w_message.slave_address = 1;
		
		w_message.data.command.operate_id =  off_fan;
	    	//ret = uart_write(fd,(char *)&w_message,len);
	    	ret = write(fd,&w_message,len);
			
	        if(ret == -1)
	       {
	        	fprintf(stderr,"uart write failed!\n");
	       		exit(EXIT_FAILURE);
	       }
	
		sleep(4);	
		w_message.data.command.operate_id =  off_fan;
	    	//ret = uart_write(fd,(char *)&w_message,len);
		ret = write(fd,&w_message,len);
		if(ret == -1)
	       {
	        	fprintf(stderr,"uart write failed!\n");
	       		exit(EXIT_FAILURE);
	       }
*/
    /*
	while(1){
		switch (message->tag){
			case start_machine_t:
				break;
			case data_flow_t:
				break;
			case rfid_msg_t:
				break;
			case command_tag_t:
				break;
			case key_msg_t:
				break;
			case other_type_t:
				break;
			default:
				break;
		}
	}
	*/
}
