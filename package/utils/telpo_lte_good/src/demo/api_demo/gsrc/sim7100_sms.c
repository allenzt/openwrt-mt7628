#include "sim7100_sms.h"

/************************************************************************************************************************* 
*����         :   u32 GSM_StringToDec(char *pStr, u8 NumDigits) 
*����         :   ��10������ʽ�ַ���ת��Ϊ������(���뱣֤��ȫΪ�����ַ�) 
*����         :   pStr:�ַ�����ʼָ�� 
*                   NumDigits:����λ��,10��������λ�� 
*����         :   ת��������� 
*����         :   �� 
*����         :   cp1300@139.com 
*ʱ��         :   2013-04-30 
*����޸�ʱ�� :   2013-04-30 
*˵��         :   �����ַ���"1865"ת����Ϊ1865,λ��Ϊ4λ 
                    ���뱣֤��ȫΪ�����ַ� 
*************************************************************************************************************************/  
u32 GSM_StringToDec(char *pStr, u8 NumDigits)  
{  
    u32 temp;  
    u32 DEC = 0;  
    u8 i;  
    u8 j;  
      
    NumDigits = (NumDigits > 10) ? 10 : NumDigits;   //���֧��10λ10������  
      
    for(i = 0;i < NumDigits;i ++)  
    {  
        temp = pStr[i] - '0';  
        if(temp > 9)         //ֻ�������ַ�Χ  
            return 0;  
        for(j = 1;j < (NumDigits - i);j ++)  
        {  
            temp *= 10;  
        }  
        DEC += temp;  
    }  
    return DEC;  
}  

/************************************************************************************************************************* 
*����         :   u32 GSM_StringToHex(char *pStr, u8 NumDigits) 
*����         :   ��16������ʽ�ַ���ת��Ϊ16����������(���뱣֤�ַ�����ĸ���Ǵ�д) 
*����         :   pStr:�ַ�����ʼָ�� 
*                   NumDigits:����λ��,16��������λ�� 
*����         :   ת��������� 
*����         :   �� 
*����         :   cp1300@139.com 
*ʱ��         :   2013-04-30 
*����޸�ʱ�� :   2013-10-17 
*˵��         :   �����ַ���"A865"ת����Ϊ0xA865,λ��Ϊ4λ 
                    ���뱣֤�ַ�����ĸ���Ǵ�д 
*************************************************************************************************************************/  
static u32 GSM_StringToHex(char *pStr, u8 NumDigits)  
{  
    u8 temp;  
    u32 HEX = 0;  
    u8 i;  
      
    NumDigits = (NumDigits > 8) ? 8 : NumDigits; //���֧��8λ16������  
      
    for(i = 0;i < NumDigits;i ++)  
    {  
        HEX <<= 4;  
        temp = pStr[i];  
        temp = (temp > '9') ? temp - 'A' + 10 : temp - '0';  
        HEX |= temp;  
    }  
    return HEX;  
}  

/************************************************************************************************************************* 
*����         :   static u8 ChartoPhoneNum(char *pChar, char *pNum, u8 CharLen) 
*����         :   ���ַ�ת��Ϊ�绰���� 
*����         :   pCHAR:�ַ�������ָ�� 
*                   pNum:�绰����ָ�� 
*                   charLen:�ַ����볤�� 
*����         :   �绰���� 
*����         :   �ײ�궨�� 
*����         :   cp1300@139.com 
*ʱ��         :   2013-04-04 
*����޸�ʱ�� :   2013-10-17 
*˵��         :   ��Ҫ���ڵ绰����,�������ĺ���ת�� 
*                   ����绰����Ϊ����,�򽫲���ż��,�������һ���ַ��ĸ�λ����λ0xf; 
*                   ת����Ϊ�ַ� 
*************************************************************************************************************************/  
static u8 ChartoPhoneNum(char *pChar, char *pNum, u8 CharLen)  
{  
    u32 i;  
    u8 temp;  
  
    for(i = 0;i < CharLen;i ++)  
    {  
        temp = pChar[i];  
        if(temp == 'F') //����һλ�ͽ�����  
        {  
            pNum[i] =   pChar[i+1];   
            return i + 1;  
        }  
        else if(temp > '9')  //������  
        {  
            return 0;   //�绰�����ʽ����  
        }  
           
        else if(i % 2)  //λ��Ϊ����  
            pNum[i-1] =  temp;  
        else    //λ��Ϊż��  
            pNum[i+1] = temp;     
    }  
    return i;  
}  

