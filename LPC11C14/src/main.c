/******************** (C) COPYRIGHT farsight************
* File Name          : main.c
* Author             : zxq
* Date First Issued  : 03/18/2012
* Description        : 
*/
#include <stdio.h>

#include "LPC11xx.h"
#include "uart.h"
#include "gpio.h"
#include "rc522.h"
#include "ssp.h"
volatile unsigned char command[50];
unsigned char a = 0;
unsigned char global = 0;
unsigned char count1 = 0;
unsigned char count2 = 0;
extern volatile uint32_t UARTCount;
extern volatile uint8_t UARTBuffer[BUFSIZE];
unsigned char data1[16] = {0x12, 0x34, 0x56, 0x78, 0xED, 0xCB, 0xA9, 0x87, 0x12, 0x34, 0x56, 0x78, 0x01, 0xFE, 0x01, 0xFE};
//M1卡的某一块写为如下格式，则该块为钱包，可接收扣款和充值命令
//4字节金额（低字节在前）＋4字节金额取反＋4字节金额＋1字节块地址＋1字节块地址取反＋1字节块地址＋1字节块地址取反
unsigned char data2[4] = {0x01, 0, 0, 0};
unsigned char DefaultKey[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
unsigned char databuf[30];
unsigned char r_buf[32];

void delay(void)
{
	unsigned int temp;
	for (temp = 500; temp != 0; temp--)
		;
}

/*******************************************************************************
* Function Name  : RFID_CheckSum
* Description    : Init RFID control.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
unsigned char RFID_CheckSum(unsigned char *databuf)
{
	unsigned char numb = 0, chksum = 0;
	for (numb = 0; numb < databuf[0]; numb++)
	{
		chksum += databuf[numb];
	}

	return chksum;
}

int main(void)
{

	unsigned char status = 0;
	unsigned char g_ucTempbuf[30];
	char i, j;
	unsigned char count = 0;
	unsigned char count_recv = 0;
	SystemInit();

	UARTInit(115200);
	SSP_IOConfig(0);
	SSP_Init(0);
	printf("start check RFID card !!!!!!!!!\n\r");
	GpioRc522();
	//初始化射频芯片
	PcdReset();
	PcdAntennaOff(); //关闭天线
	PcdAntennaOn();  //打开天线
	while (1) {
		if ((global == 1) && (a == 1)) {
			switch (command[3]) {
			case 0x21:
				count = 18;
				status = PcdSelect(g_ucTempbuf);								   //选择卡片
				status = PcdAuthState(PICC_AUTHENT1A, 1, DefaultKey, g_ucTempbuf); //验证卡片密码
				status = PcdRead(1, g_ucTempbuf);								   //读第二块地址的数据
				if (status == MI_OK) {
					LED3_ON;
				}
				if (status == MI_OK) {
					databuf[0] = 0x12;
					databuf[1] = 0x21;
					//UARTSend(& command[4],16);
					//UARTSend(g_ucTempbuf,16);
					for (i = 2, j = 0; i < count; i++, j++) {

						databuf[i] = g_ucTempbuf[j];
						if (g_ucTempbuf[j] == 0xaa) {
							i++;
							count++;
							//databuf[0] = 0x13;
							databuf[0] = databuf[0] + 1;
							databuf[i] = 0x00;
						}
					}
					j = RFID_CheckSum(databuf);
					databuf[count] = j;
					//databuf[0] = databuf[0] + 1;

					SPI0_slaveRS(0, 0xaa);
					SPI0_slaveRS(0, 0xbb);

					for (i = 0; i <= count; i++) {
						SPI0_slaveRS(0, databuf[i]);
					}
				}
				global = 0;
				//count = 18;
				GPIOSetValue(PORT1, 4, 1);
				NVIC_EnableIRQ(SSP0_IRQn);
				break;
			case 0x22:
				count = 2;
				count_recv = 29;
				r_buf[0] = 1;
				for (i = 12, j = 0; i < count_recv; i++, j++)
				{
					r_buf[j] = command[i];
				}
				UARTSend(r_buf, 16);
				//UARTSend(g_ucTempbuf,16);
				status = PcdSelect(g_ucTempbuf);								   //选择卡片
				status = PcdAuthState(PICC_AUTHENT1A, 1, DefaultKey, g_ucTempbuf); //验证卡片密码
				status = PcdWrite(1, r_buf);									   //读第二块地址的数据

				if (status == MI_OK) {
					databuf[0] = 0x03;
					databuf[1] = 0x22;

					j = RFID_CheckSum(databuf);
					databuf[2] = j;
					SPI0_slaveRS(0, 0xaa);
					SPI0_slaveRS(0, 0xbb);

					for (i = 0; i < count; i++) {
						SPI0_slaveRS(0, databuf[i]);
					}
				}
				global = 0;
				GPIOSetValue(PORT1, 4, 1);
				NVIC_EnableIRQ(SSP0_IRQn);
				break;
			default:
				break;
			}
		}
		
		status = PcdRequest(PICC_REQALL, g_ucTempbuf); //寻卡
		if (status != MI_OK) {
			LED_GREEN_OFF;
			GPIOSetValue(PORT1, 4, 1);
			a = 0;
			continue; //退出本次循环
		} else {
			GPIOSetValue(PORT1, 4, 0);
			a = 1;
		}
		printf("request ok\n\r");
		
		//防冲撞处理
		status = PcdAnticoll(g_ucTempbuf); //执行成功g_ucTempbuf[]得到卡的序列号
		if (status != MI_OK) {
			LED_GREEN_OFF;
			GPIOSetValue(PORT1, 4, 1);
			a = 0;
			continue;
		} else {
			GPIOSetValue(PORT1, 4, 0); //  有 k
			a = 1;
			LED_GREEN_ON;
		}
		printf("Anticoll ok\n\r");
		
		//选择卡片
		UARTSend(g_ucTempbuf, 4);
		status = PcdSelect(g_ucTempbuf);
        if(status != MI_OK) {
			continue;
		}
		printf("select ok\n\r");
		
		//验证卡片密码
		UARTSend(g_ucTempbuf, 4);
		status = PcdAuthState(PICC_AUTHENT1A, 1, DefaultKey, g_ucTempbuf);
        if (status != MI_OK) {
			continue;
		}
		printf("verify ok\n\r");
		
		//向卡写入数据
		status = PcdWrite(1, data1);
        if (status != MI_OK) {
			continue;
		} else {
			UARTSend(data1, 16);
		}
		printf("write ok\n\r");
		
		//操作钱包，这里是扣款
		status = PcdValue(PICC_DECREMENT, 1, data2);
        if (status != MI_OK) {
			continue;
		}
		printf("sub ok\n\r");
		
        //备份钱包
        status = PcdBakValue(1, 2);
        if (status != MI_OK) {
			continue;
		}
		printf("backup ok\n\r");
		
		//读第二块地址的数据
		status = PcdRead(1, g_ucTempbuf);
        if (status != MI_OK) {
		    LED3_OFF;
            continue;
        } else {
			UARTSend(g_ucTempbuf,16); 
			LED3_ON;
		}
		printf("read ok\n\r");
		
		PcdHalt();
	   	LED_GREEN_ON;
	}
}

/************* (C) COPYRIGHT farsight*****END OF FILE****/
