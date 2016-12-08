#ifndef	_AT_
#define	_AT_

#ifndef _DBUG_ 
#define _DBUG_
#endif

#include <sys/types.h>
#include <sys/stat.h>
#include <ctype.h>
#include <fcntl.h>
#include <termios.h>    
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>
#include <pthread.h>
#include <string.h>
#include <semaphore.h>
#include <signal.h>
#include "api_demo.h"
#include <setjmp.h>
//#include <stdbool.h>
//#include "si_voice_datatypes.h"

#define GSM_RX_MAXBUFF       4096
#define MAX_GSM         4
#define FALSE -1
#define TRUE 1
#define GSM_RX_OVERTIM       1000    //50*10=500ms
#define GSM_CARR_OVERTIM  600  // 6S
#define GSM_HANG_OVERTIM  4000 // 10s
#define SIM_ALSA_STATUS_TIME		4000
#define NET_STATUS_TIME		6000
#define SIM_CSQ_TIME				3000
#define SIM_NETWORK_TIME   5000
#define uchar unsigned char
#define  DEFAULT_PINCODE          "1234"  
#define  DEFAULT_LOCK_PINCODE     "5678"

#define VOLUME_SYS_FILE	 		"/etc/config/gainset"
#define DIGITAL_MAP					"/etc/config/dialset"
#define MISCELLANEOUS_FILE 	"/etc/config/miscellaneous"
#define TONE_DC_RING_FILE	 	"/etc/config/regional"
#define SMS_CONFIG_FILE			"/etc/config/smscnf"
#define DIAL_LIST					 		"/usr/log/dialerlist"
#define RECV_LIST						"/usr/log/recvlist"
#define MISS_LIST						"/usr/log/misslist" 
#define DBUG_LOG						"/usr/log/test.log"
#define LOCKCARD_FILE				"/usr/log/lockcard"
#define SHORT_SMS_FILE			"/usr/log/smsfile"
#define AT_ORDER_FILE				"/etc/config/atorderfile"
#define NETWORK_FILE				"/etc/config/network"
#define CALL_COUNT	10
#define START_TIME		1
#define END_TIME			0
#define WATCHDOG_ENABLE 1
#define WATCHDOG_LOW 2
#define WATCHDOG_HIGET 3
#define AT_TIMES	3
#define SMS_IDLE 0
#define SMS_SEND 1
#define SMS_DELETE 2

#define DTMF_DECODE
//#undef DTMF_DECODE
#define SMS_ENABLE
#undef SMS_ENABLE
#define SIM7600TP
#undef SIM7600TP

#define GSM_CFG
#undef GSM_CFG

typedef enum
{
	AT_INIT = 0,
	AT,	
	ATI,
	AT_CSQ,
	AT_STTONE,
	AT_CNMI,
	ATH,
	ATA,
	AT_VTS,
	AT_CLVL,
	AT_CFUN,
	AT_ECHO,
	AT_CMIC,
	AT_CREG,
	AT_CCLK,
	AT_CLIP,
	AT_CMGF,
	AT_CMGS,
	AT_CSCA,
	AT_CSCA_Q,
	AT_CMGD,
	AT_CMGR,
	AT_CMGL,
	AT_CBAND,
	AT_CPMS,
	AT_CHFA,
	ATE0,												// 25
	AT_COLP,
	AT_CSMINS,
	AT_CIPSTART,
	AT_CIPHEAD,
	AT_CIPSEND,
	AT_CIPSHUT,
	AT_CIPCLOSE,
	AT_CSTT,
	AT_CGSN,
	AT_CLCK,
	AT_CPWD,
	AT_CLCK_R,
	AT_CPIN_R,
	AT_CPIN,
	AT_CIMI,
	ATD,
	AT_W,
	AT_SMS,
	AT_CPAMP,
	ATS0,
	ATE1,
	AT_CPCMREG0,
	AT_CPCMREG,
	AT_CPCMREGOVER,
	AT_CVHU,
	AT_CHUP,
	AT_CODECSEL,
	AT_CLCC,
	AT_CNMP,
	AT_MONI,
	AT_QCRMCALL,
	AT_QCRMCALL1,
	AT_QCRMCALL0,	
	AT_CSUB,
	AT_CSQFMT,
	AT_COPS,
	AT_CMGW,
	AT_CMSS,
	AT_CNSMOD,
	AT_CGMM,
}AT_ORDER_ID;

typedef struct
{
	AT_ORDER_ID	str_id;
	char        *order;
}AT_ORDER;