// 7-bit����  
// pSrc: Դ�ַ���ָ��  
// pDst: Ŀ����봮ָ��  
// nSrcLength: Դ�ַ�������  
// ����: Ŀ����봮����  
static int gsmEncode7bit(const char* pSrc,u8* pDst)  
{  
    int nSrc;        // Դ�ַ����ļ���ֵ  
    int nDst;        // Ŀ����봮�ļ���ֵ  
    int nChar;       // ��ǰ���ڴ���������ַ��ֽڵ���ţ���Χ��0-7  
    unsigned char nLeft=0;    // ��һ�ֽڲ��������  
    int nSrcLength = strlen(pSrc);  
  
    // ����ֵ��ʼ��  
    nSrc = 0;  
    nDst = 0;  
  
    // ��Դ��ÿ8���ֽڷ�Ϊһ�飬ѹ����7���ֽ�  
    // ѭ���ô�����̣�ֱ��Դ����������  
    // ������鲻��8�ֽڣ�Ҳ����ȷ����  
    while(nSrc<nSrcLength)  
    {  
        // ȡԴ�ַ����ļ���ֵ�����3λ  
        nChar = nSrc & 7;  
  
        // ����Դ����ÿ���ֽ�  
        if(nChar == 0)  
        {  
            // ���ڵ�һ���ֽڣ�ֻ�Ǳ�����������������һ���ֽ�ʱʹ��  
            nLeft = *pSrc;  
        }  
        else  
        {  
            // ���������ֽڣ������ұ߲��������������ӣ��õ�һ��Ŀ������ֽ�  
            *pDst = (*pSrc << (8-nChar)) + nLeft;  
  
            // �����ֽ�ʣ�µ���߲��֣���Ϊ�������ݱ�������  
            nLeft = *pSrc >> nChar;  
            // �޸�Ŀ�괮��ָ��ͼ���ֵ pDst++;  
            //SIM900_debug("%c",*pDst);  
            pDst++;  nDst++;  
        }  
        // �޸�Դ����ָ��ͼ���ֵ  
        pSrc++; nSrc++;  
    }  
  
    //Nleft����ʣ�࣬��Ҫһ���Լ�������  
    nChar = nSrc & 7;  
    if(nChar != 0)  
    {  
        *pDst=nLeft;  
        nDst++;   
        pDst++;  
    }  
    //*pDst='\0';  
  
    // ����Ŀ�괮����  
    return nDst;  
}  

// 7-bit����  
// pSrc: Դ���봮ָ��,7bit����  
// pDst: Ŀ���ַ���ָ��  
// nSrcLength: Դ���봮����  
// ����: Ŀ���ַ�������  
static int gsmDecode7bit(const u8 *pSrc, char *pDst, int nSrcLength)  
{  
    int nSrc;        // Դ�ַ����ļ���ֵ  
    int nDst;        // Ŀ����봮�ļ���ֵ  
    int nByte;       // ��ǰ���ڴ���������ֽڵ���ţ���Χ��0-6  
    unsigned char nLeft;    // ��һ�ֽڲ��������  
          
    // ����ֵ��ʼ��  
    nSrc = 0;  
    nDst = 0;  
      
    // �����ֽ���źͲ������ݳ�ʼ��  
    nByte = 0;  
    nLeft = 0;  
      
    // ��Դ����ÿ7���ֽڷ�Ϊһ�飬��ѹ����8���ֽ�  
    // ѭ���ô�����̣�ֱ��Դ���ݱ�������  
    // ������鲻��7�ֽڣ�Ҳ����ȷ����  
      
    while(nSrc<nSrcLength)  
    {  
        // ��Դ�ֽ��ұ߲��������������ӣ�ȥ�����λ���õ�һ��Ŀ������ֽ�  
        *pDst = (((*pSrc) << nByte) | nLeft) & 0x7f;  
        // �����ֽ�ʣ�µ���߲��֣���Ϊ�������ݱ�������  
        nLeft = (*pSrc) >> (7-nByte);  
      
        // �޸�Ŀ�괮��ָ��ͼ���ֵ  
        pDst++;  
        nDst++;  
      
        // �޸��ֽڼ���ֵ  
        nByte++;  
      
        // ����һ������һ���ֽ�  
        if(nByte == 7)  
        {  
            // ����õ�һ��Ŀ������ֽ�  
            *pDst = nLeft;  
      
            // �޸�Ŀ�괮��ָ��ͼ���ֵ  
            pDst++;  
            nDst++;  
      
            // �����ֽ���źͲ������ݳ�ʼ��  
            nByte = 0;  
            nLeft = 0;  
        }  
      
        // �޸�Դ����ָ��ͼ���ֵ  
        pSrc++;  
        nSrc++;  
    }  
      
    *pDst = 0;  //��ӽ�����        
    // ����Ŀ�괮����  
    return nDst;  
}  

/************************************************************************************************************************* 
* ����    :   u16 OneUNICODEtoGBK(u16 unicode) 
* ����    :   ��unicode����ת��ΪGBK���� 
* ����    :   unicode 
* ����    :   GBK  
* ����    :   �ײ��д���� 
* ����    :   cp1300@139.com 
* ʱ��    :   20120602 
* ����޸�ʱ�� : 20120602 
* ˵��    :   ��Ҫflash�е����֧�� 
            GBK�뷶Χ,��8λ:0x81~0xfe;��8λ:0x40~0xfe 
*************************************************************************************************************************/   
u16 OneUNICODEtoGBK(u16 unicode)  //�ö��ֲ����㷨  
{  
    u32 offset;  
    u16 temp;  
    //UINT bw;  
    u8 buff[2];  
  
    if(unicode<=0X9FA5)  
    {  
        if(unicode>=0X4E00)  
            offset=unicode-0X4E00;//0x1b87      //0X4E00,����ƫ�����  
        else  
            return 0x2020;      //������ʾ���ַ��͸������ո����,��������  
    }     
    else if(unicode>0X9FA5)//�Ǳ�����  
    {  
        if(unicode<0XFF01||unicode>0XFF61)  
            return 0x2020;//û�ж�Ӧ����  //������ʾ���ַ��͸������ո����,��������  
        offset=unicode-0XFF01+0X9FA6-0X4E00;      
    }  
    offset *= 2;  
  
  
    //f_lseek(&UtoG_File, offset);                        		 //�ļ�ָ�����ƫ��λ��
    fseek(UtoG_File, offset, SEEK_SET);
    //if(f_read(&UtoG_File, buff, 2, &bw) != FR_OK)   //��ȡ2�ֽ�
    if (fread(buff, 1, 2, UtoG_File) == 0)
    {  
       return 0x2020;  
    }  
  
    temp = buff[0];  
    temp <<= 8;  
    temp += buff[1];  
    return temp;    //�����ҵ��ı���                  
}  
  

