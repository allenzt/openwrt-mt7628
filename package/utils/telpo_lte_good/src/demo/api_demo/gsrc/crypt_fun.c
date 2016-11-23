/*---------------------------------------------------------------------------*/

#include "crypt_fun.h"
#include "des.h"

#define DES_LEN      8

/*����ʱ����Դ����8λ���油������ֵ,��������0x00��Ϊ���*/
#define DES_PADDING  0x00



/*******************************************************************************
������int crypt_des(unsigned char *Key, unsigned char *InBuff,
               int InLen, unsigned char *OutBuff, char IsEncrypt)

���ܣ��Ե���Ϣ����Des�Ľ���,
����:
       unsigned char *Key      DES����Կ�����ȹ̶���8λ
       unsigned char *InBuff   ��Ҫ���ܵ����뻺����
       int InLen               ���뻺�����ĳ���
       unsigned char *OutBuff  ���ܺ�����������
       char IsEncrypt         �Ǽ��ܻ��ǽ���, 0 ����; 1 ���� 

���أ� ����������ĳ���

���: Zhou Guo-rui
ʱ��: 20100604
���أ�
*******************************************************************************/
int crypt_des(unsigned char *Key, unsigned char *InBuff,
             int InLen, unsigned char *OutBuff, char IsEncrypt)
{
    unsigned char tmp[DES_LEN];
    int n;
    int flag;
    des_context ctx;
    unsigned char *pInBuff;
    unsigned char *pOutBuff;

    flag = 0;
    n = 0;
    memset(&ctx,0,sizeof(des_context));
    pInBuff = InBuff;
    pOutBuff = OutBuff;

    ctx.mode = IsEncrypt;

    if (IsEncrypt == DES_ENCRYPT)
    {
        des_setkey_enc(&ctx,Key);
    }
    else
    {
        des_setkey_dec(&ctx,Key);
    }

    while (!flag)
    {
        if ((InLen - n) > DES_LEN)
        {
            des_crypt_ecb(&ctx,pInBuff,pOutBuff);
            pInBuff += DES_LEN;
            pOutBuff += DES_LEN;
        }
        else
        {
            flag = 1;
            memset(tmp,DES_PADDING,DES_LEN);
            memcpy(tmp,pInBuff, InLen - n);
            des_crypt_ecb(&ctx,tmp,pOutBuff);
        }

        n += DES_LEN;
    }

    return n;
}