//sim��״̬
typedef enum GSM_MAINSTATE_TAG
{
	HS_NONEED_SIMCARD_QUERY = 0,
	HS_NONEED_SIMCARD_QUERY_WAIT,
	HS_LOCKCARD_DETECT,
	HS_LOCKCARD_DETECT_WAIT,  
	HS_SEM_WAIT,
	HS_AT_INIT,
	HS_AT_INIT_WAIT,
	HS_AT_INIT_ERROR,
	HS_NORMAL,
	HS_NETWORK_SEEK,
	HS_SIMCOM_ALSA_SEEK,
	HS_SIMCOM_CSQ_SEEK,
	HS_NETWORK_MODE_SEEK,
	HS_DIAG_UP,
	HS_DIAG_UP_WAIT,
	HS_DIAG_DOWN,
	HS_DIAG_DOWN_WAIT,
	HS_SIMCOM_SMS_UPDATE,
	HS_SIMCOM_SMS_CMGW_WAIT,
	HS_SIMCOM_SMS_CMGD_WAIT,
	HS_SIMCOM_SMS_SEND_WAIT,
	HS_SIMCOM_SMS_SEND,
	HS_NOSIM,
	HS_VOLUME,
	HS_VOLUME_WAIT,
	HS_SIM_INSERTED,
	HS_CHECK_SIMCARD_LOCK,
	HS_NO_SERVICE,
	HS_READY_SEARCH,
	HS_SEARCHING,
	HS_CHANGESIM,
	HS_SEEKING_EMER,
	HS_EMER_LOCKED,
	HS_NO_SERVICE_LOCKED,
	HS_SEARCHING_LOCKED,
	HS_AUTO_LOCKCARD,        //����
	HS_AUTO_LOCKCARD_ENABLE, //����ʹ��
	HS_AUTO_LOCKCARD_DISABLE,
	HS_AUTO_LOCKCARD_MODIFY, //�޸Ŀ�PINCODE
	HS_AUTO_LOCKCARD_ERROR,  //��������
	HS_AUTO_LOCKTERMINAL,       //����
	HS_AUTO_LOCKTERMINAL_ERROR, //����������ʾ
	HS_AUTO_LOCKCARD_WAITING, //�����ȴ�
	HS_AUTO_LOCKTERMINAL_WAITING,//�����ȴ�
}GSM_MAINSTATE;

typedef enum GSM_SUBSTATE_TAG
{
	GSM_SUBSTATE_IDLE = 0,
	GSM_SUBSTATE_DIALING,	// ����
	GSM_SUBSTATE_CALLING_CONNECT,
	GSM_SUBSTATE_CALLING_DISCONNECT,
	GSM_SUBSTATE_CALLING,	// 
	GSM_SUBSTATE_RING, // ��������
	GSM_SUBSTATE_HANG_UP, //�Է��һ�
	GSM_SUBSTATE_RING_ANSWER, // �����Ӧ
	GSM_SUBSTATE_SEND_SMS,// ���Ͷ���
	GSM_SUBSTATE_ANSWERING,// ����ͨ��
}GSM_SUBSTATE;

typedef struct
{
	int   	tx_overtime;                //��ʱ
	int	net_status_time;
	int 	sim_status_time;
	int	sim_csq_time;
	int	sim_network_time;
	int	carr_overtime;
	int	hang_overtime;
	uchar          init_step;                  	//��ʼ������
	GSM_MAINSTATE  main;                  //GSM��״̬
	GSM_SUBSTATE   sub;						//GSM��״̬
}ST_GSM_STATE;

typedef enum 
{
	AT_IDLE = 0,        			//����
	AT_WAIT_SEND,       		//�ȴ�����
	AT_SENDING,         			//���ڷ���
	AT_WAIT_ANSWER,     	//�ȴ�Ӧ��
	AT_ANSWER_OK,       		//Ӧ����ȷ
	AT_ATD_ANSWER_OK,		// ����Ӧ������
	AT_ATD_CONNECT_OK,	//���ŶԷ��ѽ���
	AT_ATD_ANSWER_ERROR,	//����ʧ��
	AT_ERROR,
	AT_ANSWER_LOCKSIM,  //����
	AT_ANSWER_UNLOCKSIM,//����Ҫ����
	AT_ANSWER_PUK,	    	//PUK
	AT_ANSWER_IMSI,			//IMSI
	AT_ANSWER_ERROR,    	//����Ӧ��
	AT_NOCAR,           			//����
	AT_CONNECT_OK,			//��·��ͨӦ��
}AT_STATE;