/************************************************************************************************************************* 
*����         :   u32 UnicodeStrToString(u8 *pucode,char *pStr,u32 SrtLen) 
*����         :   ���ַ�unicodeת��Ϊ�ַ��� 
*����         :   pucode:ת����������� 
*                   pStr:�ַ�������ָ�� 
*                   SrtLen:�ַ����ֽڳ��� 
*����         :   ת����Ϊ�ַ���ĳ��� 
*����         :   u16 OneUNICODEtoGBK(u16 unicode); 
*����         :   cp1300@139.com 
*ʱ��         :   2013-04-04 
*����޸�ʱ�� :   2013-10-26 
*˵��         :   ���ڽ�PUD��ʽ���Ž���,������������ַ��������� 
                    2013-10-26:��������о���޷����� 
*************************************************************************************************************************/  
static u32 UnicodeStrToString(u8 *pucode,char *pStr,u32 SrtLen)  
{  
    u32 i;  
    u16 temp;  
    u32 cnt = 0;  
    u8 H,L;  
  
    for(i = 0;i < SrtLen;i+=4)  
    {  
        if(pucode[i] == '0')    //0  
        {  
            H = pucode[i+2];  
            L = pucode[i+3];  
            H = (H > '9') ? H - 'A' + 10 : H - '0';  
            L = (L > '9') ? L - 'A' + 10 : L - '0';    
            pStr[cnt++] = (H << 4) + L;     
        }  
        else  
        {  
            H = pucode[i];  
            L = pucode[i+1];  
            H = (H > '9') ? H - 'A' + 10 : H - '0';  
            L = (L > '9') ? L - 'A' + 10 : L - '0';    
            temp = (H << 4) + L;  
            temp <<= 8;  
            H = pucode[i+2];  
            L = pucode[i+3];  
            H = (H > '9') ? H - 'A' + 10 : H - '0';  
            L = (L > '9') ? L - 'A' + 10 : L - '0';    
            temp |= (H << 4) + L;  
            switch(temp)  
            {  
                case 0x3002:    //����޷���ʾ,ת��ΪGBK������  
                    temp = 0xA1A3;
					break;//'��';  break;  
                default :   
                    temp = OneUNICODEtoGBK(temp);
					break; //����ת��  
            }  
            pStr[cnt++] = temp >> 8 ;  //buf[0]
            pStr[cnt++] = temp & 0xff;  //buf[1]
        }  
    }  
    return cnt;  
}  

