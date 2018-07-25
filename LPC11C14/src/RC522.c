#include "LPC11xx.h"
#include "uart.h"
#include "gpio.h"
#include "rc522.h"
#define MAXRLEN 18
/******************************************************************************/
__asm void __nop_(void)
{
    mov r0, r0
	mov r0, r0
	mov r0, r0
	mov r0, r0
	mov r0, r0
	mov r0, r0
	mov r0, r0
	mov r0, r0
	mov r0, r0
	mov r0, r0
	mov r0, r0
	mov r0, r0
	mov r0, r0
	mov r0, r0
	mov r0, r0
	mov r0, r0 
	mov r0, r0 
	mov r0, r0 
	mov r0, r0
	mov r0, r0
	mov r0, r0
	mov r0, r0 
	mov r0, r0
	mov r0, r0 
	mov r0, r0 
	mov r0, r0 
	mov r0, r0 
	mov r0, r0 
	mov r0, r0 
	mov r0, r0 
	mov r0, r0 
	mov r0, r0 
	mov r0, r0 
	mov r0, r0 
	mov r0, r0 
	mov r0, r0 
	mov r0, r0 
	mov r0, r0 
	mov r0, r0 
	mov r0, r0 
	mov r0, r0 
	mov r0, r0 
	bx lr
}

/***********************************************************************/
unsigned char ReadRawRC(unsigned char addr)
{
    unsigned char i, ucaddr;
    unsigned char ucResult = 0;
    GPIOSetValue(PORT1, RC522_SCK, 0); //clock line low level
    GPIOSetValue(PORT0, RC522_NSS, 0); //enable rc522 low level
    ucaddr = ((addr << 1) & 0x7e) | 0x80;
    for (i = 8; i > 0; i--)
    {
        GPIOSetValue(PORT1, RC522_SI, ((ucaddr & 0x80) == 0x80));
        GPIOSetValue(PORT1, RC522_SCK, 1);
        ucaddr <<= 1;
        GPIOSetValue(PORT1, RC522_SCK, 0);
    }
    for (i = 8; i > 0; i--)
    {
        GPIOSetValue(PORT1, RC522_SCK, 1);
        ucResult <<= 1;
        ucResult |= RCR22_SObit;
        GPIOSetValue(PORT1, RC522_SCK, 0);
    }
    GPIOSetValue(PORT0, RC522_NSS, 1); //enable rc522 low level
    GPIOSetValue(PORT1, RC522_SCK, 1); //clock line low level
    return ucResult;
}
/******************************************************************/
void WriteRawRC(unsigned char addr, unsigned char val)
{
    unsigned char i, ucaddr;

    GPIOSetValue(PORT1, RC522_SCK, 0); //clock line low level
    GPIOSetValue(PORT0, RC522_NSS, 0); //enable rc522 low level
    ucaddr = ((addr << 1) & 0x7e);     //write higher bit 0 and lower bit 0
    for (i = 8; i > 0; i--)
    {
        GPIOSetValue(PORT1, RC522_SI, ((ucaddr & 0x80) == 0x80));
        GPIOSetValue(PORT1, RC522_SCK, 1);
        ucaddr <<= 1;
        GPIOSetValue(PORT1, RC522_SCK, 0);
    }
    for (i = 8; i > 0; i--)
    {
        __nop_();
        GPIOSetValue(PORT1, RC522_SI, ((val & 0x80) == 0x80));
        GPIOSetValue(PORT1, RC522_SCK, 1);
        val <<= 1;
        __nop_();
        GPIOSetValue(PORT1, RC522_SCK, 0);
    }
    __nop_();
    GPIOSetValue(PORT0, RC522_NSS, 1); //enable rc522 low level
    GPIOSetValue(PORT1, RC522_SCK, 1); //clock line low level
}