typedef struct
{
	AT_STATE       state;
	AT_ORDER_ID	   order;
	char           tx_data[30];
}ST_AT_ORDER;

typedef struct
{
	int          rx_length;                  //AT���յĳ���
	char       rx_data[GSM_RX_MAXBUFF];    //AT���յĻ���
}ST_AT_RX;

typedef struct
{
	uchar	com_state;
	uchar	callin_nu[14];     //��������
	uchar	imei[16];
	uchar	cellid[10][10];		
	uchar 	cell_num;
	uchar	at_times;
	int	sms_num;
	ST_AT_ORDER    at;                        //AT����״̬�뻺�� 
	ST_AT_RX       rx;				// ���ջ�����
}ST_GSM_AT;

typedef struct
{
	uchar  terminal;                          //������־
	uchar  card;                              //������־
	uchar 	pinmodify;
	char   imsi[15];                          //imsi��,����ʱʹ��
	char   checksum;                      //У���
}ST_LOCK_SIM;

struct calltime
{
	int tm_sec;  //����Ŀǰ����, ������ΧΪ0-59, ��������61 ��
    int tm_min;  //����Ŀǰ����, ��Χ0-59
    int tm_hour;  //����ҹ�����ʱ��, ��ΧΪ0-23
    int tm_mday;  //Ŀǰ�·ݵ�����, ��Χ01-31
    int tm_mon;  //����Ŀǰ�·�, ��һ������, ��Χ��0-11
    int tm_year;  //��1900 ���������������
    int tm_wday;  //һ���ڵ�����, ������һ����, ��ΧΪ0-6
    int tm_yday;  //�ӽ���1 ��1 ���������������, ��ΧΪ0-365
    int tm_isdst;  //�չ��Լʱ������
};

typedef struct
{
	uchar	num_list;
	uchar 	call_number[14];
	struct calltime start_time;
	struct calltime end_time;
	int 		talk_time;
}call_log;

call_log  dial_list,recv_call_list,miss_call_list;

int fd, fd3, fd4, fd5;
int gpio_fd;
volatile int  fd2;
FILE *fp,*atorder_file;
//uchar audiobuf[320];
char *audiobuf;
char rcv_buf[1024];
char pcm_flag;
char answer_begin;
char slic_pcm_read_open;
char *pattern;
char patternx[1024];

int sms_update;
int sms_operation;
int map_enable;			// ��ͼʹ��
int unmatch_undial;	// ��ƥ�䲻����
int net_enable;			// ������������
int net_flag;
int dialing_key;			// �첦��
int call_antipole; 		// ͨ������
int dial_up_timeout; 	// ���ų�ʱ
int call_id_enable;		// ������ʾ����
int call_id_mode;		// ������ʾģʽ
int jitter_delay;			// ��С������ʱ
int max_jitter_delay;	// ��󶶶���ʱ

int answer_call;
int miss_call;
int watchdog_state;
jmp_buf env;
int nums;

//pthread_mutex_t gsm_recv_mutex=PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t audio_mutex;
pthread_mutex_t cid_mutex;
pthread_mutex_t dtmf_mutex;
pthread_mutex_t atorder_mutex;
pthread_mutex_t send_atorder_mutex;
pthread_mutex_t pcm_mutex;
pthread_mutex_t pcm_recv_mutex;
pthread_mutex_t pcm_send_mutex;
sem_t	gsm_sem;
sem_t	gsm_pcm_recv, gsm_pcm_send, pcm_record, gsm_pcm_sem, gsm_pcm_sem2;

void at_fixed_format(AT_ORDER_ID order);
void gsm_init_and_sim_active();
void send_at_order();
void gsm_rx_query_handler();
void  set_gsm_sub_state(GSM_SUBSTATE state );
GSM_SUBSTATE get_gsm_sub_state();
GSM_MAINSTATE get_gsm_mainstate();
void gsm_pcm_answer(AT_ORDER_ID order);
void set_gsm_at_state( AT_STATE state );
void clear_at_rx_buff();
void at_swicth();
void *gsm_send_handler(void *arg);
void *gsm_init_handler(void * arg);
void *gsm_recv_handler(void *arg);
void *gsm_audio_recv(void *arg);
void dialing(char *number);
void time_handler(int k);

ST_GSM_STATE  st_gsm_state;
ST_GSM_AT     	st_gsm_at;
ST_LOCK_SIM		st_lock_sim;

#endif