/************************************************************************************************************************* 
*����         :   bool gsm_parsepdu_sms(char *pPDU,char *pSMS,u32 len,sms_info *pInfo) 
*����         :   ����һ��PDU��ʽ����                           ������
*����         :   pPDU:����PDU���ݻ�����ָ�� 
*                   pSMS:������Ķ��Ż�����ָ�� 
*                   pInfo:������Ϣָ�� 
*����         :   TRUE:�ɹ�;FALSE:ʧ�� 
*����         :   void UnicodeToGBK(u16 *pUnicode, u16 *pGBK, u32 cnt); 
*����         :   cp1300@139.com 
*ʱ��         :   2013-04-04 
*����޸�ʱ�� :   2013-05-01 
*˵��         :   �� 
*************************************************************************************************************************/  
bool gsm_parsepdu_sms(char *pPDU, char *pSMS, sms_info *pInfo)  
{  
    u16 cnt = 0;  
    u16 temp;  
    char *p;  
    u16 SMS_Size;  
    //��ȡ�������ĺ��볤��  
    temp = GSM_StringToHex(&pPDU[cnt], 2);  //��16������ʽ�ַ���ת��Ϊ������  
    cnt += 2;           //����ǰ��Ķ������ĺ��볤���ֽ�  
    cnt += temp*2;      //����ǰ��Ķ���������Ϣ  
      
    //����PDU����  RT  UDHI  SRI -  -  MMS   MTI  MTI  //UDHIΪ1,�����û�������ͷ����Ϣ,���ڱ�ʶ���Ų����Ϣ  
    pInfo->PDU = GSM_StringToHex(&pPDU[cnt], 2); //��16������ʽ�ַ���ת��Ϊ������//PDU����  
    cnt += 2;           //����PDUͷ�����ֽ�  
  
    //���㷢�Ͷ��ŵĺ���ĳ���  
    temp = GSM_StringToHex(&pPDU[cnt], 2);  //��16������ʽ�ַ���ת��Ϊ������  
    cnt += 2;           //�����绰���볤���ֽ�  
    cnt += 2;           //������ַ����,��Ϊ"91",һ�ֽ�  
    pInfo->NumLen = ChartoPhoneNum((char *)&pPDU[cnt], (char *)&(pInfo->NumBuff[0]), (temp > SMS_NUM_LEN_MAX - 2) ? (SMS_NUM_LEN_MAX - 2) : temp); //ת�����ͺ���  
    pInfo->NumBuff[pInfo->NumLen] = 0;    //������  
 
    cnt += (temp%2) ? (temp+1) : temp;  //�������ͺ��볤�ȵ��ֽ���  
    cnt+=2; //����PID,2Byte  
    pInfo->DSC = GSM_StringToHex(&pPDU[cnt], 2); //��ȡDSC��Ϣ  
    cnt+=2; //����DSC,2Byte  
    //cnt+=2;   //����VP,2B   //û�������־  
    //cnt+=2;   //����UDL,2B//û�������־  
  
      
    pInfo->Timer.Year = (pPDU[cnt + 1] - '0') * 10 + (pPDU[cnt] - '0');      cnt += 2;   //��  
    pInfo->Timer.Month = (pPDU[cnt + 1] - '0') * 10 + (pPDU[cnt] - '0'); cnt += 2;   //��
    pInfo->Timer.Day = (pPDU[cnt + 1] - '0') * 10 + (pPDU[cnt] - '0');       cnt += 2;   //��  
    pInfo->Timer.Hour = (pPDU[cnt + 1] - '0') * 10 + (pPDU[cnt] - '0');      cnt += 2;   //ʱ  
    pInfo->Timer.Minute = (pPDU[cnt + 1] - '0') * 10 + (pPDU[cnt] - '0');    cnt += 2;   //��  
    pInfo->Timer.Second = (pPDU[cnt + 1] - '0') * 10 + (pPDU[cnt] - '0');    cnt += 2;   //��                                  
    cnt += 2;   //����ʱ��2�ֽ�  
    SMS_Size = GSM_StringToHex(&pPDU[cnt], 2);      //��������ַ�����,����Ӣ��,���Ķ���һ���ַ�   
    cnt += 2;                                           //�������ų����ֽ�,2B  
  
    if(pInfo->PDU & 0x40)    //�û�������ͷ����Ϣ,��ʶ�����Ѿ����ָ�Ϊ����  
    {  
        cnt += 8;           //����ǰ��8������,ֻҪ�����4��,��ʶ  
        SMS_Size -= 12;     //���ų��ȼ�ȥƫ��  
          
        pInfo->AllNum = GSM_StringToHex(&pPDU[cnt], 2);//�����ָܷ���  
        cnt += 2;       //����2B������  
        pInfo->PreNum = GSM_StringToHex(&pPDU[cnt], 2);//���㵱ǰλ��  
        cnt += 2;       //����2B�ĵ�ǰλ��  
    }  
    else  
    {  
        pInfo->AllNum = pInfo->PreNum = 0;    //����û�б��ָ�  
    }  
      
    //DCS 00:7BIT����;08:UCS2����;04:8bit����  
    switch((pInfo->DSC) & 0x0f)  
    {  
        case 0x00:  //7bit����  
        {  
            pInfo->SMS_Size = (SMS_Size > 160) ? 160 : SMS_Size;      //���Ŵ�С  
            pInfo->TEXT_MODE = 1;      
            SMS_Size = (SMS_Size * 7 / 8) + (((SMS_Size * 7) % 8) ? 1 : 0);//�������ռ�ÿռ��С  
            pPDU += cnt;  
            for(temp = 0;temp < SMS_Size;temp ++)                //��PDU����ת��Ϊ16��������  
            {  
                pPDU[temp] = GSM_StringToHex(&pPDU[temp << 1], 2);    //B����ת��ΪPDU��ʽ��ռ��2B  
            }  
            gsmDecode7bit((u8 *)pPDU, (char *)pSMS, SMS_Size);  //7bit->8bit,���ݳ��Ȼᷢ���仯  
          
        }break;  
        case 0x04:  //8bit����  
        {  
            printf("sms not support\n");
            return FALSE;  
        }  
        case 0x08:  //UCS2����  
        {   
            pInfo->TEXT_MODE = 0;      
            SMS_Size = (SMS_Size > 140) ? 140 : SMS_Size;        //�����ַ�����Ϊ140B  
            //UNICODE PDUת��Ϊ�ַ��� --> GBK,���ض��Ŵ�С,ÿ�������ַ�ռ��2�ֽ�,ÿ���ֽ�ת��ΪPDU��ռ��2B  
            pInfo->SMS_Size = UnicodeStrToString((u8 *)pPDU+cnt,(char *)pSMS,SMS_Size<<1);       
        }break;  
        default:
			printf("unknown format\n");
			return FALSE;  
    }  
  
    pSMS[pInfo->SMS_Size] = '\0';                //��ӽ�����       
      
    return TRUE;  
}

/************************************************************************************************************************* 
* ����                :   static u16 GSM_GetU2SCharOffset(char *pBuff,u16 CharNum) 
* ����                :   ����ָ���ַ���ƫ��λ�� 
* ����                :   pBuff:�ַ���������; 
                        CharNum:�ַ�ƫ�� 
* ����                :   �ַ�����С 
* ����                :   �� 
* ����                :   cp1300@139.com 
* ʱ��                :   2013-10-25 
* ����޸�ʱ��    :   2013-10-25 
* ˵��                :   ����ָ���������ַ�(������Ӣ��)�Ĵ�С,����PDU,U2Sģʽ��,����ֻ����70���ַ�,���ǲ�����Ӣ�� 
                        ��ʱӢ��ֻռ��һ���ֽ�,��������ռ��2���ֽ� 
*************************************************************************************************************************/  
static u16 GSM_GetU2SCharOffset(char *pBuff,u16 CharNum)  
{  
    u16 i;  
    u16 cnt = 0;  
      
  
    for(i = 0;i < CharNum;)  
    {  
        if((pBuff[i] & 0xff) >= 0x80) //����  
        {  
            cnt +=2;  
            i +=2;  
        }  
        else if(pBuff[i] == 0)  //�ַ�������  
        {  
            break;  
        }  
        else                    //ASCII  
        {  
            cnt += 1;  
            i ++;  
        }  
    }  
    return cnt;  
}  