/////////////////////////////////////////////////////////////////////
//功    能：清RC522寄存器位
//参数说明：reg[IN]:寄存器地址
//          mask[IN]:清位值
/////////////////////////////////////////////////////////////////////
void ClearBitMask(unsigned char reg, unsigned char mask)
{
    char tmp = 0x0;
    tmp = ReadRawRC(reg);         //读Reg寄存器的状态
    WriteRawRC(reg, tmp & ~mask); //向Reg寄存器写入tmp&~mask位
}
/***********************************************************************/
void PcdAntennaOff(void)
{
    ClearBitMask(TxControlReg, 0x03);
}

/*******************************************************************/
char PcdReset(void)
{
    GPIOSetValue(PORT0, RC522_RESET, 1); // reset rc522;
    __nop_();
    GPIOSetValue(PORT0, RC522_RESET, 0);
    __nop_();
    GPIOSetValue(PORT0, RC522_RESET, 1);
    __nop_();
    WriteRawRC(CommandReg, PCD_RESETPHASE);
    __nop_();
    WriteRawRC(ModeReg, 0x3d);
    __nop_();
    WriteRawRC(TReloadRegL, 30);

    WriteRawRC(TReloadRegH, 0);

    WriteRawRC(TModeReg, 0x8d);

    WriteRawRC(TPrescalerReg, 0x3e);

    WriteRawRC(TxAskReg, 0x40);

    return MI_OK;
}
/////////////////////////////////////////////////////////////////////
//功    能：置RC522寄存器位
//参数说明：reg[IN]:寄存器地址
//          mask[IN]:置位值
/////////////////////////////////////////////////////////////////////
void SetBitMask(unsigned char reg, unsigned char mask)
{
    char tmp = 0x0;
    tmp = ReadRawRC(reg);        //读Reg寄存器的状态
    WriteRawRC(reg, tmp | mask); //向Reg寄存器写入 MASK|tmp位
}

