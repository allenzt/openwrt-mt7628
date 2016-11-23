#ifndef SIM7100_SMS
#define SIM7100_SMS

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;

#define SMS_NUM_LEN_MAX     16      //�绰������󳤶�16λ  
#define DISABLE 0 
#define ENABLE 1
#define FALSE 0
#define TRUE	1

//���ŷ�������,ʹ���ַ���ʽ  
typedef struct  
{  
    u8  Year;       //��20xx��  
    u8  Month;      //��  
    u8  Day;        //��  
    u8  Hour;       //Сʱ  
    u8  Minute;     //����  
    u8  Second;     //��  
    u8  Reserve1;   //����  
    u8  Reserve2;   //����  
}SMS_TIMER ;  

// ����Ϣ�����ṹ������/���빲�� 
// ���У��ַ�����0��β 
typedef struct { 
    char SCA[16];       // ����Ϣ�������ĺ���(SMSC��ַ) 
    char TPA[16];       // Ŀ������ظ�����(TP-DA��TP-RA) 
    char TP_PID;        // �û���ϢЭ���ʶ(TP-PID)  ����:��Ե�
    char TP_DCS;        // �û���Ϣ���뷽ʽ(TP-DCS) 	00 04 08
    char TP_SCTS[16];   // ����ʱ����ַ���(TP_SCTS), ����ʱ�õ� 
    char TP_UD[161];    // ԭʼ�û���Ϣ(����ǰ�������TP-UD) 
    char index;         // ����Ϣ��ţ��ڶ�ȡʱ�õ� 
    char message_type;  //��Ϣ���ͣ��յġ����� ��
} SM_PARAM; 

typedef struct  
{  
    SMS_TIMER    Timer;                      //���ŷ��͵�ʱ��     
    char  NumBuff[SMS_NUM_LEN_MAX];   //�绰���뻺����,ʹ�õ����ַ�ģʽ                
    u8     NumLen;                     //�绰���볤��  
    u8     SMS_Size;                   //������Ч���ݳ���,���140B  
    u8     TEXT_MODE;                  // 1:����ΪTEXTģʽ;0:����ΪPDUģʽ  
    u8     PDU;                        //PDU����,���������Ƿ��ж���ͷ����Ϣ  
    u8     DSC;                        //DSC����,���������Ƿ�Ϊ�ַ�ģʽ(0),PDUģʽ(0X08)  
    u8     AllNum;                     //��ǰ�����ָܷ���  
    u8     PreNum;                     //��ǰλ��  
    u8     IndexNum;                 //��ǰ����λ��1-50  
    u8	tp_ud[161];
    u8	message_type;  			//��Ϣ���ͣ��յġ����� ��	
}sms_info ;  

#define GtoU    "/etc/config/GtoU.sys"       //GBK ת UCICODE �����λ��  
#define UtoG    "/etc/config/UtoG.sys"       //UCICODE ת GBK �����λ��  

FILE *GtoU_File;            //GtoU �ļ�������  
FILE *UtoG_File;            //UtoG �ļ�������  

//�绰����ṹ  
#define PHONE_NUMBER_MAX_LEN    24-2        //�绰������󳤶�  
typedef struct  
{  
    u8      PhoneNumLen;        //�绰���볤��  
    char    PhoneNumBuff[PHONE_NUMBER_MAX_LEN + 1];//�绰���뻺����,�绰����ǰ���2λΪ�������,�й�Ϊ86,��绰��Ҫ����ǰ���2λ����  
}PHONE_NUMBER;  

static PHONE_NUMBER SMSServeNumber;                                 //ȫ�ֶ������ĺ���  

#endif