/************************************************************************************************************************* 
* ����                :   bool GSM_CheckNotASCII(char *pBuff,u16 Len) 
* ����                :   ����ַ������Ƿ��з�ASCII���� 
* ����                :   pBuff:�ַ���������;Len:���� 
* ����                :   FALSE:�ַ���ȫ��ΪASCII����;TRUE:�ַ������з�ASCII����,һ��Ϊ���ֱ��� 
* ����                :   �� 
* ����                :   cp1300@139.com 
* ʱ��                :   2013-10-25 
* ����޸�ʱ��    :   2013-10-25 
* ˵��                :   ����ѡ���Ͷ��ŵ�ģʽ,ѡ��U2S����7BIT���� 
*************************************************************************************************************************/  
bool GSM_CheckNotASCII(char *pBuff,u16 Len)  
{  
    u16 i;  
      
    for(i = 0;i < Len;i ++)  
    {  
        if((pBuff[i] & 0xff) >= 0x80)  
            return TRUE;  
    }  
    return FALSE;  
}  

//PDUģʽ�������Ƴ���,���70���ַ�,������Ӣ��  
//����:����֮����ַ�����  
u32 GSM_PDUStrRes(char *pStr)  
{  
    u32 n = 0;  
  
    while(*pStr != 0)  
    {  
        n ++;  
        if(n == 71)  
        {  
            //SIM900_debug("PDUģʽ���ų��ȳ���70B,ǿ��Ϊ70B!\r\n");  
            *pStr = 0;      //ǿ����ӽ�����  
            return n;  
        }  
        if((u8)*pStr < 0x80) //ASCII  
        {  
            pStr ++;  
        }  
        else if((u8)*pStr > 0x80)    //����  
        {             
            pStr += 2;  
        }  
    }       
    return n;  
}  

/************************************************************************************************************************* 
*����         :   void GSM_HexToString(u32 HexNum,c har *pStr, u8 NumDigits) 
*����         :   ����������ת��Ϊ16������ʽ�ַ���(��ĸΪ��д,����������) 
*����         :   HexNum:16�������� 
                    pStr:�ַ�������ָ�� 
*                   NumDigits:����λ��,16��������λ�� 
*����         :   �� 
*����         :   �� 
*����         :   cp1300@139.com 
*ʱ��         :   2013-04-30 
*����޸�ʱ�� :   2013-04-30 
*˵��         :   �����ַ���0xA865ת����Ϊ"A865",λ��Ϊ4λ 
*************************************************************************************************************************/  
static void GSM_HexToString(u32 HexNum,char *pStr, u8 NumDigits)  
{  
    u8 temp;  
    u8 i;  
      
    NumDigits = (NumDigits > 8) ? 8 : NumDigits; //���֧��8λ16������  
      
    for(i = 0;i < NumDigits;i++)  
    {  
        temp = 0x0f & (HexNum >> (4 * (NumDigits - 1 - i)));  
        temp = (temp > 0x09) ? (temp - 0x0A + 'A') : (temp + '0');  
        pStr[i] = temp;  
    }  
}  

/************************************************************************************************************************* 
* ����    :   u16 OneGBKtoUNICODE(u16 GBKCode) 
* ����    :   ��GBK����ת��Ϊunicode���� 
* ����    :   GBK  
* ����    :   unicode 
* ����    :   �ײ��д���� 
* ����    :   cp1300@139.com 
* ʱ��    :   20120602 
* ����޸�ʱ�� : 20120602 
* ˵��    :   ��Ҫflash�е����֧�� 
            GBK�뷶Χ,��8λ:0x81~0xfe;��8λ:0x40~0xfe 
*************************************************************************************************************************/   
u16 OneGBKtoUNICODE(u16 GBKCode)  // ����Ҫ�����
{  
    u8 ch,cl;  
    //UINT bw;  
    u16 data;  
  
    ch = GBKCode >> 8;  
    cl = GBKCode & 0x00ff;  
  
    ch -= 0x81;  
    cl -= 0x40;  
      
    //f_lseek(&GtoU_File, (ch*0xbf+cl)*2);                        				//�ļ�ָ�����ƫ��λ��  
    fseek(GtoU_File, (ch*0xbf+cl)*2, SEEK_SET);
    //if(f_read(&GtoU_File, (u8 *)&data, 2, &bw) != FR_OK)        //��ȡ2�ֽ�  
   	if (fread((u8 *)&data, 1, 2, GtoU_File) == 0)
    {  
       return 0x1fff;  
    }  
      
    return (ch<=0x7d && cl<=0xbe) ? data : 0x1fff;  
    
   /*   
   ch = GBKCode >> 8; 
    cl = GBKCode & 0x00ff; 
 
    ch -= 0x81; 
    cl -= 0x40;  
    return (ch<=0x7d && cl<=0xbe) ? wUnicodes[ch*0xbf+cl] : 0x1fff;      
    */  
  
}  

