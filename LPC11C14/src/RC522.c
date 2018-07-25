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
//��    �ܣ���RC522�Ĵ���λ
//����˵����reg[IN]:�Ĵ�����ַ
//          mask[IN]:��λֵ
/////////////////////////////////////////////////////////////////////
void ClearBitMask(unsigned char reg, unsigned char mask)
{
    char tmp = 0x0;
    tmp = ReadRawRC(reg);         //��Reg�Ĵ�����״̬
    WriteRawRC(reg, tmp & ~mask); //��Reg�Ĵ���д��tmp&~maskλ
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
//��    �ܣ���RC522�Ĵ���λ
//����˵����reg[IN]:�Ĵ�����ַ
//          mask[IN]:��λֵ
/////////////////////////////////////////////////////////////////////
void SetBitMask(unsigned char reg, unsigned char mask)
{
    char tmp = 0x0;
    tmp = ReadRawRC(reg);        //��Reg�Ĵ�����״̬
    WriteRawRC(reg, tmp | mask); //��Reg�Ĵ���д�� MASK|tmpλ
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
//��    �ܣ�ͨ��RC522��ISO14443��ͨѶ
//����˵����Command[IN]:RC522������
//          pInData[IN]:ͨ��RC522���͵���Ƭ������
//          InLenByte[IN]:�������ݵ��ֽڳ���
//          pOutData[OUT]:���յ��Ŀ�Ƭ��������
//          *pOutLenBit[OUT]:�������ݵ�λ����
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
    case PCD_AUTHENT:   // 0x0E   ��֤��Կ
        irqEn = 0x12;   // 0001 0010
        waitFor = 0x10; // 0001 0000
        break;
    case PCD_TRANSCEIVE: //0x0C   ���Ͳ���������
        irqEn = 0x77;    // 0111 0111
        waitFor = 0x30;  // 0011 0000
        break;
    default:
        break;
    }

    WriteRawRC(ComIEnReg, irqEn | 0x80); //ʹ����Ӧ�ж�
    ClearBitMask(ComIrqReg, 0x80);       //���������жϱ�־
    WriteRawRC(CommandReg, PCD_IDLE);    //����Ĵ�������
    SetBitMask(FIFOLevelReg, 0x80);      //��λFIFO��дָ��

    for (i = 0; i < InLenByte; i++)
    {
        WriteRawRC(FIFODataReg, pInData[i]);
    }                                //��Ҫ���͸���������д��522��FIFO���ݼĴ���
    WriteRawRC(CommandReg, Command); //522ִ�з�������

    if (Command == PCD_TRANSCEIVE) //�Ƿ�Ϊ�������ݵ�����
    {
        SetBitMask(BitFramingReg, 0x80);
    } //ִ�з����������� ���뷢������������Ч���

    i = 500; //����ʱ��Ƶ�ʵ���������M1�����ȴ�ʱ��25ms
    do
    {
        n = ReadRawRC(ComIrqReg); // ���жϱ�־�Ĵ���
        i--;
    } while ((i != 0) && !(n & 0x01) && !(n & waitFor)); //�˳���IRQ����:1. i=0 2.��ʱ���������ж�3.��������⵽��Ч�������ж�OR�������ֹ�ж�

    ClearBitMask(BitFramingReg, 0x80); //����������������

    if (i != 0) //��Ƶ���޿�
    {

        if (!(ReadRawRC(ErrorReg) & 0x1B)) //�������־�Ĵ���&0001 1011����һ�´�������FIFO��������ײ������żУ�����SOF����
        {
            status = MI_OK;       //��ʱ�޴���
            if (n & irqEn & 0x01) //�Ƿ�Ϊ��ʱ���ж�
            {
                status = MI_NOTAGERR;
            } //����
            if (Command == PCD_TRANSCEIVE)
            {
                n = ReadRawRC(FIFOLevelReg);             //�������˼����ֽ�
                lastBits = ReadRawRC(ControlReg) & 0x07; //���յ����һ���ֽڵ���Чλ
                if (lastBits)
                {
                    *pOutLenBit = (n - 1) * 8 + lastBits;
                } //N*8+LASTBITSλ
                else
                {
                    *pOutLenBit = n * 8;
                } //N*8λ
                if (n == 0)
                {
                    n = 1;
                }
                if (n > MAXRLEN)
                {
                    n = MAXRLEN;
                } //���ɽ���18���ֽ�
                for (i = 0; i < n; i++)
                {
                    pOutData[i] = ReadRawRC(FIFODataReg);
                } //ȡ�����յ������ݷŵ�����POUTDATA����--ucComMF522Buf[MAXRLEN]
            }
        }
        else
        {
            status = MI_ERR;
        } //�����־
    }

    SetBitMask(ControlReg, 0x80);     //ֹͣ��ʱ��
    WriteRawRC(CommandReg, PCD_IDLE); //��λCOMMANDREG
    return status;
}
/////////////////////////////////////////////////////////////////////
//��    �ܣ�Ѱ��
//����˵��: req_code[IN]:Ѱ����ʽ
//                0x52 = Ѱ��Ӧ�������з���14443A��׼�Ŀ�
//                0x26 = Ѱδ��������״̬�Ŀ�
//          pTagType[OUT]����Ƭ���ʹ���
//                0x4400 = Mifare_UltraLight
//                0x0400 = Mifare_One(S50)
//                0x0200 = Mifare_One(S70)
//                0x0800 = Mifare_Pro(X)
//                0x4403 = Mifare_DESFire
//��    ��: �ɹ�����MI_OK
/////////////////////////////////////////////////////////////////////
char PcdRequest(unsigned char req_code, unsigned char *pTagType)
{
    char status;
    unsigned int unLen;
    unsigned char ucComMF522Buf[MAXRLEN];

    ClearBitMask(Status2Reg, 0x08);  //����Status2Reg��MFAuthent Commandִ�гɹ���־λ
    WriteRawRC(BitFramingReg, 0x07); //����Transceive���ʼλ
    SetBitMask(TxControlReg, 0x03);  //��������
    ucComMF522Buf[0] = req_code;     //ȡ522Ҫִ�е�����

    status = PcdComMF522(PCD_TRANSCEIVE, ucComMF522Buf, 1, ucComMF522Buf, &unLen); //��PICC����Ѱ��������ȫ�������������PICC���ص�����

    if ((status == MI_OK) && (unLen == 0x10)) //û�д��󲢽ӽ���Ϊ2���ֽ�
    {
        *pTagType = ucComMF522Buf[0];       //ȡ���ջ������ĵ�һ���ֽ�
        *(pTagType + 1) = ucComMF522Buf[1]; //ȡ���ջ������ĵڶ����ֽ�
    }
    else
    {
        status = MI_ERR;
    } //����

    return status;
}
/*******************************************************************************/
/////////////////////////////////////////////////////////////////////
//��    �ܣ�����ײ
//����˵��: pSnr[OUT]:��Ƭ���кţ�4�ֽ�
//��    ��: �ɹ�����MI_OK
/////////////////////////////////////////////////////////////////////
char PcdAnticoll(unsigned char *pSnr)
{
    char status;
    unsigned char i, snr_check = 0;
    unsigned int unLen;
    unsigned char ucComMF522Buf[MAXRLEN];

    ClearBitMask(Status2Reg, 0x08);    //�����־λ
    WriteRawRC(BitFramingReg, 0x00);   //000 ָʾ���һ���ֽڵ�����λ�������͡�
    ClearBitMask(CollReg, 0x80);       //������ײ���н���λ�������
    ucComMF522Buf[0] = PICC_ANTICOLL1; //0x93 ����ײ �������������
    ucComMF522Buf[1] = 0x20;

    status = PcdComMF522(PCD_TRANSCEIVE, ucComMF522Buf, 2, ucComMF522Buf, &unLen); //��ÿ������кţ�ucComMF522Buf[]
    if (status == MI_OK)
    {
        for (i = 0; i < 4; i++)
        {
            *(pSnr + i) = ucComMF522Buf[i]; //���ؿ������к�
            snr_check ^= ucComMF522Buf[i];  //����У����
        }
        if (snr_check != ucComMF522Buf[i])
        {
            status = MI_ERR;
        } //�д���
    }

    SetBitMask(CollReg, 0x80); //��λ����ײλ
    return status;
}
//��MF522����CRC16����
/////////////////////////////////////////////////////////////////////
void CalulateCRC(unsigned char *pIndata, unsigned char len, unsigned char *pOutData)
{
    unsigned char i, n;
    ClearBitMask(DivIrqReg, 0x04);    //��ֹCRC
    WriteRawRC(CommandReg, PCD_IDLE); //��λ522CommandReg�Ĵ���
    SetBitMask(FIFOLevelReg, 0x80);   // ��λFIFO�Ķ�дָ��
    for (i = 0; i < len; i++)
    {
        WriteRawRC(FIFODataReg, *(pIndata + i));
    }                                    //��*pIndata��������ֵд��FIFODataReg
    WriteRawRC(CommandReg, PCD_CALCCRC); //ִ��CRCУ��
    i = 0xFF;                            //�ȴ�255us
    do
    {
        n = ReadRawRC(DivIrqReg); //���ж������־�Ĵ���
        i--;
    } while ((i != 0) && !(n & 0x04));
    pOutData[0] = ReadRawRC(CRCResultRegL); //CRCУ��ĵ�8λ
    pOutData[1] = ReadRawRC(CRCResultRegM); //CRCУ��ĸ�8λ
}
/////////////////////////////////////////////////////////////////////
//��    �ܣ�ѡ����Ƭ
//����˵��: pSnr[IN]:��Ƭ���кţ�4�ֽ�
//��    ��: �ɹ�����MI_OK
/////////////////////////////////////////////////////////////////////
char PcdSelect(unsigned char *pSnr)
{
    char status = 0XFF;
    unsigned char i;
    unsigned int unLen;
    unsigned char ucComMF522Buf[MAXRLEN]; //MAXRLEN = 18

    ucComMF522Buf[0] = PICC_ANTICOLL1; //����ײ����
    ucComMF522Buf[1] = 0x70;
    ucComMF522Buf[6] = 0;
    for (i = 0; i < 4; i++)
    {
        ucComMF522Buf[i + 2] = *(pSnr + i); //��俨�����к�
        ucComMF522Buf[6] ^= *(pSnr + i);    //����У����
    }
    CalulateCRC(ucComMF522Buf, 7, &ucComMF522Buf[7]); //���CRCУ������16λֵ
                                                      //����ucComMF522Buf��0��1��
    ClearBitMask(Status2Reg, 0x08);                   //����MFAuthent Commandִ�гɹ���־λ

    status = PcdComMF522(PCD_TRANSCEIVE, ucComMF522Buf, 9, ucComMF522Buf, &unLen); //��CRCֵ�Ϳ��ŷ��Ŀ���

    if ((status == MI_OK) && (unLen == 0x18)) //����24���ֽ�&״̬Ϊ�޴���
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
//��    �ܣ���֤��Ƭ����
//����˵��: auth_mode[IN]: ������֤ģʽ
//                 0x60 = ��֤A��Կ
//                 0x61 = ��֤B��Կ
//          addr[IN]�����ַ
//          pKey[IN]������
//          pSnr[IN]����Ƭ���кţ�4�ֽ�
//��    ��: �ɹ�����MI_OK
/////////////////////////////////////////////////////////////////////
char PcdAuthState(unsigned char auth_mode, unsigned char addr, unsigned char *pKey, unsigned char *pSnr)
{
    char status;
    unsigned int unLen;
    unsigned char i, ucComMF522Buf[MAXRLEN];

    ucComMF522Buf[0] = auth_mode; //PICC��֤A��Կָ��
    ucComMF522Buf[1] = addr;      //���ַ
    for (i = 0; i < 6; i++)
    {
        ucComMF522Buf[i + 2] = *(pKey + i);
    } //�򻺳����������
    for (i = 0; i < 6; i++)
    {
        ucComMF522Buf[i + 8] = *(pSnr + i);
    } //�򻺳�������������Ӧ�Ŀ������кţ���Ч4���ֽ�
      //   memcpy(&ucComMF522Buf[2], pKey, 6);
      //   memcpy(&ucComMF522Buf[8], pSnr, 4);

    status = PcdComMF522(PCD_AUTHENT, ucComMF522Buf, 12, ucComMF522Buf, &unLen); //��֤����Ϳ���
    if ((status != MI_OK) || (!(ReadRawRC(Status2Reg) & 0x08)))                  //������֤�Ƿ�ɹ�
    {
        status = MI_ERR;
    }

    return status;
}
/////////////////////////////////////////////////////////////////////
//��    �ܣ���ȡM1��һ������
//����˵��: addr[IN]�����ַ
//          pData[OUT]�����������ݣ�16�ֽ�
//��    ��: �ɹ�����MI_OK
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
//��    �ܣ�д���ݵ�M1��һ��
//����˵��: addr[IN]�����ַ
//          pData[IN]��д������ݣ�16�ֽ�
//��    ��: �ɹ�����MI_OK
/////////////////////////////////////////////////////////////////////
char PcdWrite(unsigned char addr, unsigned char *pData)
{
    char status;
    unsigned int unLen;
    unsigned char i, ucComMF522Buf[MAXRLEN];

    ucComMF522Buf[0] = PICC_WRITE; //PICC_WRITE  0xA0   д��
    ucComMF522Buf[1] = addr;
    CalulateCRC(ucComMF522Buf, 2, &ucComMF522Buf[2]); //�����ݽ���CRCУ��
                                                      //�����ucCoMF522Buf��0��1��
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
        }                                                                               // Ҫ��ֵ������
        CalulateCRC(ucComMF522Buf, 16, &ucComMF522Buf[16]);                             //�����ݽ���CRCУ��У��ֵ
                                                                                        //�����ucCoMF522Buf��0��1��
        status = PcdComMF522(PCD_TRANSCEIVE, ucComMF522Buf, 18, ucComMF522Buf, &unLen); //�������ݣ������տ����ص�����
        if ((status != MI_OK) || (unLen != 4) || ((ucComMF522Buf[0] & 0x0F) != 0x0A))
        {
            status = MI_ERR;
        }
    }

    return status;
}
/////////////////////////////////////////////////////////////////////
//��    �ܣ����Ƭ��������״̬
//��    ��: �ɹ�����MI_OK
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
//��    �ܣ��ۿ�ͳ�ֵ
//����˵��: dd_mode[IN]��������
//               0xC0 = �ۿ�
//               0xC1 = ��ֵ
//          addr[IN]��Ǯ����ַ
//          pValue[IN]��4�ֽ���(��)ֵ����λ��ǰ
//��    ��: �ɹ�����MI_OK
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
//��    �ܣ�����Ǯ��
//����˵��: sourceaddr[IN]��Դ��ַ
//          goaladdr[IN]��Ŀ���ַ
//��    ��: �ɹ�����MI_OK
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
				GPIO����RC522��ʼ��
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