/***************************************************************************/
void PcdAntennaOn(void)
{
    unsigned char i;
    i = ReadRawRC(TxControlReg);
    if (!(i & 0x03))
    {
        SetBitMask(TxControlReg, 0x03);
    }
}
/////////////////////////////////////////////////////////////////////
//功    能：通过RC522和ISO14443卡通讯
//参数说明：Command[IN]:RC522命令字
//          pInData[IN]:通过RC522发送到卡片的数据
//          InLenByte[IN]:发送数据的字节长度
//          pOutData[OUT]:接收到的卡片返回数据
//          *pOutLenBit[OUT]:返回数据的位长度
/////////////////////////////////////////////////////////////////////
char PcdComMF522(unsigned char Command, unsigned char *pInData, unsigned char InLenByte, unsigned char *pOutData, unsigned int *pOutLenBit)
{
    char status = MI_ERR;
    unsigned char irqEn = 0x00;
    unsigned char waitFor = 0x00;
    unsigned char lastBits;
    unsigned char n;
    unsigned int i;
    switch (Command)
    {
    case PCD_AUTHENT:   // 0x0E   验证密钥
        irqEn = 0x12;   // 0001 0010
        waitFor = 0x10; // 0001 0000
        break;
    case PCD_TRANSCEIVE: //0x0C   发送并接收数据
        irqEn = 0x77;    // 0111 0111
        waitFor = 0x30;  // 0011 0000
        break;
    default:
        break;
    }

    WriteRawRC(ComIEnReg, irqEn | 0x80); //使能相应中断
    ClearBitMask(ComIrqReg, 0x80);       //清零所有中断标志
    WriteRawRC(CommandReg, PCD_IDLE);    //命令寄存器复零
    SetBitMask(FIFOLevelReg, 0x80);      //复位FIFO读写指针

    for (i = 0; i < InLenByte; i++)
    {
        WriteRawRC(FIFODataReg, pInData[i]);
    }                                //把要发送给卡的数据写入522的FIFO数据寄存器
    WriteRawRC(CommandReg, Command); //522执行发送命令

    if (Command == PCD_TRANSCEIVE) //是否为发送数据的命令
    {
        SetBitMask(BitFramingReg, 0x80);
    } //执行发送数据命令 仅与发送数据命令有效结合

    i = 500; //根据时钟频率调整，操作M1卡最大等待时间25ms
    do
    {
        n = ReadRawRC(ComIrqReg); // 读中断标志寄存器
        i--;
    } while ((i != 0) && !(n & 0x01) && !(n & waitFor)); //退出的IRQ条件:1. i=0 2.定时器减到零中断3.接收器检测到有效的数据中断OR命令本身终止中断

    ClearBitMask(BitFramingReg, 0x80); //结束发送数据命令

    if (i != 0) //射频区无卡
    {

        if (!(ReadRawRC(ErrorReg) & 0x1B)) //读错误标志寄存器&0001 1011不是一下错误：数据FIFO已满、碰撞错误、奇偶校验错误、SOF错误
        {
            status = MI_OK;       //暂时无错误
            if (n & irqEn & 0x01) //是否为定时器中断
            {
                status = MI_NOTAGERR;
            } //错误
            if (Command == PCD_TRANSCEIVE)
            {
                n = ReadRawRC(FIFOLevelReg);             //共接收了几个字节
                lastBits = ReadRawRC(ControlReg) & 0x07; //接收的最后一个字节的有效位
                if (lastBits)
                {
                    *pOutLenBit = (n - 1) * 8 + lastBits;
                } //N*8+LASTBITS位
                else
                {
                    *pOutLenBit = n * 8;
                } //N*8位
                if (n == 0)
                {
                    n = 1;
                }
                if (n > MAXRLEN)
                {
                    n = MAXRLEN;
                } //最多可接收18个字节
                for (i = 0; i < n; i++)
                {
                    pOutData[i] = ReadRawRC(FIFODataReg);
                } //取出接收到的数据放到数组POUTDATA【】--ucComMF522Buf[MAXRLEN]
            }
        }
        else
        {
            status = MI_ERR;
        } //错误标志
    }

    SetBitMask(ControlReg, 0x80);     //停止定时器
    WriteRawRC(CommandReg, PCD_IDLE); //复位COMMANDREG
    return status;
}
/////////////////////////////////////////////////////////////////////
//功    能：寻卡
//参数说明: req_code[IN]:寻卡方式
//                0x52 = 寻感应区内所有符合14443A标准的卡
//                0x26 = 寻未进入休眠状态的卡
//          pTagType[OUT]：卡片类型代码
//                0x4400 = Mifare_UltraLight
//                0x0400 = Mifare_One(S50)
//                0x0200 = Mifare_One(S70)
//                0x0800 = Mifare_Pro(X)
//                0x4403 = Mifare_DESFire
//返    回: 成功返回MI_OK
/////////////////////////////////////////////////////////////////////
char PcdRequest(unsigned char req_code, unsigned char *pTagType)
{
    char status;
    unsigned int unLen;
    unsigned char ucComMF522Buf[MAXRLEN];

    ClearBitMask(Status2Reg, 0x08);  //清零Status2Reg的MFAuthent Command执行成功标志位
    WriteRawRC(BitFramingReg, 0x07); //清零Transceive命令开始位
    SetBitMask(TxControlReg, 0x03);  //开启天线
    ucComMF522Buf[0] = req_code;     //取522要执行的命令

    status = PcdComMF522(PCD_TRANSCEIVE, ucComMF522Buf, 1, ucComMF522Buf, &unLen); //向PICC发送寻天线区内全部卡命令，并接收PICC返回的数据

    if ((status == MI_OK) && (unLen == 0x10)) //没有错误并接接收为2个字节
    {
        *pTagType = ucComMF522Buf[0];       //取接收缓冲区的第一个字节
        *(pTagType + 1) = ucComMF522Buf[1]; //取接收缓冲区的第二个字节
    }
    else
    {
        status = MI_ERR;
    } //错误

    return status;
}
/*******************************************************************************/
/////////////////////////////////////////////////////////////////////
//功    能：防冲撞
//参数说明: pSnr[OUT]:卡片序列号，4字节
//返    回: 成功返回MI_OK
/////////////////////////////////////////////////////////////////////
char PcdAnticoll(unsigned char *pSnr)
{
    char status;
    unsigned char i, snr_check = 0;
    unsigned int unLen;
    unsigned char ucComMF522Buf[MAXRLEN];

    ClearBitMask(Status2Reg, 0x08);    //清除标志位
    WriteRawRC(BitFramingReg, 0x00);   //000 指示最后一个字节的所有位将被发送。
    ClearBitMask(CollReg, 0x80);       //发生碰撞所有接收位将被清除
    ucComMF522Buf[0] = PICC_ANTICOLL1; //0x93 防冲撞 发到卡里的命令
    ucComMF522Buf[1] = 0x20;

    status = PcdComMF522(PCD_TRANSCEIVE, ucComMF522Buf, 2, ucComMF522Buf, &unLen); //获得卡的序列号，ucComMF522Buf[]
    if (status == MI_OK)
    {
        for (i = 0; i < 4; i++)
        {
            *(pSnr + i) = ucComMF522Buf[i]; //返回卡的序列号
            snr_check ^= ucComMF522Buf[i];  //计算校验码
        }
        if (snr_check != ucComMF522Buf[i])
        {
            status = MI_ERR;
        } //有错误
    }

    SetBitMask(CollReg, 0x80); //置位防碰撞位
    return status;
}
//用MF522计算CRC16函数
/////////////////////////////////////////////////////////////////////
void CalulateCRC(unsigned char *pIndata, unsigned char len, unsigned char *pOutData)
{
    unsigned char i, n;
    ClearBitMask(DivIrqReg, 0x04);    //禁止CRC
    WriteRawRC(CommandReg, PCD_IDLE); //复位522CommandReg寄存器
    SetBitMask(FIFOLevelReg, 0x80);   // 复位FIFO的读写指针
    for (i = 0; i < len; i++)
    {
        WriteRawRC(FIFODataReg, *(pIndata + i));
    }                                    //把*pIndata缓冲区的值写如FIFODataReg
    WriteRawRC(CommandReg, PCD_CALCCRC); //执行CRC校验
    i = 0xFF;                            //等待255us
    do
    {
        n = ReadRawRC(DivIrqReg); //读中断请求标志寄存器
        i--;
    } while ((i != 0) && !(n & 0x04));
    pOutData[0] = ReadRawRC(CRCResultRegL); //CRC校验的低8位
    pOutData[1] = ReadRawRC(CRCResultRegM); //CRC校验的高8位
}
/////////////////////////////////////////////////////////////////////
//功    能：选定卡片
//参数说明: pSnr[IN]:卡片序列号，4字节
//返    回: 成功返回MI_OK
/////////////////////////////////////////////////////////////////////
char PcdSelect(unsigned char *pSnr)
{
    char status = 0XFF;
    unsigned char i;
    unsigned int unLen;
    unsigned char ucComMF522Buf[MAXRLEN]; //MAXRLEN = 18

    ucComMF522Buf[0] = PICC_ANTICOLL1; //防冲撞命令
    ucComMF522Buf[1] = 0x70;
    ucComMF522Buf[6] = 0;
    for (i = 0; i < 4; i++)
    {
        ucComMF522Buf[i + 2] = *(pSnr + i); //填充卡的序列号
        ucComMF522Buf[6] ^= *(pSnr + i);    //计算校验码
    }
    CalulateCRC(ucComMF522Buf, 7, &ucComMF522Buf[7]); //获得CRC校验结果的16位值
                                                      //放入ucComMF522Buf【0，1】
    ClearBitMask(Status2Reg, 0x08);                   //清零MFAuthent Command执行成功标志位

    status = PcdComMF522(PCD_TRANSCEIVE, ucComMF522Buf, 9, ucComMF522Buf, &unLen); //把CRC值和卡号发的卡里

    if ((status == MI_OK) && (unLen == 0x18)) //返回24个字节&状态为无错误
    {
        status = MI_OK;
    }
    else
    {
        status = MI_ERR;
    }

    return status;
}
/////////////////////////////////////////////////////////////////////
//功    能：验证卡片密码
//参数说明: auth_mode[IN]: 密码验证模式
//                 0x60 = 验证A密钥
//                 0x61 = 验证B密钥
//          addr[IN]：块地址
//          pKey[IN]：密码
//          pSnr[IN]：卡片序列号，4字节
//返    回: 成功返回MI_OK
/////////////////////////////////////////////////////////////////////
char PcdAuthState(unsigned char auth_mode, unsigned char addr, unsigned char *pKey, unsigned char *pSnr)
{
    char status;
    unsigned int unLen;
    unsigned char i, ucComMF522Buf[MAXRLEN];

    ucComMF522Buf[0] = auth_mode; //PICC验证A密钥指令
    ucComMF522Buf[1] = addr;      //块地址
    for (i = 0; i < 6; i++)
    {
        ucComMF522Buf[i + 2] = *(pKey + i);
    } //向缓冲区填充密码
    for (i = 0; i < 6; i++)
    {
        ucComMF522Buf[i + 8] = *(pSnr + i);
    } //向缓冲区填充与密码对应的卡的序列号，有效4个字节
      //   memcpy(&ucComMF522Buf[2], pKey, 6);
      //   memcpy(&ucComMF522Buf[8], pSnr, 4);

    status = PcdComMF522(PCD_AUTHENT, ucComMF522Buf, 12, ucComMF522Buf, &unLen); //验证密码和卡号
    if ((status != MI_OK) || (!(ReadRawRC(Status2Reg) & 0x08)))                  //密码验证是否成功
    {
        status = MI_ERR;
    }

    return status;
}
/////////////////////////////////////////////////////////////////////
//功    能：读取M1卡一块数据
//参数说明: addr[IN]：块地址
//          pData[OUT]：读出的数据，16字节
//返    回: 成功返回MI_OK
/////////////////////////////////////////////////////////////////////
char PcdRead(unsigned char addr, unsigned char *pData)
{
    char status;
    unsigned int unLen;
    unsigned char i, ucComMF522Buf[MAXRLEN];

    ucComMF522Buf[0] = PICC_READ;
    ucComMF522Buf[1] = addr;
    CalulateCRC(ucComMF522Buf, 2, &ucComMF522Buf[2]);

    status = PcdComMF522(PCD_TRANSCEIVE, ucComMF522Buf, 4, ucComMF522Buf, &unLen);
    if ((status == MI_OK) && (unLen == 0x90))
    //   {   memcpy(pData, ucComMF522Buf, 16);   }
    {
        for (i = 0; i < 16; i++)
        {
            *(pData + i) = ucComMF522Buf[i];
        }
    }
    else
    {
        status = MI_ERR;
    }

    return status;
}
/////////////////////////////////////////////////////////////////////
//功    能：写数据到M1卡一块
//参数说明: addr[IN]：块地址
//          pData[IN]：写入的数据，16字节
//返    回: 成功返回MI_OK
/////////////////////////////////////////////////////////////////////
char PcdWrite(unsigned char addr, unsigned char *pData)
{
    char status;
    unsigned int unLen;
    unsigned char i, ucComMF522Buf[MAXRLEN];

    ucComMF522Buf[0] = PICC_WRITE; //PICC_WRITE  0xA0   写块
    ucComMF522Buf[1] = addr;
    CalulateCRC(ucComMF522Buf, 2, &ucComMF522Buf[2]); //对数据进行CRC校验
                                                      //存放于ucCoMF522Buf【0，1】
    status = PcdComMF522(PCD_TRANSCEIVE, ucComMF522Buf, 4, ucComMF522Buf, &unLen);

    if ((status != MI_OK) || (unLen != 4) || ((ucComMF522Buf[0] & 0x0F) != 0x0A))
    {
        status = MI_ERR;
    }

    if (status == MI_OK)
    {
        //memcpy(ucComMF522Buf, pData, 16);
        for (i = 0; i < 16; i++)
        {
            ucComMF522Buf[i] = *(pData + i);
        }                                                                               // 要充值的内容
        CalulateCRC(ucComMF522Buf, 16, &ucComMF522Buf[16]);                             //对数据进行CRC校验校验值
                                                                                        //存放于ucCoMF522Buf【0，1】
        status = PcdComMF522(PCD_TRANSCEIVE, ucComMF522Buf, 18, ucComMF522Buf, &unLen); //发送数据，并接收卡返回的数据
        if ((status != MI_OK) || (unLen != 4) || ((ucComMF522Buf[0] & 0x0F) != 0x0A))
        {
            status = MI_ERR;
        }
    }

    return status;
}
/////////////////////////////////////////////////////////////////////
//功    能：命令卡片进入休眠状态
//返    回: 成功返回MI_OK
/////////////////////////////////////////////////////////////////////
char PcdHalt(void)
{
    //    char status;
    unsigned int unLen;
    unsigned char ucComMF522Buf[MAXRLEN];

    ucComMF522Buf[0] = PICC_HALT;
    ucComMF522Buf[1] = 0;
    CalulateCRC(ucComMF522Buf, 2, &ucComMF522Buf[2]);

    //    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);
    PcdComMF522(PICC_HALT, ucComMF522Buf, 4, ucComMF522Buf, &unLen);
    return MI_OK;
}
/////////////////////////////////////////////////////////////////////
//功    能：扣款和充值
//参数说明: dd_mode[IN]：命令字
//               0xC0 = 扣款
//               0xC1 = 充值
//          addr[IN]：钱包地址
//          pValue[IN]：4字节增(减)值，低位在前
//返    回: 成功返回MI_OK
/////////////////////////////////////////////////////////////////////
char PcdValue(unsigned char dd_mode, unsigned char addr, unsigned char *pValue)
{
    char status;
    unsigned int unLen;
    unsigned char i, ucComMF522Buf[MAXRLEN];

    ucComMF522Buf[0] = dd_mode;
    ucComMF522Buf[1] = addr;
    CalulateCRC(ucComMF522Buf, 2, &ucComMF522Buf[2]);

    status = PcdComMF522(PCD_TRANSCEIVE, ucComMF522Buf, 4, ucComMF522Buf, &unLen);

    if ((status != MI_OK) || (unLen != 4) || ((ucComMF522Buf[0] & 0x0F) != 0x0A))
    {
        status = MI_ERR;
    }

    if (status == MI_OK)
    {
        // memcpy(ucComMF522Buf, pValue, 4);
        for (i = 0; i < 16; i++)
        {
            ucComMF522Buf[i] = *(pValue + i);
        }
        CalulateCRC(ucComMF522Buf, 4, &ucComMF522Buf[4]);
        unLen = 0;
        status = PcdComMF522(PCD_TRANSCEIVE, ucComMF522Buf, 6, ucComMF522Buf, &unLen);
        if (status != MI_ERR)
        {
            status = MI_OK;
        }
    }

    if (status == MI_OK)
    {
        ucComMF522Buf[0] = PICC_TRANSFER;
        ucComMF522Buf[1] = addr;
        CalulateCRC(ucComMF522Buf, 2, &ucComMF522Buf[2]);

        status = PcdComMF522(PCD_TRANSCEIVE, ucComMF522Buf, 4, ucComMF522Buf, &unLen);

        if ((status != MI_OK) || (unLen != 4) || ((ucComMF522Buf[0] & 0x0F) != 0x0A))
        {
            status = MI_ERR;
        }
    }
    return status;
}
/////////////////////////////////////////////////////////////////////
//功    能：备份钱包
//参数说明: sourceaddr[IN]：源地址
//          goaladdr[IN]：目标地址
//返    回: 成功返回MI_OK
/////////////////////////////////////////////////////////////////////
char PcdBakValue(unsigned char sourceaddr, unsigned char goaladdr)
{
    char status;
    unsigned int unLen;
    unsigned char ucComMF522Buf[MAXRLEN];

    ucComMF522Buf[0] = PICC_RESTORE;
    ucComMF522Buf[1] = sourceaddr;
    CalulateCRC(ucComMF522Buf, 2, &ucComMF522Buf[2]);

    status = PcdComMF522(PCD_TRANSCEIVE, ucComMF522Buf, 4, ucComMF522Buf, &unLen);

    if ((status != MI_OK) || (unLen != 4) || ((ucComMF522Buf[0] & 0x0F) != 0x0A))
    {
        status = MI_ERR;
    }

    if (status == MI_OK)
    {
        ucComMF522Buf[0] = 0;
        ucComMF522Buf[1] = 0;
        ucComMF522Buf[2] = 0;
        ucComMF522Buf[3] = 0;
        CalulateCRC(ucComMF522Buf, 4, &ucComMF522Buf[4]);

        status = PcdComMF522(PCD_TRANSCEIVE, ucComMF522Buf, 6, ucComMF522Buf, &unLen);
        if (status != MI_ERR)
        {
            status = MI_OK;
        }
    }

    if (status != MI_OK)
    {
        return MI_ERR;
    }

    ucComMF522Buf[0] = PICC_TRANSFER;
    ucComMF522Buf[1] = goaladdr;

    CalulateCRC(ucComMF522Buf, 2, &ucComMF522Buf[2]);

    status = PcdComMF522(PCD_TRANSCEIVE, ucComMF522Buf, 4, ucComMF522Buf, &unLen);

    if ((status != MI_OK) || (unLen != 4) || ((ucComMF522Buf[0] & 0x0F) != 0x0A))
    {
        status = MI_ERR;
    }

    return status;
}
/************************************************************************
				GPIO——RC522初始化
****************************************************************************/
void GpioRc522(void)
{
    LPC_IOCON->JTAG_TMS_PIO1_0 = (LPC_IOCON->JTAG_TMS_PIO1_0 & (~0x07) | 0x1);      //enable p1.0 gpio mode
    LPC_IOCON->JTAG_TDO_PIO1_1 = (LPC_IOCON->JTAG_TDO_PIO1_1 & (~0x07) | 0x1);      //enable p1.1 gpio mode
    LPC_IOCON->JTAG_nTRST_PIO1_2 = (LPC_IOCON->JTAG_nTRST_PIO1_2 & (~0xff) | 0x81); //enable p1.2 gpio mode
    LPC_IOCON->JTAG_TDI_PIO0_11 = (LPC_IOCON->JTAG_TDI_PIO0_11 & (~0x07) | 0x1);    //enable p0.11 gpio mode
    GPIOSetDir(PORT1, RC522_SCK, 1);                                                //set gpio output mode ,port 1, bit 0
    GPIOSetDir(PORT0, RC522_NSS, 1);                                                //set gpio output mode ,port 0, bit  11
    GPIOSetDir(PORT1, RC522_SI, 1);                                                 //set gpio output mode ,port 1, bit  1
    GPIOSetDir(PORT1, RC522_SO, 0);                                                 //set gpio input mode ,port 1, bit 0
    GPIOSetDir(PORT0, RC522_RESET, 1);                                              //RC522 RESET BIT

    GPIOSetDir(PORT3, 4, 1);
    GPIOSetDir(PORT3, 5, 1);
    GPIOSetDir(PORT1, 4, 1);
}