/************************************************************************************************************************* 
*����         :   static u32 StringToUnicodeStr(char *pStr, char *pucode,u32 SrtLen) 
*����         :   ���ַ���ת��Ϊunicode,���洢Ϊ16������ʽ���ַ��� 
*����         :   pStr:�ַ�������ָ�� 
*                   pucode:ת����������� 
*                   SrtLen:�ַ����ֽڳ��� 
*����         :   ת����Ϊ�ַ���ĳ��� 
*����         :   u16 OneGBKtoUNICODE(u16 GBKCode) 
*����         :   cp1300@139.com 
*ʱ��         :   2013-04-04 
*����޸�ʱ�� :   2013-10-17 
*˵��         :   ���ڽ���������ת��ΪPUD��ʽ,������������ַ��������� 
*                   ��"a,b,c"--->"0,0,6,1,0,0,6,2,0,0,6,3" 
*                   �������������Ϊ�����4�� 
*************************************************************************************************************************/  
static u32 StringToUnicodeStr(char *pStr, char *pucode,u32 SrtLen)  
{  
    u32 i;  
    u16 temp;  
    u8 m;  
    u8 chTmp= 0;  
    u32 cnt = 0;  

    for(i = 0;i < SrtLen;i ++)  
    {      	
		if( (pStr[i]&0xff) < 0x80)   //ASCII  
		{  
			temp = pStr[i];  
		}  
		else                //GBK  
		{  
			temp = pStr[i++];  
			temp <<= 8;  
			temp += pStr[i]&0xff;   
			temp = OneGBKtoUNICODE(temp);  
		}  

		for(m = 0; m <= 12; m+=4) // m <=8; m+=8  
		{  
			chTmp = (temp >> (12-m)) & 0x0F;            //��ȡ��λ   ����ط��ǲ���������  8-m & 0xff
			if(chTmp > 0x09)   //! 0x0A-0x0F  
			pucode[cnt ++] = chTmp-0x0A+'A';       //! 'A'-'F'  
			else                                 //! 0x00-0x09  
			pucode[cnt ++] = chTmp-0x00+'0';       //! '0'-'9'  
		}  
    }  
    return cnt;  // �ַ�������2����
}

/************************************************************************************************************************* 
*����         :   static u8 PhoneNumtoPDUChar(u8 *pNum, char *pCHAR,u8 NumLen) 
*����         :   ���绰�����ַ�ת��ΪPDUҪ����ַ� 
*����         :   pNum:�绰����ָ�� 
*                   pChar:�ַ�������ָ�� 
*                   NumLen:�绰���볤�� 
*����         :   �ַ����� 
*����         :   �ײ�궨�� 
*����         :   cp1300@139.com 
*ʱ��         :   2013-04-04 
*����޸�ʱ�� :   2013-10-17 
*˵��         :   ��Ҫ���ڵ绰����,�������ĺ���ת�� 
*                   �������Ϊ����,�򽫲���ż��,�������һ���ַ��ĸ�λ����λ0xf; 
*                   ����������ӽ����� 
*************************************************************************************************************************/  
static u8 PhoneNumtoPDUChar(u8 *pNum, char *pChar,u8 NumLen)  
{  
    u8 i;  
    u8 temp;  
  
    for(i = 0;i < NumLen;i ++)  
    {  
        temp = (pNum[i]+'0') & 0x0f;  
        if(i % 2)   //λ��Ϊ����  
            pChar[i-1] = (temp > 9) ? ('a' + temp - 10) :( temp + '0');  
        else        //λ��Ϊż��  
            pChar[i+1] = (temp > 9) ? ('a' + temp - 10) : (temp + '0');    
    }  
    if(i % 2)  
    {  
        pChar[NumLen-1] = 'F';  
        return (NumLen + 1);  
    }  
      
    return NumLen;  
}

