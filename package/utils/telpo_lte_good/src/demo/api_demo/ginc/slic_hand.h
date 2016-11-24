#ifndef _SLIC_HAND_
#define _SLIC_HAND_

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>    
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>
#include <pthread.h>
#include <string.h>

#define ulong unsigned long
#define uchar unsigned char
#define  uint     unsigned short

#define DMA_CFG
#undef DMA_CFG

#define OFFHOOK_MAX_COUNT 	5
#define ONHOOK_MAX_COUNT    10         //�һ�������
#define OPEN_RING_TIME      	100        //100*10ms=1S   
#define RING_TIME           		400        //400*10ms=4S   
#define MAX_AUDIO_SIZES    	640		// ÿһ��8000���ֽڣ�
#define gsm_size 320

typedef enum zref
{
	IM_KORA = 0,
	IM_STANDARD,
	IM_JAPAN,
	IM_BELLCORE,
	IM_CTR,
	IM_CHINA_CO,
	IM_CHINA_PBX,
	IM_JAPAN_PBX,
	IM_INDIA_NEW,
	IM_GERMANY,
	IM_UK,
	IM_AUSTRALIA,
	IM_VARIATION,
}impedance_order;

typedef struct 
{
	impedance_order im_id;
	char *impedance;
}impedance_match;

typedef enum SLIC_HOOKSTATE_TAG
{
	SLIC_ON_HOOK = 0,   	//�һ�	  
	SLIC_OFF_HOOK,      	//ժ��    
}SLIC_HOOKSTATE;

typedef struct
{
	SLIC_HOOKSTATE hook;           		// ժ��Ϊ1,�һ�Ϊ0
	int 			   onhook_time;				// �һ���Чʱ��
	int 			   offhook_time;				// ժ����Чʱ��
}ST_SLIC_STATE;

ST_SLIC_STATE  st_slic_state;

void *slic_hand(void *arg);
void set_slic_state(char i);
char get_slic_state(void);
void slic_init();
void *proslic_audio(void *arg);
void *proslic_audio_write(void *arg);

#endif