//����һ������  
//���ŵ绰���볤�Ȱ����ֽ��㵫�Ƕ������ĺ��밴�հ��ֽ���  
//0891683108707505F011000D91683110064530F50008AA0C9E3F9E4475355B5079D16280  // �������ӿƼ�
/************************************************************************************************************************* 
*����         :   bool GSM_SendOneSMS(char *pSMS, u8 *pPDU, char *pServeNumber, char *pPhoneNumber) 
*����         :   ����һ����ͨ����,�������� 
*����         :   pSMS:�������ݻ�����ָ��,����Ϊ�ı��ĵ�,�����ַ�����Ҫ������ 
*                   pPDU:PDU���ݻ�����ָ�� 
                    pServeNumber:�������ĺ��� 
                    pPhoneNumber:Ŀ���ֻ�����ṹָ�� 
*����         :   TRUE:���ŷ��ͳɹ�;FALSE:���ŷ���ʧ�� 
*����         :   �ײ� 
*����         :   cp1300@139.com 
*ʱ��         :   2013-04-04 
*����޸�ʱ�� :   201310-23 
*˵��         :   �����ı���Ҫ��ӽ����� 
                    �绰���������86�ȹ������ſ�ͷ 
                    PDU���Է�������,����textֻ�ܷ���Ӣ�� 
*************************************************************************************************************************/
bool GSM_SendOneSMS(char *pSMS, u8 *pPDU, char *pPhoneNumber)  // �������ĺ���һ��Ҫ��86
{  
    //SIM900_ERROR error;  
    u16 OffsetCnt = 0;      //������ƫ�Ƽ�����  
    u32 cnt;  
    u16 temp;  
    char ComBuff[16];  
    u16  nSMSCenterLen= 0, nSMSPduLen = 0;  
    u16 SMSLen = 0;         //�����ַ�����  
    u8 EnableU2S = DISABLE;    //ʹ��U2S����ģʽ,Ĭ��Ϊ7BIT����ģʽ  
    u8 *p = (u8 *)pSMS;  
      
    while(*p != 0)  
    {  
        if(*p >= 0x80)       //�к���  
        {  
            EnableU2S = ENABLE; //ʹ��U2S����ģʽ  
            //SIM900_debug("��Ҫ���͵Ķ���ΪPDU��ʽ\r\n");  
            break;  
        }  
        p++;  
    }  
      
                  
    if(EnableU2S == ENABLE)         //ʹ����U2S����ģʽ  
    {  
        SMSLen = GSM_PDUStrRes(pSMS);   //����PDU���ų���,������ų���  
    }  
    else                            //TEXTģʽ����  
    {  
        SMSLen = strlen(pSMS);      //������ų���  
        if(SMSLen > 160)     //���ų��ȴ���160���ַ�  
        {  
            pSMS[160] = 0;          //��ӽ�����,���Ƴ���  
            SMSLen = 160;  
        }  
    }  
#if 0      //sim7100 ���Ͷ��Ų���Ҫ��Ӷ������ĺ���
    //����������ĺ��볤��,+91,+86,�������ĺ��������86��ͷ,����Ҫ����91(+),����Ϊÿ�����ְ��ֽ�,���㲹F  
    temp = (strlen(pServeNumber) + 2 + 1) /2;    
    GSM_HexToString(temp, (char *)(pPDU+OffsetCnt), 2); //�������ĺ��볤��ת��Ϊ16������ʽ�ַ���  
    OffsetCnt += 2;                                     //�����������ĺ��볤���ֽ�  
      
    pPDU[OffsetCnt++] = '9';    //������������  
    pPDU[OffsetCnt++] = '1';  
    OffsetCnt += PhoneNumtoPDUChar((u8 *)pServeNumber,(char *)(pPDU+OffsetCnt),strlen(pServeNumber));   //�������ĺ���  
    nSMSCenterLen = OffsetCnt / 2;  
#endif
	// SCA �����̶�Ϊ0
	pPDU[OffsetCnt++] = '0';  
    pPDU[OffsetCnt++] = '0';
    //! PDU  
    pPDU[OffsetCnt++] = '1';  
    pPDU[OffsetCnt++] = '1';  
    //! For MR  
    pPDU[OffsetCnt++] = '0';  
    pPDU[OffsetCnt++] = '0';  
    //! For DA  
      
    //����绰���볤��,+86,���Ͷ��ŵĵ绰������86��ͷ,�绰���볤��Ϊ�ַ�����  
    GSM_HexToString(strlen(pPhoneNumber), (char *)(pPDU+OffsetCnt), 2); //�ֻ����볤��ת��Ϊ16������ʽ�ַ���  
    OffsetCnt += 2; //�����ֻ����볤���ֽ�  
      
    pPDU[OffsetCnt++] = '9';    //������������  
    pPDU[OffsetCnt++] = '1';  
    OffsetCnt += PhoneNumtoPDUChar((u8 *)pPhoneNumber,(char *)(pPDU+OffsetCnt),strlen(pPhoneNumber));   //���ŷ��ͺ���  
    //! For PID  
    pPDU[OffsetCnt++] = '0';  
    pPDU[OffsetCnt++] = '0';  
    //! For DCS  
    if(EnableU2S == ENABLE)         //U2S  
    {  
        pPDU[OffsetCnt++] = '0';  
        pPDU[OffsetCnt++] = '8';  
    }  
    else                            //7BIT  
    {  
        pPDU[OffsetCnt++] = '0';  
        pPDU[OffsetCnt++] = '0';  
    }  
      
    //! For VP  
    pPDU[OffsetCnt++] = 'A';  
    pPDU[OffsetCnt++] = 'A';  
      
    //! For UDL AND UD  
      //! ע��,�˴��Ƚ��û����ݳ�������Ϊ00,��  
      //! ��¼��ʱ�Ļ�����λ��,Ȼ��ȱ������,   
      //! ȷ�����û����ݳ��Ⱥ����޸�Ϊʵ�ʳ���  
      cnt = OffsetCnt;  
      pPDU[OffsetCnt++] = '0';  
      pPDU[OffsetCnt++] = '0';    
      
    //��������  
    if(EnableU2S == ENABLE)         //U2S  
    {  
        temp = StringToUnicodeStr(pSMS,(char *)&pPDU[OffsetCnt], strlen(pSMS));//����������ת��Ϊ�ַ�����  
        OffsetCnt += temp;  
        GSM_HexToString(temp/2, (char *)&pPDU[cnt], 2);  // ���PDU ���ų���
        //! PDU���ĳ���,����AT+CMGSҪ�õ��˳���  
        nSMSPduLen = OffsetCnt / 2 -nSMSCenterLen;  
    }  
    else                            //7bit  
    {  
        u8 buff[140];               //TEXT���Ż�����  
          
        temp = gsmEncode7bit(pSMS, buff);                       //��ASCIIת��Ϊ7bit����  
        GSM_HexToString(SMSLen, (char *)&pPDU[cnt], 2);  
          
        for(cnt = 0;cnt < temp;cnt ++)  
        {  
            GSM_HexToString(buff[cnt], (char *)&pPDU[OffsetCnt+cnt*2], 2);  //7bit����ת��Ϊ16���Ƹ�ʽ�ַ���  
        }  
        OffsetCnt += (temp << 1);  
        //! PDU���ĳ���,����AT+CMGSҪ�õ��˳���  
        nSMSPduLen = OffsetCnt / 2 -nSMSCenterLen;  
    }  
      
    //�������ݳ���ת��Ϊ16������ʽ�ַ���,�洢���ų���  
      
    //end  
    pPDU[OffsetCnt++] = 0x1A;  // CTRL+Z
    pPDU[OffsetCnt++] = 0x00;  // ������
   
    return TRUE;  
}
/************************************************************************************************************************* 
*����         :   bool GSM_SendSMS(char *pSMS, char *pPhoneNumber) 
*����         :   ����һ������ 
*����         :   pSMS:�������ݻ�����ָ��,����Ϊ�ı��ĵ�,�����ַ�����Ҫ������ 
                    pPhoneNumber:Ŀ��绰���� 
*����         :   TRUE:���ŷ��ͳɹ�;FALSE:���ŷ���ʧ�� 
*����         :   �ײ� 
*����         :   cp1300@139.com 
*ʱ��         :   2013-10-25 
*����޸�ʱ�� :   2013-10-25 
*˵��         :   ��Ҫ�ȵ���SIM900_SetSMSServeNumber()���ö������ĺ��� 
                    ��Ҫʹ��ȫ�ֵ�PDU���ݻ����� 
                    һ��Ҫ��ӽ����� 
                    �����ų��ȳ����������ų������ƺ�ᷢ�Ͷ������� 
*************************************************************************************************************************/  
#define SMS_MAX_LEN     2048                    //������󳤶�  
bool GSM_SendSMS(char *pSMS, char *pPhoneNumber)  
{  
    char SMSBuff[160+1];    //�������160Byte,����һ��������  
    u8 PDUBuff[512];        //����PDU���ݻ�����  
    u16 SMSLen;             //���ų���  
    u16 SMSOffset;          //���ŷ���ƫ��λ��,���ڷ��Ͷ�������  
    u16 i,j;  
      
    SMSLen = strlen(pSMS);  		//��ȡҪ���͵Ķ��ų���  
    if(SMSLen > SMS_MAX_LEN) 
		SMSLen = SMS_MAX_LEN;   //���ƶ�����󳤶�,��ֹ���޷���  
		
#if 0      
    if(strlen(SMSServeNumber.PhoneNumBuff) == 0)  
    {  
        //SIM900_debug("���ڶ������ĺ�������ʧ��,���¶����޷�����!\r\n");  
        return FALSE;  
    }  
#endif

    SMSOffset = 0;          //��ʼƫ��Ϊ0  
    while(1)  
    {  
        if((SMSLen-SMSOffset) > 160)  
            j = 160;  
        else   
            j = SMSLen-SMSOffset;  
        for(i = 0;i < j;i ++)  
        {  
            SMSBuff[i] = pSMS[SMSOffset + i];   //���ƶ��ŵ����ͻ�����  
        }  
        SMSBuff[j] = 0; //��ӽ�����  
          
        if(GSM_CheckNotASCII(SMSBuff,j) == TRUE)    //�ָ�Ķ����к��з�ASCII����,��ôֻ��ʹ��U2S����,ֻ�ܷ���70���ַ�(������Ӣ��)  
        {  
            SMSOffset += GSM_GetU2SCharOffset(SMSBuff,70);  //��һ����������70���ַ�,������һ���ָ����ʼλ��  
            SMSBuff[SMSOffset] = 0;  
        }  
        else  
        {  
            SMSOffset += j; //��һ���ָ����ʼλ��  
            SMSBuff[SMSOffset] = 0;  
        }  
          
        //SIM900_WaitSleep(1000);   //�ȴ���һ���������  
        if(GSM_SendOneSMS(SMSBuff, PDUBuff, pPhoneNumber) == TRUE)  
        {  
			send_one_message(PDUBuff);
        }  
        else  
        {  
            //SIM900_debug("���Ͷ���ʧ��!\r\n");  
            return FALSE;  
        }  
        if(SMSOffset >= SMSLen) 
			break;   //���ŷ������,�˳�  
    }  
      
    return TRUE;  
      
}


/************************************************************************************************************************* 
* ����    :   u8 GBK_UNICODE_Init(void) 
* ����    :   ��ʼ��GBK,UNICODE����� 
* ����    :   ��    
* ����    :   0:��ʼ���ɹ�;����:��ʼ��ʧ�� 
* ����    :   �ײ��д���� 
* ����    :   cp1300@139.com 
* ʱ��    :   2013-04-18 
* ����޸�ʱ�� : 2013-04-18 
* ˵��    :   �� 
*************************************************************************************************************************/   
u8 GBK_UNICODE_Init(void)  
{    
    GtoU_File = fopen(GtoU, "r");
	if (GtoU_File == NULL)
	{
		printf("open GtoU failed\n");
		return 0;
	}
	UtoG_File = fopen(UtoG, "r");
	if (UtoG_File == NULL)
	{
		printf("open UtoG failed\n");
  		return 0;
	}
    return 1;  
} 
