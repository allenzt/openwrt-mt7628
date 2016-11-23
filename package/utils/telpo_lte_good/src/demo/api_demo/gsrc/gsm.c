#include "gsm.h"
#include "dbug_log.h"
#include "sim7100_sms.h"

//GSM��ʼ�������б�
AT_ORDER_ID gsm_init_list[] =
{
	ATS0,  // ����Ҫ�忨
	AT_CLIP,
	AT_CLCC,
#ifdef SMS_ENABLE
	AT_CMGF,	
	AT_CPMS,
	AT_CNMI,
	AT_CMGL,	
#endif
	//AT_CSQFMT,  	// ���Բ�Ҫ��
	//AT_CGSN,			// ���Բ�Ҫ��
	AT_CIMI,			// ����Ҫ��  д�뵽�ļ�
	AT_COPS,			// ����Ҫ��  д�뵽�ļ�
	//AT_CNSMOD,
	//AT_CSUB,			// ���Բ�Ҫ��
	//AT_CSQ,			// ���Բ�Ҫ��
	AT_CVHU,
};

AT_ORDER_ID gsm_noneed_simcard_query[] = 
{
	ATE1,
	AT_CGMM,
#ifndef SIM7600TP
	AT_CODECSEL,
#endif
	AT_CSQFMT,  	// ���Բ�Ҫ��
	AT_CGSN,			// ���Բ�Ҫ��   д�뵽�ļ�
	AT_CSUB,			// ���Բ�Ҫ��   д�뵽�ļ�
	AT_CSQ,			// ���Բ�Ҫ��		д�뵽�ļ�
	//AT_CPIN_R,		// ���Բ�Ҫ��   д�뵽�ļ�
};

const AT_ORDER at_order_table[] =
{	
	AT_INIT,             "",
	AT,	                 "AT\r",                   //AT\CR\CR\LFOK\CR\LF   
	ATI,							"ATI\r",
	AT_CSQ,              "AT+CSQ\r",               //�ź�ǿ��: ��Χ0~32  AT+CSQ\CR\CR\LF+CSQ: 13,0\CR\LF\CR\LFOK\CR\LF
	AT_STTONE,           "AT+STTONE=",   // ����������
	AT_CNMI,             "AT+CNMI=2,1,0,0\r\n",    //�¶��Ų�ѯ
	ATH,                 "ATH\r",                  //�һ�
	ATA,                 "ATA\r",                  //����Ӧ��
	AT_VTS,              "AT+VTS=",                //����DTMF��
	AT_CLVL,             "AT+CLVL=",          //��Ͳ����
	AT_CFUN,             "AT+CFUN=0\r",            //�ػ�
	AT_ECHO,             "AT+ECHO=32767,100,5,0\r",//ECHO����
	AT_CMIC,             "AT+CMIC=0,8\r",          //MIC��������
	AT_CREG,             "AT+CREG?\r",             //��û���ź�
	AT_CCLK,             "AT+CCLK\r",              //ʱ�Ӳ�ѯ
	AT_CLIP,             "AT+CLIP=1\r",            //���к���ʶ��
	AT_CMGF,             "AT+CMGF=0\r",
	AT_CMGS,             "AT+CMGS=\"",             //������
	AT_CSCA,			 	"AT+CSCA=\"",			   //���ö������ĺ�	
	AT_CSCA_Q,           "AT+CSCA?\r",             //��ѯ�������ĺ�
	AT_CMGD,             "AT+CMGD=",               //ɾ����
	AT_CMGR,             "AT+CMGR=",               //������
	//AT_CMGL,             "AT+CMGL=\"ALL\"\r\n",          //�оٶ���TEXT
	AT_CMGL,             "AT+CMGL=4\r\n",          //�оٶ��� PDU
	AT_CBAND,            "AT+CBAND?\r",            //��ѯ��ǰƵ��
	AT_CPMS,             "AT+CPMS=\"SM\",\"SM\",\"SM\"\r\n",               //�Զ��Ų���
	AT_CHFA,             "AT+CHFA=0\r",            //����ͨ���л�,1ΪMIC,0Ϊ�ֱ�
	ATE0,                	"ATE0\r",                 			//ȡ������
	AT_COLP,             "AT+COLP=1\r",            //������ͨ����
	AT_CSMINS,           "AT+CSMINS?\r",           //SIM����װ״̬
	AT_CIPSTART,         "AT+CIPSTART",            //GPRS
	AT_CIPHEAD,          "AT+CIPHEAD",             //GPRS
	AT_CIPSEND,          "AT+CIPSEND",             //GPRS
	AT_CIPSHUT,          "AT+CIPSHUT",             //GPRS
	AT_CIPCLOSE,         "AT+CIPCLOSE",            //Close TCP/UDP
	AT_CSTT,             "AT+CSTT",                //�û�����
	AT_CGSN,             "AT+CGSN\r",              //��ģ�����к�IMEI��
	AT_CLCK,             "AT+CLCK=\"SC\",",        //��������AT+CLCK="SC",1/0,Pincode;  1ʹ�ܣ�0��ʹ�� 
    AT_CPWD,             "AT+CPWD=\"SC\",\"",      //�޸�Pincode
	AT_CLCK_R,			 "AT+CLCK=\"SC\",2\r", 	   //��⿨�Ƿ�ʹ�ܣ�+CLCK: 1/0
	AT_CPIN_R,           "AT+CPIN?\r",             //+CPIN: READY��+CPIN: SIM PUK
	AT_CPIN,			 "AT+CPIN=",			   //sim PIN CODE or PUK
    AT_CIMI,             "AT+CIMI\r",              //IMSI��
	ATD,                 "ATD",                    //��ʽΪATD112;\r,112Ϊ����
	AT_W,                "AT&W\r",				// ��������
	AT_SMS,              "",	                                //"ON CAR"���йһ�  
    AT_CPAMP,			 "AT+CPAMP=0\r",
    ATS0,					"ATS0=000\r",                        // ��ֹ�Զ�����
    ATE1,					"ATE1\r",											// ʹ�ܻ���
    AT_CPCMREG0,		"AT+CPCMREG?\r",					//��ѯsimģ��alsa�ػ�����״̬
    AT_CPCMREG,			"AT+CPCMREG=1\r",						// ʹ�ܽ���pcm����
    AT_CPCMREGOVER,	"AT+CPCMREG=0\r",				// ʹ����֮��Ҫ�ر�pcm
    AT_CVHU,					"AT+CVHU=0\r",
    AT_CHUP,					"AT+CHUP\r",
    AT_CODECSEL,			"AT+CODECSEL=0\r",
    AT_CLCC,					"AT+CLCC=1\r",
    AT_CNMP,					"AT+CNMP=",
    AT_MONI,					"AT+MONI?\r",
    AT_QCRMCALL,			"AT$QCRMCALL?\r",
    AT_QCRMCALL1,		"AT$QCRMCALL=1,1\r",
    AT_QCRMCALL0,		"AT$QCRMCALL=0,1\r",
    AT_CSUB,					"AT+CSUB\r",					//��ѯsimģ��̼��汾
    AT_CSQFMT,				"AT+CSQFMT=1\r",
    AT_COPS,					"AT+COPS?\r",
    AT_CMGW,				"AT+CMGW=",
    AT_CMSS,			    	"AT+CMSS=",
    AT_CNSMOD,			"AT+CNSMOD?\r",
    AT_CGMM,				"AT+CGMM\r",
};

int simcom_enable (SiVoiceChanType_ptr pChan)
{
    /*
    ** assert reset, wait 250ms, release reset, wait 250ms
    */
    //pChan->deviceId->ctrlInterface->Reset_fptr(pChan->deviceId->ctrlInterface->hCtrl,4); // �ߵ�ƽ
    ioctl(gpio_fd, 4);
    return RC_NONE;
}

int simcom_disable (SiVoiceChanType_ptr pChan)
{
    /*
    ** assert reset, wait 250ms, release reset, wait 250ms
    */
    //pChan->deviceId->ctrlInterface->Reset_fptr(pChan->deviceId->ctrlInterface->hCtrl,5); // �͵�ƽ
    ioctl(gpio_fd, 5);
    return RC_NONE;
}

void ttyusb_exist(chanState ports)
{
	char ttybuf[20] = {0};
	int i = 0;
	if (access("/dev/ttyUSB0", F_OK) == -1) // ������
	{
		if (access("/dev/ttyUSB2", F_OK) != -1)
		{
			remove("/dev/ttyUSB2");
		}
	}
	for (i = 0; i < 6; i++)
	{
		sprintf(ttybuf, "/dev/ttyUSB%d", i);
		ttybuf[strlen(ttybuf)] = '\0';
		if (access(ttybuf, F_OK) == -1) //����ļ��Ƿ���ڣ���������ھ͵ȴ�
		{
			perror("");
			simcom_enable(ports.ProObj); //ʹ��4G ģ��
			sleep(25);
			break;
		}
	}
}

void gsm_delay_time()
{
	if (st_gsm_state.tx_overtime > 0)
	{
		st_gsm_state.tx_overtime--;
	}
	if (st_gsm_state.net_status_time > 0)
	{
		st_gsm_state.net_status_time--;
	}
	if (st_gsm_state.sim_status_time > 0)
	{
		st_gsm_state.sim_status_time--;
	}
	if (st_gsm_state.sim_csq_time > 0)
	{
		st_gsm_state.sim_csq_time--;
	}
	if (st_gsm_state.sim_network_time > 0 )
	{
		st_gsm_state.sim_network_time--;
	}
}

void gsm_cmgw()
{
	char *number = "13016054035";
	char buf[100];
	//��ȡĿ�ĺ���
	//AT+CMGW="13016054035" <CR> (TEXT MODE)   PDU:AT+CMGW=����(���ų���)
	if (st_gsm_at.at.state == AT_IDLE)
	{
		memset(st_gsm_at.at.tx_data, 0x00, sizeof(st_gsm_at.at.tx_data));
		st_gsm_at.at.state = AT_WAIT_SEND;
		st_gsm_at.at.order = AT_CMGW;
		//sprintf(buf, "\"%s\" <CR> (TEXT MODE)", number);
		sprintf(buf, "%d", 1); // PDU
		memcpy(st_gsm_at.at.tx_data, buf, strlen(buf));
		memcpy(&st_gsm_at.at.tx_data[strlen(buf)], (char *)"\r", strlen("\r"));
	}
}

void gsm_cmgd()
{
	int num = 26;
	char buf[4];
	//��ȡɾ���Ķ��ű��
	if (st_gsm_at.at.state == AT_IDLE)
	{
		memset(st_gsm_at.at.tx_data, 0x00, sizeof(st_gsm_at.at.tx_data));
		st_gsm_at.at.state = AT_WAIT_SEND;
		st_gsm_at.at.order = AT_CMGD;
		sprintf(buf, "%d", num);
		memcpy(st_gsm_at.at.tx_data, buf, strlen(buf));
		memcpy(&st_gsm_at.at.tx_data[strlen(buf)], (char *)"\r", strlen("\r"));
	}
}

int send_one_message(char *pdu)
{
	int ret = 0;
	
	do{
		ret = uart_send(fd, pdu, strlen(pdu));		
	}while (ret <= 0);
	
	return ret;
}

int fill_short_sms()
{
	int ret = 0;
	//��ȡ���͵�����
	char smsbuf[256] = "hello world";
	smsbuf[strlen(smsbuf)] = 0x1a;
	do{
		ret = uart_send(fd, smsbuf, strlen(smsbuf));		
	}while (ret <= 0);
	
	return 0;
}

int send_sms()
{
	char buf[5];
	if (st_gsm_at.at.state == AT_IDLE)
	{
		memset(st_gsm_at.at.tx_data, 0x00, sizeof(st_gsm_at.at.tx_data));
		st_gsm_at.at.state = AT_WAIT_SEND;
		st_gsm_at.at.order = AT_CMSS;
		sprintf(buf, "%d", st_gsm_at.sms_num);
		memcpy(st_gsm_at.at.tx_data, buf, strlen(buf));
		memcpy(&st_gsm_at.at.tx_data[strlen(buf)], (char *)"\r", strlen("\r"));
	}
}

void gsm_on_hook()
{
#ifdef _DBUG_
	printf("gsm_on_hook\n");
#endif
	at_fixed_format(ATH);
}

void play_congestion_tone()
{
	memset(st_gsm_at.at.tx_data,0x00,sizeof(st_gsm_at.at.tx_data));
	st_gsm_at.at.state = AT_WAIT_SEND;
	st_gsm_at.at.order = AT_STTONE;
	memcpy(st_gsm_at.at.tx_data,(char *)"1,3,100000\r",strlen("1,3,100000\r"));
}

AT_STATE get_gsm_at_state()
{
	return (st_gsm_at.at.state);
}

void set_gsm_at_state( AT_STATE state )
{
	st_gsm_at.at.state = state;
}
void gsm_callin_answer(AT_ORDER_ID order)
{
	at_fixed_format(order); 
}

void gsm_pcm_answer(AT_ORDER_ID order)
{
	at_fixed_format(order);
	
}

void set_gsm_atorder(AT_ORDER_ID atorder)
{
	st_gsm_at.at.order = atorder;
}

void gsm_pcm_over(AT_ORDER_ID order)
{
	at_fixed_format(order);
	pcm_flag = 0;
}

void clear_at_rx_buff()
{
	memset(&st_gsm_at.rx, 0x00, sizeof(ST_AT_RX));
}

void gsm_init()
{
	pcm_flag = 0;
	answer_call = 0;
	miss_call = 0;
	answer_begin = 0;
	net_flag = 0;
	sms_update = 0;
	sms_operation = 0;
	memset(&st_gsm_at,0x00,sizeof(st_gsm_at));
	memset(&st_gsm_state,0x00,sizeof(st_gsm_state));	
	memset(&st_lock_sim,0x00,sizeof(st_lock_sim));
	
	atorder_file = fopen(AT_ORDER_FILE, "w+");// �ɶ���д������ļ�
	if (!atorder_file) 
	{
		fprintf(stderr, "Unable to create file '%s'\n", AT_ORDER_FILE);
		exit(1); // �����˳�
	}
}

void at_fixed_format(AT_ORDER_ID order)
{
	if (st_gsm_at.at.state == AT_IDLE)
	{
#ifdef _DBUG_
		printf("hello ...\n");
#endif
		memset(st_gsm_at.at.tx_data, 0x00, sizeof(st_gsm_at.at.tx_data));
		st_gsm_at.at.state = AT_WAIT_SEND;
		st_gsm_at.at.order = order;  // ����ָ�û����
	}
}

void dtmf_tone_dialing(char *number)
{
	if (st_gsm_at.at.state == AT_IDLE)
	{
		memset(st_gsm_at.at.tx_data,0x00,sizeof(st_gsm_at.at.tx_data));
		st_gsm_at.at.state = AT_WAIT_SEND;
		st_gsm_at.at.order = AT_VTS;
		memcpy(st_gsm_at.at.tx_data,number,strlen(number));
		memcpy(&st_gsm_at.at.tx_data[strlen(number)],(char *)";\r",strlen(";\r"));
	}
}

void dialing(char *number)
{
	if (st_gsm_at.at.state == AT_IDLE)
	{
		memset(st_gsm_at.at.tx_data,0x00,sizeof(st_gsm_at.at.tx_data));
		st_gsm_at.at.state = AT_WAIT_SEND;
		st_gsm_at.at.order = ATD;
		memcpy(st_gsm_at.at.tx_data,number,strlen(number));
		memcpy(&st_gsm_at.at.tx_data[strlen(number)],(char *)";\r",strlen(";\r"));
	}
}
void get_sim_imei()
{
	if (st_gsm_at.at.state == AT_IDLE)
	{
		memset(st_gsm_at.at.tx_data,0x00,sizeof(st_gsm_at.at.tx_data));
		st_gsm_at.at.state = AT_WAIT_SEND;
		st_gsm_at.at.order = ATI;
	}
}

void query_gsm_cell()
{
	if (st_gsm_at.at.state == AT_IDLE)
	{
		memset(st_gsm_at.at.tx_data,0x00,sizeof(st_gsm_at.at.tx_data));
		st_gsm_at.at.state = AT_WAIT_SEND;
		st_gsm_at.at.order = AT_MONI;
	}
}

void switch_to_gsm(char *mode)
{
	char temp[10];
	if (st_gsm_at.at.state == AT_IDLE)
	{
		memset(st_gsm_at.at.tx_data,0x00,sizeof(st_gsm_at.at.tx_data));
		st_gsm_at.at.state = AT_WAIT_SEND;
		st_gsm_at.at.order = AT_CNMP;
		sprintf(temp,"%s%s",mode,"\r");
		memcpy(&st_gsm_at.at.tx_data,temp,strlen(temp));
	}
}

/*****************************************************************
������void input_puk_code(char *puk , char *newpin)
���ܣ�ͨ��PUK �޸�PIN CODE
������at+cpin== 12345678 ,1234
ע�⣺
��ƣ�
ʱ�䣺
�޸ģ�
******************************************************************/
void input_puk_code(char *puk , char *newpin)
{
	char temp[20] = {0};
	memset(st_gsm_at.at.tx_data,0x00,sizeof(st_gsm_at.at.tx_data));
	if (st_gsm_at.at.state == AT_IDLE)
	{
		st_gsm_at.at.state = AT_WAIT_SEND;
		st_gsm_at.at.order = AT_CPIN;
		sprintf(temp,"%s,%s%s",puk,newpin,"\r");
		memcpy(st_gsm_at.at.tx_data,temp,strlen(temp));
	}
}

/*****************************************************************
������void modify_pin_code( uchar  channel,char *oldpassw ,char *newpassw )
���ܣ��޸�PIN CODE
������at++cpwd=="SC","5151","1234"
ע�⣺�޸�ǰ����ʹ��PIN
��ƣ�
ʱ�䣺
�޸ģ�
******************************************************************/
void modify_pin_code(char *oldpassw ,char *newpassw )
{
	char temp[20] = {0};
	memset(st_gsm_at.at.tx_data,0x00,sizeof(st_gsm_at.at.tx_data));
	if (st_gsm_at.at.state == AT_IDLE)
	{
		st_gsm_at.at.state = AT_WAIT_SEND;
		st_gsm_at.at.order = AT_CPWD;
		sprintf(temp,"%s%s,%s%s%s",oldpassw,"\"","\"",newpassw,"\"\r");
		memcpy(st_gsm_at.at.tx_data,temp,strlen(temp));
	}
}

/*****************************************************************
������void set_sim_card( uchar  channel,char *passw ,uchar flag )
���ܣ�SIM card ʹ�ܻ�ʹ��,
������flag:1��ʾʹ�ܣ�0��ʾ��ʹ�� passw:PIN CODE
ע�⣺at++clck=="SC",0,"1234"
��ƣ�
ʱ�䣺
�޸ģ�
******************************************************************/
void set_sim_card(char *passw ,uchar flag)
{
	char temp[20] = {0};
	memset(st_gsm_at.at.tx_data,0x00,sizeof(st_gsm_at.at.tx_data));
	if (st_gsm_at.at.state == AT_IDLE)
	{
		st_gsm_at.at.state = AT_WAIT_SEND;
		st_gsm_at.at.order = AT_CLCK;
		sprintf(temp,"%d%s%s%s",flag,",\"",passw,"\"\r");// "SC",1,1234
		memcpy(st_gsm_at.at.tx_data,temp,strlen(temp));
	}
}

/*****************************************************************
������void input_pin_code( uchar  channel,char *passw )
���ܣ�����PIN CODE
������at+cpin=82520634,1234
ע�⣺
��ƣ�
ʱ�䣺
�޸ģ�
******************************************************************/
void input_pin_code(char *passw)
{
	char temp[50] = {0};
	memset(st_gsm_at.at.tx_data,0x00,sizeof(st_gsm_at.at.tx_data));
	if (st_gsm_at.at.state == AT_IDLE)
	{
		st_gsm_at.at.state = AT_WAIT_SEND;
		st_gsm_at.at.order = AT_CPIN;
		sprintf(temp,"%s%s",passw,"\r");
		memcpy(st_gsm_at.at.tx_data,temp,strlen(temp));
	}
}

/*****************************************************************
������void query_sim_card(uchar channel)
���ܣ���ѯSIM���Ƿ�װ
******************************************************************/

void query_sim_card()
{
	at_fixed_format(AT_CPIN_R);
}

/*����״̬��ѯ*/
void network_status()
{
	at_fixed_format(AT_CPIN_R);
}

void gsm_init_and_sim_active(chanState *ports)
{
	switch (st_gsm_state.main)
	{		   
		case HS_NONEED_SIMCARD_QUERY:
			pthread_mutex_lock(&atorder_mutex);
			st_gsm_at.at.state = AT_IDLE;
			st_gsm_state.tx_overtime = GSM_RX_OVERTIM; // 10s�Ķ�ʱ
			at_fixed_format(gsm_noneed_simcard_query[st_gsm_state.init_step]);	//���sim���Ƿ�װ���Ƿ���Ҫ����
			st_gsm_state.main = HS_NONEED_SIMCARD_QUERY_WAIT;
			break;
		case HS_NONEED_SIMCARD_QUERY_WAIT:
			if (st_gsm_at.at.state == AT_ANSWER_OK)
			{
				st_gsm_state.init_step++;
				if ( st_gsm_state.init_step == sizeof(gsm_noneed_simcard_query)/sizeof(int))
				{
					//sem_post(&gsm_sem);
					//fclose(atorder_file);
					st_gsm_state.init_step = 0;
					st_gsm_state.main = HS_LOCKCARD_DETECT; //HS_LOCKCARD_DETECT
				}
				else
				{
					st_gsm_state.main = HS_NONEED_SIMCARD_QUERY;
				}
				pthread_mutex_unlock(&atorder_mutex);
			}
			else if (st_gsm_at.at.state == AT_ERROR ||st_gsm_state.tx_overtime == 0)
			{
				//st_gsm_state.main= HS_AT_INIT_ERROR;
				//pthread_mutex_unlock(&atorder_mutex);
				goto error_exit;
			}
		break;
#if 1
		case HS_LOCKCARD_DETECT: 		//�������
			pthread_mutex_lock(&atorder_mutex);
			st_gsm_at.at.state = AT_IDLE;
			st_gsm_state.tx_overtime = GSM_RX_OVERTIM; // 10s�Ķ�ʱ
			at_fixed_format(AT_CPIN_R);	//���sim���Ƿ�װ���Ƿ���Ҫ����
			st_gsm_state.main = HS_LOCKCARD_DETECT_WAIT;
			break;
		case HS_LOCKCARD_DETECT_WAIT:
			if (st_gsm_at.at.state == AT_ANSWER_UNLOCKSIM)
			{ 
				st_gsm_state.main = HS_AT_INIT;//HS_AT_INIT;HS_VOLUME
				pthread_mutex_unlock(&atorder_mutex);
#if 0
				if (st_lock_sim.card==1)
				{
					st_gsm_state.main=HS_AUTO_LOCKCARD_ENABLE;
				}
				else
				{
					st_gsm_state.main = HS_AT_INIT;//
				}
#endif
			}
			else if (st_gsm_at.at.state == AT_ANSWER_LOCKSIM)
			{		
				st_gsm_state.main = HS_LOCKCARD_DETECT;//	�������				
				pthread_mutex_unlock(&atorder_mutex);
				sleep(3);   // �ȴ�һ����ȥ���
			}
			else if (st_gsm_at.at.state == AT_ERROR)
			{
				st_gsm_state.main = HS_NOSIM;
				pthread_mutex_unlock(&atorder_mutex);
			}
			else if (st_gsm_state.tx_overtime == 0) //û�з���ֵ
			{
				goto error_exit;
			}
#endif
#if 0
			else if (st_gsm_state.tx_overtime == 0)
			{
				st_gsm_state.main = HS_LOCKCARD_DETECT;//	�������	
				pthread_mutex_unlock(&atorder_mutex);
			}
#endif
			break;
		case HS_NOSIM:
			break;
#if 0
		case HS_AUTO_LOCKCARD:
			if (st_gsm_state.tx_overtime==0)
			{
				st_gsm_at.at.state = AT_IDLE;
				input_pin_code((char *)DEFAULT_PINCODE); 
			}
			if (st_gsm_at.at.state==AT_ANSWER_OK)
			{
				if (st_lock_sim.card==1)
				{
					st_gsm_state.main=HS_AT_INIT;
				}
				else
				{
					st_gsm_state.main = HS_AUTO_LOCKCARD_DISABLE;//
				}
			}
			else if (st_gsm_at.at.state==AT_ANSWER_ERROR)
			{
#ifdef _DBUG_
				printf("pin code error\n");
				dbug_log(__FILE__,__LINE__,"PIN CODE ERROR\n");
#endif
				st_gsm_state.main = HS_AUTO_LOCKCARD_ERROR;
			}
			break;
		case HS_AUTO_LOCKCARD_DISABLE:
			if (st_gsm_state.tx_overtime==0)
			{
				st_gsm_at.at.state=AT_IDLE;
				set_sim_card(DEFAULT_PINCODE,0);
			}
			if (st_gsm_at.at.state ==AT_ANSWER_OK)
			{				
				st_gsm_state.main=HS_AT_INIT;
			}
			else if (st_gsm_at.at.state ==AT_ERROR)
			{
#ifdef _DBUG_
				printf("pin code error\n");
				dbug_log(__FILE__,__LINE__,"PIN CODE ERROR\n");
#endif
				st_gsm_state.main = HS_AUTO_LOCKCARD_ERROR;
			}
			break;
		case HS_AUTO_LOCKCARD_ENABLE:
			if (st_gsm_state.tx_overtime==0)
			{
				st_gsm_at.at.state=AT_IDLE;
				set_sim_card(DEFAULT_PINCODE,1);
			}
			if (st_gsm_at.at.state ==AT_ANSWER_OK)
			{
				if (st_lock_sim.pinmodify==1)
				{
					st_gsm_state.main=HS_AUTO_LOCKCARD_MODIFY;
				}
			}
			else if (st_gsm_at.at.state ==AT_ERROR)
			{
#ifdef _DBUG_
				printf("pin code error\n");
				dbug_log(__FILE__,__LINE__,"PIN CODE ERROR\n");
#endif
				st_gsm_state.main = HS_AUTO_LOCKCARD_ERROR;
			}
			break;
		case HS_AUTO_LOCKCARD_MODIFY:
			if (st_gsm_state.tx_overtime==0)
			{
				st_gsm_at.at.state=AT_IDLE;
				modify_pin_code((char *)DEFAULT_PINCODE,(char *)DEFAULT_LOCK_PINCODE);
			}
			if (st_gsm_at.at.state ==AT_ANSWER_OK)
			{
				st_gsm_at.at.state = AT_IDLE;
				st_gsm_state.main = HS_AT_INIT;

			}
			else if (st_gsm_at.at.state ==AT_ERROR)
			{
#ifdef _DBUG_
				printf("new pin code error\n");
				dbug_log(__FILE__,__LINE__,"NEW PIN CODE ERROR\n");
#endif
				st_gsm_state.main = HS_AUTO_LOCKCARD_ERROR;
			}
			break;
		case HS_AUTO_LOCKCARD_ERROR:
			break;
#endif
		case HS_SEM_WAIT:
			//sem_wait(&gsm_sem);
			st_gsm_state.main = HS_LOCKCARD_DETECT;// HS_AT_INIT;//HS_LOCKCARD_DETECT
			break;
		case HS_AT_INIT:								//��ʼ��		
			pthread_mutex_lock(&atorder_mutex);
			st_gsm_at.at.state = AT_IDLE;
			st_gsm_state.tx_overtime = GSM_RX_OVERTIM;	
			at_fixed_format(gsm_init_list[st_gsm_state.init_step]);
			st_gsm_state.main = HS_AT_INIT_WAIT;			
			break;
		case HS_AT_INIT_WAIT:
			if (st_gsm_at.at.state == AT_ANSWER_OK)
			{
				st_gsm_state.init_step++;
				if (st_gsm_state.init_step == sizeof(gsm_init_list)/sizeof(int))
				{
					//sem_post(&gsm_sem);
					//fclose(atorder_file);
					st_gsm_state.main = HS_VOLUME; //HS_LOCKCARD_DETECT
				}
				else
				{
					st_gsm_state.main = HS_AT_INIT;
				}
				pthread_mutex_unlock(&atorder_mutex);
			}
			else if (st_gsm_at.at.state == AT_ERROR ||st_gsm_state.tx_overtime == 0)
			{
				//st_gsm_state.main= HS_AT_INIT_ERROR;
				//pthread_mutex_unlock(&atorder_mutex);
				goto error_exit;
			}
#if 0
			else if (st_gsm_state.tx_overtime==0)
			{
				st_gsm_state.main = HS_AT_INIT;
				pthread_mutex_unlock(&atorder_mutex);
			}
#endif
			break;	
		case HS_AT_INIT_ERROR:
			break;
#if 0
		case HS_NOSIM:			// ���SIM���Ƿ�װ
			if ( st_gsm_state.tx_overtime == 0 )	
			{
				st_gsm_at.at.state = AT_IDLE;// ʹ����״̬
				query_sim_card();			
			}
			if (st_gsm_at.at.state == AT_ANSWER_OK )    // �в忨
			{
				st_gsm_at.at.state = AT_IDLE;
				st_gsm_state.main = HS_VOLUME;//HS_VOLUME

			}
			else
			{
				//��������Ƿ���SIM�� �ӳ����ʱ��
				st_gsm_state.tx_overtime = GSM_RX_OVERTIM;
			}
			break;
#endif
		case HS_VOLUME:
			pthread_mutex_lock(&atorder_mutex);
			st_gsm_at.at.state = AT_IDLE;// ʹ����״̬
			st_gsm_at.at_times = 0;
			st_gsm_state.tx_overtime = GSM_RX_OVERTIM;
			volume_adjust();			
			st_gsm_state.main = HS_VOLUME_WAIT;
			break;
		case HS_VOLUME_WAIT:
			if (st_gsm_at.at.state == AT_ANSWER_OK)    // 
			{
				//st_gsm_state.net_status_time = 6000; //��ʱ1���ӣ��������״̬
				st_gsm_at.at_times = 0;
				st_gsm_state.sim_status_time = SIM_ALSA_STATUS_TIME; //��ʱ30�룬���sim alsa�ػ�����״̬
				st_gsm_state.sim_csq_time = SIM_CSQ_TIME;
				st_gsm_state.main = HS_NORMAL;		 
				pthread_mutex_unlock(&atorder_mutex);
			}		
			else if (st_gsm_at.at.state == AT_ERROR)
			{
				st_gsm_at.at_times++;
				if (st_gsm_at.at_times >= AT_TIMES)
					goto error_exit;
				st_gsm_at.at.state = AT_IDLE;// ʹ����״̬
				volume_adjust();	
			}
			else if (st_gsm_state.tx_overtime == 0)
			{
				goto error_exit;
			}
			break;
		case HS_NORMAL:			// ��̬
			//break;
#ifdef _DBUG_
			//printf("gsm_init_and_sim_active exit\n");
#endif
			//pthread_exit(NULL);
			//simcom_reset(ports[0].ProObj); //simcom ģ�鸴λ
			//break;
#if 1		
			if (st_gsm_state.net_status_time == 0 && !net_flag)	
			{
				pthread_mutex_lock(&atorder_mutex);
				st_gsm_at.at.state = AT_IDLE;// ʹ����״̬
				st_gsm_state.tx_overtime = GSM_RX_OVERTIM;
				at_fixed_format(AT_QCRMCALL); //��ѯ����״̬
				st_gsm_state.net_status_time = NET_STATUS_TIME; //��ʱ1���ӣ��������״̬
				st_gsm_state.main = HS_NETWORK_SEEK;  
			}
			else if (st_gsm_state.sim_status_time == 0)
			{
				pthread_mutex_lock(&atorder_mutex);
				st_gsm_at.at.state = AT_IDLE;// ʹ����״̬
				st_gsm_state.tx_overtime = GSM_RX_OVERTIM;
				at_fixed_format(AT_CPCMREG0); //���sim alsa�ػ�����״̬
				st_gsm_state.sim_status_time = SIM_ALSA_STATUS_TIME; //��ʱ30�룬���sim alsa�ػ�����״̬
				st_gsm_state.main = HS_SIMCOM_ALSA_SEEK;
			}
			else if (st_gsm_state.sim_csq_time == 0)
			{
				pthread_mutex_lock(&atorder_mutex);
				st_gsm_at.at.state = AT_IDLE;// ʹ����״̬
				st_gsm_state.tx_overtime = GSM_RX_OVERTIM;
				at_fixed_format(AT_CSQ); //���sim alsa�ػ�����״̬
				st_gsm_state.sim_csq_time = SIM_CSQ_TIME; //��ʱ30�룬���sim alsa�ػ�����״̬
				st_gsm_state.main = HS_SIMCOM_CSQ_SEEK;
			}
			else if (st_gsm_state.sim_network_time == 0)
			{
				pthread_mutex_lock(&atorder_mutex);
				st_gsm_at.at.state = AT_IDLE;// ʹ����״̬
				st_gsm_state.tx_overtime = GSM_RX_OVERTIM;
				at_fixed_format(AT_CNSMOD); //���sim alsa�ػ�����״̬
				st_gsm_state.sim_network_time = SIM_NETWORK_TIME; //��ʱ50�룬���sim alsa�ػ�����״̬
				st_gsm_state.main = HS_NETWORK_MODE_SEEK;
			}
#ifdef SMS_ENABLE
			else if (sms_update == 1) //�����ռ���
			{
				pthread_mutex_lock(&atorder_mutex);
				st_gsm_at.at.state = AT_IDLE;// ʹ����״̬
				st_gsm_state.tx_overtime = GSM_RX_OVERTIM;
				at_fixed_format(AT_CMGL);
				st_gsm_state.main = HS_SIMCOM_SMS_UPDATE;
			}
			else if (sms_operation == SMS_SEND) // ���Ͷ���
			{
				pthread_mutex_lock(&atorder_mutex);
				st_gsm_at.at.state = AT_IDLE;// ʹ����״̬
				st_gsm_state.tx_overtime = GSM_RX_OVERTIM;
				gsm_cmgw();
				st_gsm_state.main = HS_SIMCOM_SMS_CMGW_WAIT;
			}
			else if (sms_operation == SMS_DELETE) //ɾ������
			{
				pthread_mutex_lock(&atorder_mutex);
				st_gsm_at.at_times = 0;
				st_gsm_at.at.state = AT_IDLE;// ʹ����״̬
				st_gsm_state.tx_overtime = GSM_RX_OVERTIM;
				gsm_cmgd();
				st_gsm_state.main = HS_SIMCOM_SMS_CMGD_WAIT;
			}
#endif
			break;
#ifdef SMS_ENABLE
		case HS_SIMCOM_SMS_CMGW_WAIT:
			if (st_gsm_at.at.state == AT_ANSWER_OK)
			{ 
				set_gsm_sub_state(GSM_SUBSTATE_SEND_SMS);
				set_gsm_atorder(AT_INIT);
				set_gsm_at_state(AT_IDLE);
				//fill_short_sms();  // text mode 
				//char psms[] = "�����hello ��";
				/*
				��ȡ�������ݣ���ȡĿ�ĺ���
				*/
				char psms[] = "hello";			
				printf("psms:%s\n", psms);
				GSM_SendSMS(psms, "8613016054035");
				st_gsm_state.tx_overtime = GSM_RX_OVERTIM;
				st_gsm_state.main = HS_SIMCOM_SMS_SEND_WAIT;
			}
			else if (st_gsm_state.tx_overtime == 0)
			{
				goto error_exit;
			}
			break;
		case HS_SIMCOM_SMS_CMGD_WAIT:
			if (st_gsm_at.at.state == AT_ANSWER_OK)    // 
			{				
				sms_operation = SMS_IDLE;
				st_gsm_at.at_times = 0;
				st_gsm_state.main = HS_NORMAL;		 
				pthread_mutex_unlock(&atorder_mutex);
			}		
			else if (st_gsm_at.at.state == AT_ERROR)
			{
				st_gsm_at.at_times++;
				if (st_gsm_at.at_times >= AT_TIMES)
					goto error_exit;
			}
			else if (st_gsm_state.tx_overtime == 0)
			{
				goto error_exit;
			}
			break;
		case HS_SIMCOM_SMS_SEND_WAIT:
			if (st_gsm_at.at.state == AT_ANSWER_OK)
			{ 
				st_gsm_at.at.state = AT_IDLE;// ʹ����״̬
				st_gsm_state.tx_overtime = GSM_RX_OVERTIM;
				send_sms();
				st_gsm_state.main = HS_SIMCOM_SMS_SEND;
			}
			else if (st_gsm_state.tx_overtime == 0)
			{
				goto error_exit;
			}
			break;
		case HS_SIMCOM_SMS_SEND:
			if (st_gsm_at.at.state == AT_ANSWER_OK)
			{ 
				st_gsm_state.main = HS_NORMAL; 
				sms_operation = SMS_IDLE;
				pthread_mutex_unlock(&atorder_mutex);
			}
			else if (st_gsm_state.tx_overtime == 0)
			{
				goto error_exit;
			}
			break;
		case HS_SIMCOM_SMS_UPDATE:
			if (st_gsm_at.at.state == AT_ANSWER_OK)
			{ 
				st_gsm_state.main = HS_NORMAL; 
				sms_update = 0;
				pthread_mutex_unlock(&atorder_mutex);
			}
			else if (st_gsm_state.tx_overtime == 0)
			{
				goto error_exit;
			}
			break;
#endif
		case HS_NETWORK_SEEK:
			if (st_gsm_at.at.state == AT_ANSWER_OK && net_enable == 1) //���ųɹ�,������Ҫ����
			{ 
				st_gsm_state.main = HS_NORMAL; 
				pthread_mutex_unlock(&atorder_mutex);
			}
			else if (st_gsm_at.at.state == AT_ANSWER_OK && net_enable == 0) //���ųɹ������ǲ���Ҫ����
			{
				st_gsm_state.main = HS_DIAG_DOWN; 
				pthread_mutex_unlock(&atorder_mutex);
			}
			else if (st_gsm_at.at.state == AT_ERROR && net_enable == 1) //����ʧ�ܣ���Ҫ����
			{
				st_gsm_state.main = HS_DIAG_UP; 
				pthread_mutex_unlock(&atorder_mutex);
			}
			else if (st_gsm_at.at.state == AT_ERROR && net_enable == 0) //����ʧ�ܣ�������Ҫ����
			{
				printf("st_gsm_at.at.state == AT_ERROR && net_enable == 0\n");
				st_gsm_state.main = HS_NORMAL; 
				net_flag = 1;
				pthread_mutex_unlock(&atorder_mutex);
			}
			else if (st_gsm_state.tx_overtime == 0)
			{
				goto error_exit;
			}
			break;		
		case HS_DIAG_UP:
			pthread_mutex_lock(&atorder_mutex);
			st_gsm_at.at.state = AT_IDLE;// ʹ����״̬
			st_gsm_state.tx_overtime = GSM_RX_OVERTIM;
			at_fixed_format(AT_QCRMCALL1); //��������
			st_gsm_state.main = HS_DIAG_UP_WAIT;  
			break;
		case HS_DIAG_UP_WAIT:
			if (st_gsm_at.at.state == AT_ANSWER_OK)
			{
				st_gsm_state.main = HS_NORMAL; 
				pthread_mutex_unlock(&atorder_mutex);
			}
			else if (st_gsm_at.at.state == AT_ERROR ||st_gsm_state.tx_overtime == 0)
			{
				st_gsm_state.main = HS_DIAG_UP; 
				pthread_mutex_unlock(&atorder_mutex);
			}
#if 0
			else if (st_gsm_state.tx_overtime == 0)
			{
				//goto error_exit;
			}
#endif
			break;
		case HS_DIAG_DOWN:
			pthread_mutex_lock(&atorder_mutex);
			st_gsm_at.at.state = AT_IDLE;// ʹ����״̬
			st_gsm_state.tx_overtime = GSM_RX_OVERTIM;
			at_fixed_format(AT_QCRMCALL0); // ��ֹ��������
			st_gsm_state.main = HS_DIAG_DOWN_WAIT;  
			break;
		case HS_DIAG_DOWN_WAIT:
			if (st_gsm_at.at.state == AT_ANSWER_OK)
			{
				st_gsm_state.main = HS_NORMAL; 
				net_flag = 1;
				pthread_mutex_unlock(&atorder_mutex);
			}
			else if (st_gsm_at.at.state == AT_ERROR || st_gsm_state.tx_overtime == 0)
			{
				st_gsm_state.main = HS_DIAG_DOWN; 
				pthread_mutex_unlock(&atorder_mutex);
			}
#if 0
			else if (st_gsm_state.tx_overtime == 0)
			{
				goto error_exit;
			}
#endif
			break;
		case HS_SIMCOM_ALSA_SEEK:
			if (st_gsm_at.at.state == AT_ANSWER_OK)
			{
				st_gsm_state.main = HS_NORMAL; 
				pthread_mutex_unlock(&atorder_mutex);
			}
			else if (st_gsm_at.at.state == AT_ERROR ||st_gsm_state.tx_overtime == 0)
			{
				//st_gsm_state.main = HS_NORMAL; 
				goto error_exit;		
			}
			break;
		case HS_SIMCOM_CSQ_SEEK:
			if (st_gsm_at.at.state == AT_ANSWER_OK)
			{
				st_gsm_state.main = HS_NORMAL; 
				pthread_mutex_unlock(&atorder_mutex);
			}
			else if (st_gsm_state.tx_overtime == 0)
			{
				//st_gsm_state.main = HS_NORMAL; 
				goto error_exit;		
			}
			break;
		case HS_NETWORK_MODE_SEEK:
			if (st_gsm_at.at.state == AT_ANSWER_OK)
			{
				st_gsm_state.main = HS_NORMAL; 
				pthread_mutex_unlock(&atorder_mutex);
			}
			else if (st_gsm_state.tx_overtime == 0)
			{
				//st_gsm_state.main = HS_NORMAL; 
				goto error_exit;		
			}
			break;
#endif
		default:
			break;
	}
	
	pthread_mutex_lock(&send_atorder_mutex);
	send_at_order();
	pthread_mutex_unlock(&send_atorder_mutex);
	return ;
error_exit:
	
	pthread_mutex_unlock(&atorder_mutex);
	printf("DBG: %s: software running error need restart\n", __FUNCTION__);
	simcom_disable(ports->ProObj); //ģ��ֱ�Ӷϵ�
	ProSLIC_ShutdownChannel(ports->ProObj);
	SiVoice_Reset(ports->ProObj);
	ProSLIC_destroyChannel(&(ports->ProObj));	
	exit(1);


}

void send_at_order()
{
	char	temp[50] = {0};
	uint	length;
	if (st_gsm_at.at.state == AT_WAIT_SEND)
	{
		length = strlen(at_order_table[st_gsm_at.at.order].order);		
#ifdef _DBUG_
		printf("at_order_table[st_gsm_at.at.order].order=%s\n",at_order_table[st_gsm_at.at.order].order);		
#endif
		dbug_log(__FILE__,__LINE__,"at_order_table[st_gsm_at.at.order].order=%s\n",at_order_table[st_gsm_at.at.order].order);
		memcpy(temp, at_order_table[st_gsm_at.at.order].order, length);
		memcpy(&temp[length], st_gsm_at.at.tx_data, strlen(st_gsm_at.at.tx_data));
		st_gsm_at.at.state = AT_WAIT_ANSWER;
		uart_send(fd, temp, strlen(temp));		
	}
	return ;
}

void *gsm_init_handler(void * arg)
{
	chanState *ports = (chanState *)arg;
	while (1)
	{
		usleep(10000);
		gsm_init_and_sim_active(ports);		
	}	
	pthread_exit(NULL);
}

int uart_pcm_send(int fd, char *send_buf,int data_len)
{
    int ret;  
    ret = write(fd,send_buf,data_len);
    if (ret == data_len)
    {	
    	return ret;
    } 
    else 
    {    
    	tcflush(fd,TCOFLUSH);    
		return FALSE;
    }
}

int uart_send2(int fd, char *send_buf,int data_len)
{
	int ret,fs_sel;
    fd_set fs_write;  
    struct timeval time;   
   	FD_ZERO(&fs_write);
    FD_SET(fd,&fs_write);   
    time.tv_sec = 0;
    time.tv_usec = 0;	// 200ms

	//ʹ��selectʵ�ִ��ڵķ�����
    fs_sel = select(fd+1,NULL, &fs_write, NULL, &time);
    if (fs_sel)
    {
    	if (FD_ISSET(fd, &fs_write))
    	{
			ret = write(fd, send_buf, data_len);
			return ret;
    	}
    }
	else
	{
		return FALSE;
	}	
    
}

int uart_send(int fd, char *send_buf,int data_len)
{
	int ret,fs_sel;
    fd_set fs_write;  
   	FD_ZERO(&fs_write);
    FD_SET(fd, &fs_write);   
	struct timeval time;  
	time.tv_sec = 0;
	time.tv_usec = 0;
	
	 //ʹ��selectʵ�ִ��ڵķ�����
    fs_sel = select(fd + 1,NULL, &fs_write, NULL, &time);
    if (fs_sel)
    {
    	if (FD_ISSET(fd, &fs_write))
    	{
			ret = write(fd, send_buf, data_len);
	    	if (ret == data_len )
	    	{
	    		return ret;
	    	} 
	    	else 
	    	{    
	    		tcflush(fd,TCOFLUSH);    
				return ret;
	    	}	
    	}
    }
	else
	{
		return FALSE;
	}	
}

int uart_rev(int fd, char *rcv_buf,int data_len)
{
    int len,fs_sel;
    fd_set fs_read;  
    struct timeval time;   
   	FD_ZERO(&fs_read);
    FD_SET(fd,&fs_read);   
    time.tv_sec = 0;
    time.tv_usec = 0;

	//ʹ��selectʵ�ִ��ڵĶ�·ͨ��
    fs_sel = select(fd + 1,&fs_read,NULL,NULL,&time);
    if (fs_sel)
    {
    	if (FD_ISSET(fd, &fs_read))
		{
			len = read(fd,rcv_buf,data_len);
			if (len == data_len)
			{
				return len;
			}
			else
			{
				//tcflush(fd,TCIFLUSH); 
			}
			return len;
    	}
    }	
	return FALSE;
}

int gsm_recv()
{
	int res = 0;	
	int i = 0;
	
	res = uart_rev(fd, st_gsm_at.rx.rx_data, sizeof(st_gsm_at.rx.rx_data));
	if (res > 0)
	{
		st_gsm_at.rx.rx_length = res;		
#ifdef _DBUG_
		printf("st_gsm_at.rx.rx_data = %s\n", st_gsm_at.rx.rx_data);		
#endif	
		dbug_log(__FILE__,__LINE__, "st_gsm_at.rx.rx_data = %s\n", st_gsm_at.rx.rx_data);
	}
	return res;
}

void  set_gsm_sub_state(GSM_SUBSTATE state )
{
	st_gsm_state.sub = state;
}

GSM_MAINSTATE get_gsm_mainstate()
{
	return (st_gsm_state.main);
}

GSM_SUBSTATE get_gsm_sub_state()
{
	return (st_gsm_state.sub);
}

void write_cmgl_information(const char *filename, const char *buf, int size)
{
	FILE *cmgl_short_sms;
	cmgl_short_sms = fopen(filename, "w+");
	fwrite(buf, 1, size, cmgl_short_sms);
	fclose(cmgl_short_sms);	
}

void save_sms_message(const char *filename,sms_info *psms, int size)
{
	int i = 0;
	char buf[512];
	FILE *sms_file = NULL;
	sms_file = fopen(filename, "w+");
	for(i = 0; i < size; i++)
	{
		sprintf(buf, "%d,%d,	%s,	20%d/%d/%d/%d:%d\r\n%s\r\n", psms[i].IndexNum, psms[i].message_type, psms[i].NumBuff, 
			psms[i].Timer.Year, psms[i].Timer.Month, psms[i].Timer.Day, psms[i].Timer.Hour, psms[i].Timer.Minute, psms[i].tp_ud);
		fwrite(buf, strlen(buf), 1, sms_file);		
	}
	fclose(sms_file);
}

int parse_pdu_sms(char *smsptr, sms_info *pmsg)
{
	int sms_num = 0;
	while ((smsptr = strstr(smsptr, "+CMGL:")) != NULL) 
	{ 
	    smsptr += 6;        														// ����"+CMGL:" 			         	
	    pmsg[sms_num].IndexNum = atoi(smsptr);			       
	    smsptr += 3;		       
	    pmsg[sms_num].message_type = atoi(smsptr);			        
	    smsptr = strstr(smsptr, "\r\n");    								// ����һ�� 
	    smsptr += 2;        														// ����"\r\n" 
		gsm_parsepdu_sms(smsptr, pmsg[sms_num].tp_ud, &pmsg[sms_num]);			       
	    sms_num++;        														// ׼������һ������Ϣ 
	} 

	return sms_num;
}

int inser_atorder_file(const char *file, char *buf, int size)
{
	FILE *fp,*fp1;
	char buf1[100][100];
	char temp[100] = {0};
	int k = 0, i = 0, result = 0, num = 0;

	if (atorder_file)
	{
		fclose(atorder_file);
		atorder_file = NULL;
	}
	fp = fopen(file, "a+");
	if (!fp)
		perror(" ");
	while ((fgets(temp, 50, fp)) !=0)
	{		
		strcpy(buf1[k], temp);
		k++;
	}
	for (i = 0; i < k; i++)
	{
		if (memcmp((char *)"AT+CSQ", buf1[i], 6) == 0)
		{
			fclose(fp);
			//remove(file); //����ɾ�� ��Ϊ�б�Ľ�������
			if ((fp1 = fopen(file,"w+")) == 0)  // �о����
			{
				perror("can not recreate file");
				return 0;
			}
			else
			{
				for (num = 0; num < i; num++)
				{
					fputs(buf1[num], fp1); 
				}
				fseek(fp1, 0, SEEK_END);
				fwrite(buf, 1, size, fp1);
				fflush(fp1);
				result = 1;
			}			
#if 0
			result = 1;
			break;
#endif
		}
		else if (memcmp((char *)"AT+CPIN?", buf1[i], 8) == 0)
		{
			for (num = i; num < k; num++)
			{
				fputs(buf1[num], fp1); 
			}
			fclose(fp1);
			return 0;
		}
	}
	if (result == 0)
	{
		fseek(fp, 0, SEEK_END);
		fwrite(buf, 1, size, fp);
		fclose(fp);
		return 0;
	}
	fclose(fp1);
	return 0;
#if 0
	if (result == 0)
	{
		fseek(fp, 0, SEEK_END);
		fwrite(buf, 1, size, fp);
		fclose(fp);
		return 0;
	}
	else
	{
		fclose(fp);
		//remove(file); //����ɾ�� ��Ϊ�б�Ľ�������
		if ((fp1 = fopen(file,"w+")) == 0)  // �о����
		{
			perror("can not recreate file");
			return 0;
		}
		else
		{
			for(num = 0; num < i; num++)
			{
				fputs(buf1[num], fp1); 
			}
			fseek(fp1, 0, SEEK_END);
			fwrite(buf, 1, size, fp1);
			fclose(fp1);
			return 0;
		}
	}
#endif
}

int insert_atorder_cnsmod(const char *file, char *buf, int size)
{
	FILE *fp,*fp1;
	char buf1[100][100];
	char temp[100] = {0};
	int k = 0, i = 0, result = 0, num = 0;

	if (atorder_file)
	{
		fclose(atorder_file);
		atorder_file = NULL;
	}
	fp = fopen(file, "a+");
	if (!fp)
		perror(" ");
	while ((fgets(temp, 50, fp)) !=0)
	{		
		strcpy(buf1[k], temp);
		k++;
	}
	for (i = 0; i < k; i++)
	{
		if (memcmp((char *)"AT+CNSMOD?", buf1[i], 10) == 0)
		{
			fclose(fp);
			if ((fp1 = fopen(file,"w+")) == 0)  // �о����
			{
				perror("can not recreate file");
				return 0;
			}
			else
			{
				for (num = 0; num < i; num++)
				{
					fputs(buf1[num], fp1); 
				}
				fseek(fp1, 0, SEEK_END);
				fwrite(buf, 1, size, fp1);
				fflush(fp1);
				result = 1;
			}			
		}
	}
	if (result == 0) // û�о�ֱ���ں����
	{
		fseek(fp, 0, SEEK_END);
		fwrite(buf, 1, size, fp);
		fclose(fp);
		return 0;
	}
	fclose(fp1);
	return 0;
}

void at_swicth()
{
	int i, j, n = 0;
	int length;	
	int ret = 0;
	char *smsptr = NULL;          // �ڲ��õ�����ָ�� 
	sms_info pmsg[50];
	int sms_num = 0;
	
	switch(st_gsm_at.at.order)
	{		
		case AT:
		case ATE0:
		case ATE1:
		case ATS0:
		case AT_W:
		case AT_CHFA:
		case AT_COLP:	
		case AT_CLIP:
		case AT_STTONE:  
		case AT_CMGF:	
		case AT_SMS:
		case AT_CMGD:
		case AT_CSCA:	
		case AT_CPWD:
		case AT_CPIN:
		case AT_CLCK:
		case AT_CLVL:	
		case AT_CMIC:
		case AT_VTS:
		case AT_CPAMP:	
		case AT_CVHU:
		case AT_CHUP:
		case AT_CODECSEL:
		case AT_CLCC:
		case AT_CNMP:
		case AT_CSQFMT:
		case AT_CPMS:
		case AT_CNMI:
		case AT_CMSS:
			for ( i = 0;i < st_gsm_at.rx.rx_length; i++ )
			{
				if (memcmp((char *)"OK",&st_gsm_at.rx.rx_data[i], 2) == 0)// ֻ�Ƕ�
				{
#ifdef _DBUG_
					printf("OK...........\n");
					dbug_log(__FILE__,__LINE__,"OK..............\n");
#endif
					st_gsm_at.at.state = AT_ANSWER_OK;
					memset(&st_gsm_at.rx,0x00,sizeof(ST_AT_RX));	
					break;
				}
				else if (memcmp((char *)"ERROR",&st_gsm_at.rx.rx_data[i], 5) == 0)
				{
#ifdef _DBUG_
					printf("error\n");
					st_gsm_at.at.state = AT_ERROR;
					dbug_log(__FILE__,__LINE__,"ERROR\n");
#endif
					memset(&st_gsm_at.rx,0x00,sizeof(ST_AT_RX));	
					break;
				}						
			}
			break;
		case AT_CPCMREGOVER:
			for ( i = 0;i < st_gsm_at.rx.rx_length; i++ )
			{
				if (memcmp((char *)"OK",&st_gsm_at.rx.rx_data[i],2) == 0)// ֻ�Ƕ�
				{
#ifdef _DBUG_
					printf("AT_CPCMREGOVER OK...........\n");
					dbug_log(__FILE__,__LINE__,"OK..............\n");
#endif
					st_gsm_at.at.state = AT_ANSWER_OK;
					memset(&st_gsm_at.rx,0x00,sizeof(ST_AT_RX));	
					break;
				}
				else if (memcmp((char *)"ERROR",&st_gsm_at.rx.rx_data[i],5) == 0 )
				{
#ifdef _DBUG_
					printf("AT_CPCMREGOVER error\n");
					st_gsm_at.at.state = AT_ERROR;
					dbug_log(__FILE__,__LINE__,"ERROR\n");
#endif
					memset(&st_gsm_at.rx,0x00,sizeof(ST_AT_RX));	
					break;
				}			
			}
			break;
		case ATH:	
			for ( i = 0;i < st_gsm_at.rx.rx_length; i++ )
			{
				if (memcmp((char *)"OK",&st_gsm_at.rx.rx_data[i],2) == 0)// ֻ�Ƕ�
				{
#ifdef _DBUG_
					printf("ATH OK...........\n");
					dbug_log(__FILE__,__LINE__,"OK..............\n");
#endif
					st_gsm_at.at.state = AT_ANSWER_OK;
					memset(&st_gsm_at.rx,0x00,sizeof(ST_AT_RX));	
					break;
				}
				else if (memcmp((char *)"ERROR",&st_gsm_at.rx.rx_data[i],5) == 0 )
				{
#ifdef _DBUG_
					printf("ATH ERROR\n");
					st_gsm_at.at.state = AT_ERROR;
					dbug_log(__FILE__,__LINE__,"ERROR\n");
#endif
					memset(&st_gsm_at.rx,0x00,sizeof(ST_AT_RX));	
					break;
				}			
			}
			break;
		case AT_QCRMCALL0:
		case AT_QCRMCALL1:
			for ( i = 0;i < st_gsm_at.rx.rx_length; i++ )
			{
				if (memcmp((char *)"OK",&st_gsm_at.rx.rx_data[i],2) == 0)// ֻ�Ƕ�
				{
#ifdef _DBUG_
					printf("OK...........\n");
					dbug_log(__FILE__,__LINE__,"OK..............\n");
#endif
					st_gsm_at.at.state = AT_ANSWER_OK;
					memset(&st_gsm_at.rx,0x00,sizeof(ST_AT_RX));	
					break;
				}
				else if (memcmp((char *)"ERROR",&st_gsm_at.rx.rx_data[i],5) == 0 )
				{
#ifdef _DBUG_
					printf("error\n");
					st_gsm_at.at.state = AT_ERROR;
					dbug_log(__FILE__,__LINE__,"ERROR\n");
#endif
					memset(&st_gsm_at.rx,0x00,sizeof(ST_AT_RX));	
					break;
				}				
			}
		break;
		case AT_MONI:
			st_gsm_at.cell_num = 0;
			n = 0;
			memset(st_gsm_at.cellid,0x0,sizeof(st_gsm_at.cellid));
			for ( i = 0;i < st_gsm_at.rx.rx_length; i++ )
			{
				if (memcmp((char *)"Id:",&st_gsm_at.rx.rx_data[i],3) == 0)
				{
					i += 5; //�м����һ���ո�
					j = i;					
					for (;i < st_gsm_at.rx.rx_length; i++)
					{
						if (st_gsm_at.rx.rx_data[i] == ',' || st_gsm_at.rx.rx_data[i] == ']')
						{
							memcpy(st_gsm_at.cellid[n++],&st_gsm_at.rx.rx_data[i],i - j);
#ifdef _DBUG_
							printf("cellid%d: %s\n",n,st_gsm_at.cellid[n - 1]);
#endif
							dbug_log(__FILE__,__LINE__,"cellid%d: %s\n",n,st_gsm_at.cellid[n - 1]);
							break;
						}
					}				
				}
			}
			if (n > 0)
			{
				st_gsm_at.cell_num = n;
				st_gsm_at.at.state = AT_ANSWER_OK;
			}
			else
			{
				st_gsm_at.at.state = AT_ERROR;
			}
			break;
		case ATI:
			for (i=0; i < st_gsm_at.rx.rx_length; i++ )
			{
				if (memcmp((char *)"IMEISV:",&st_gsm_at.rx.rx_data[i],7) == 0)// ֻ�Ƕ�
				{
					i+=8;
					memset(&st_gsm_at.imei,0x0,sizeof(st_gsm_at.imei));
					memcpy(&st_gsm_at.imei,&st_gsm_at.rx.rx_data[i],15);
#ifdef _DBUG_
					printf("imei=%s\n",st_gsm_at.imei);
#endif
					dbug_log(__FILE__,__LINE__,"imei=%s\n",st_gsm_at.imei);
					st_gsm_at.at.state = AT_ANSWER_OK;
					memset(&st_gsm_at.rx,0x00,sizeof(ST_AT_RX));	
					break;
				}
			}
			break;
		case AT_CPCMREG:
			for ( i = 0;i < st_gsm_at.rx.rx_length; i++ )
			{
				if (memcmp((char *)"OK",&st_gsm_at.rx.rx_data[i],2) == 0)// ֻ�Ƕ�
				{
#ifdef _DBUG_
					printf("OK...........\n");
					dbug_log(__FILE__,__LINE__,"OK..............\n");
#endif
					st_gsm_at.at.state = AT_ANSWER_OK;
					memset(&st_gsm_at.rx,0x00,sizeof(ST_AT_RX));	
					break;
				}
				else if (memcmp((char *)"ERROR",&st_gsm_at.rx.rx_data[i],5) == 0 )
				{
#ifdef _DBUG_
					printf("error...........\n");
					dbug_log(__FILE__,__LINE__,"ERROR\n");
#endif
					st_gsm_at.at.state = AT_ANSWER_ERROR;
					memset(&st_gsm_at.rx,0x00,sizeof(ST_AT_RX));	
					break;
				}
			}
			break;

		case AT_CPIN_R:// �����û��sim��   // ����ط����Բ���
			atorder_file = fopen(AT_ORDER_FILE, "a+");// �ɶ���д������ļ�
			if (!atorder_file) 
			{
				fprintf(stderr, "Unable to create file '%s'\n", AT_ORDER_FILE);
				exit(1); // �����˳�
			}
			ret = fwrite(st_gsm_at.rx.rx_data, 1, st_gsm_at.rx.rx_length, atorder_file);
			fflush(atorder_file);
			st_gsm_at.at.order = AT_INIT;
			length = strlen("+CPIN");
			for ( i = 0; i < st_gsm_at.rx.rx_length; i++ )
			{
				if (memcmp((char *)"+CPIN",&st_gsm_at.rx.rx_data[i],length) == 0)
				{
					//"+CPIN:  READY",��ʾ��װ
					i+=length;
					for (;i<st_gsm_at.rx.rx_length;i++)
					{
						if (memcmp((char *)"READY",&st_gsm_at.rx.rx_data[i],5) == 0)
						{
							st_gsm_at.at.state = AT_ANSWER_UNLOCKSIM;			
							memset(&st_gsm_at.rx,0x00,sizeof(ST_AT_RX));	
							break;
						}
						else if (memcmp((char *)"SIM PIN",&st_gsm_at.rx.rx_data[i],7)==0)
						{
							st_gsm_at.at.state = AT_ANSWER_LOCKSIM;
							memset(&st_gsm_at.rx,0x00,sizeof(ST_AT_RX));	
							break;
						}
#if 1
						else if ((memcmp((char *)"ERROR",&st_gsm_at.rx.rx_data[i],5) == 0) )// ��ʾû�а�װ
						{
							st_gsm_at.at.state = AT_ERROR;
							//st_gsm_state.main = HS_NOSIM;
#ifdef _DBUG_
							printf("There is no SIMCARD\n");
							dbug_log(__FILE__,__LINE__,"There is no SIMCARD\n");
#endif
							memset(&st_gsm_at.rx,0x00,sizeof(ST_AT_RX));
							break;
						}
#endif
					}
					break;			
				}		
#if 0
				else if ((memcmp((char *)"ERROR",&st_gsm_at.rx.rx_data[i],5) == 0) )// ��ʾû�а�װ
				{
					st_gsm_at.at.state = AT_ERROR;
#ifdef _DBUG_
					printf("There is no SIMCARD\n");
					dbug_log(__FILE__,__LINE__,"There is no SIMCARD\n");
#endif
					memset(&st_gsm_at.rx,0x00,sizeof(ST_AT_RX));
					break;
				}
#endif
			}
			break;		
		case ATD:
			for ( i = 0; i < st_gsm_at.rx.rx_length; i++ )
			{
				if (memcmp((char *)"BEGIN",&st_gsm_at.rx.rx_data[i],5)==0)
				{
#ifdef _DBUG_
					printf("ATD/ATA OK\n");
					dbug_log(__FILE__,__LINE__,"ATD/ATA OK\n");
#endif
					st_gsm_at.at.state = AT_ATD_CONNECT_OK;
					memset(&st_gsm_at.rx,0x00,sizeof(ST_AT_RX));	
					break;
				}
				else if (memcmp((char *)"ERROR",&st_gsm_at.rx.rx_data[i],5) == 0)
				{
#ifdef _DBUG_
					printf("ATD/ATA ERROR\n");
					dbug_log(__FILE__,__LINE__,"ATD/ATA ERROR\n");
#endif
					st_gsm_at.at.state = AT_ATD_ANSWER_ERROR;
					memset(&st_gsm_at.rx,0x00,sizeof(ST_AT_RX));	
					break;
				}
				else if (memcmp((char *)"NO CARRIER",&st_gsm_at.rx.rx_data[i],10) == 0)
				{
#ifdef _DBUG_
					printf("ATD/ATA NO CARRIER\n");
					dbug_log(__FILE__,__LINE__,"ATD/ATA NO CARRIER\n");
#endif
					st_gsm_at.at.state = AT_ATD_ANSWER_ERROR;
					memset(&st_gsm_at.rx,0x00,sizeof(ST_AT_RX));	
					break;
				}
				else if (memcmp((char *)"OK", &st_gsm_at.rx.rx_data[i], 2) == 0)
				{
#ifdef _DBUG_
					printf("ATD/ATA OK\n");
					dbug_log(__FILE__,__LINE__,"ATD/ATA OK\n");
#endif
					st_gsm_at.at.state = AT_ATD_ANSWER_OK;
					memset(&st_gsm_at.rx,0x00,sizeof(ST_AT_RX));	
					break;
				}
			}
			break;
			
		case ATA:
			for ( i = 0; i < st_gsm_at.rx.rx_length; i++ )
			{
				if (memcmp((char *)"BEGIN",&st_gsm_at.rx.rx_data[i],5)==0)
				{
#ifdef _DBUG_
					printf("ATD/ATA OK\n");
					dbug_log(__FILE__,__LINE__,"ATD/ATA OK\n");
#endif
					st_gsm_at.at.state = AT_CONNECT_OK;
					memset(&st_gsm_at.rx,0x00,sizeof(ST_AT_RX));	
					break;
				}
				else if (memcmp((char *)"ERROR",&st_gsm_at.rx.rx_data[i],5) == 0)
				{
#ifdef _DBUG_
					printf("ATD/ATA ERROR\n");
					dbug_log(__FILE__,__LINE__,"ATD/ATA ERROR\n");
#endif
					st_gsm_at.at.state = AT_ANSWER_ERROR;
					memset(&st_gsm_at.rx,0x00,sizeof(ST_AT_RX));	
					break;
				}
				else if (memcmp((char *)"NO CARRIER",&st_gsm_at.rx.rx_data[i],10) == 0)
				{
#ifdef _DBUG_
					printf("ATD/ATA NO CARRIER\n");
					dbug_log(__FILE__,__LINE__,"ATD/ATA NO CARRIER\n");
#endif
					st_gsm_at.at.state = AT_ANSWER_ERROR;
					memset(&st_gsm_at.rx,0x00,sizeof(ST_AT_RX));	
					break;
				}
			}
			break;
		case AT_QCRMCALL:
			for ( i = 0;i < st_gsm_at.rx.rx_length; i++ )
			{
				if (memcmp((char *)"$QCRMCALL:",&st_gsm_at.rx.rx_data[i],10) == 0)// ��ʾ���ųɹ�
				{
#ifdef _DBUG_
					printf("OK...........\n");
					dbug_log(__FILE__,__LINE__,"OK..............\n");
#endif
					st_gsm_at.at.state = AT_ANSWER_OK;
					memset(&st_gsm_at.rx,0x00,sizeof(ST_AT_RX));	
					break;
				}
#if 1
				else if (memcmp((char *)"ERROR",&st_gsm_at.rx.rx_data[i],5) == 0) // ��ʾû�в忨
				{
#ifdef _DBUG_
					printf("error\n");
					st_gsm_at.at.state = AT_ERROR;
					dbug_log(__FILE__,__LINE__,"ERROR\n");
#endif
					memset(&st_gsm_at.rx,0x00,sizeof(ST_AT_RX));	
					break;
				}
				else if (memcmp((char *)"OK",&st_gsm_at.rx.rx_data[i],2) == 0) // ��ʾ�Ѿ����˿�������û�гɹ�
				{
#ifdef _DBUG_
					printf("error\n");
					st_gsm_at.at.state = AT_ERROR;
					dbug_log(__FILE__,__LINE__,"ERROR\n");
#endif
					memset(&st_gsm_at.rx,0x00,sizeof(ST_AT_RX));	
					break;
				}
#endif
			}
			break;
		case AT_CPCMREG0:
			for ( i = 0; i < st_gsm_at.rx.rx_length; i++ )
			{
				if (memcmp((char *)"+CPCMREG:", &st_gsm_at.rx.rx_data[i], 9) == 0)
				{
					if (memcmp((char *)"+CPCMREG: 0", &st_gsm_at.rx.rx_data[i], 11) == 0)// ��ʾsimģ��alsa��������
					{
#ifdef _DBUG_
						printf("OK...........\n");
						dbug_log(__FILE__,__LINE__,"OK..............\n");
#endif
						st_gsm_at.at.state = AT_ANSWER_OK;
						memset(&st_gsm_at.rx,0x00,sizeof(ST_AT_RX));	
						break;
					}
					else if (memcmp((char *)"+CPCMREG: 1", &st_gsm_at.rx.rx_data[i], 11) == 0)// ��ʾsimģ��alsa��������
					{
#ifdef _DBUG_
						printf("OK...........\n");
						dbug_log(__FILE__,__LINE__,"OK..............\n");
#endif
						st_gsm_at.at.state = AT_ANSWER_OK;
						memset(&st_gsm_at.rx,0x00,sizeof(ST_AT_RX));	
						break;

					}
					else
					{
#ifdef _DBUG_
						printf("error\n");
						st_gsm_at.at.state = AT_ERROR;
						dbug_log(__FILE__,__LINE__,"ERROR\n");
#endif
						memset(&st_gsm_at.rx, 0x00, sizeof(ST_AT_RX));	
						break;
					}
				}
			}
			break;
		case AT_CMGL:
			for ( i = 0; i < st_gsm_at.rx.rx_length; i++ )
			{
				if (strstr(st_gsm_at.rx.rx_data, "OK") != NULL)
				{
#ifdef _DBUG_
					printf("OK...........\n");
					dbug_log(__FILE__,__LINE__,"OK..............\n");
#endif
					smsptr = st_gsm_at.rx.rx_data;
					sms_num = parse_pdu_sms(smsptr, pmsg);
					save_sms_message(SHORT_SMS_FILE, pmsg, sms_num);
					//write_cmgl_information(SHORT_SMS_FILE, st_gsm_at.rx.rx_data, st_gsm_at.rx.rx_length);  //TEXT MODE
					st_gsm_at.at.state = AT_ANSWER_OK;
					memset(&st_gsm_at.rx,0x00,sizeof(ST_AT_RX));	
					break;
				}
				else if (strstr(st_gsm_at.rx.rx_data, "ERROR") != NULL)					
				{
#ifdef _DBUG_
					printf("error\n");
					st_gsm_at.at.state = AT_ERROR;
					dbug_log(__FILE__,__LINE__,"ERROR\n");
#endif
					memset(&st_gsm_at.rx,0x00,sizeof(ST_AT_RX));	
					break;
				}	
				else if (memcmp((char *)"AT+CMGL",&st_gsm_at.rx.rx_data[i], strlen("AT+CMGL")) == 0)
				{
					printf("AT+CMGL\n");
					memset(&st_gsm_at.rx,0x00,sizeof(ST_AT_RX));	
					break;
				}
			}
			break;
		case AT_CMGW:
			for ( i = 0; i < st_gsm_at.rx.rx_length; i++ )
			{
				if (memcmp((char *)">", &st_gsm_at.rx.rx_data[i], 1) == 0)
				{
#ifdef _DBUG_
					printf("OK...........\n");
					dbug_log(__FILE__,__LINE__,"OK..............\n");
#endif
					st_gsm_at.at.state = AT_ANSWER_OK;
					memset(&st_gsm_at.rx,0x00,sizeof(ST_AT_RX));	
					break;
				}
			}
			break;
		case AT_CGMM:
			for ( i = 0; i < st_gsm_at.rx.rx_length; i++ )
			{
				if (memcmp((char *)"SIMCOM_SIM7100C", &st_gsm_at.rx.rx_data[i], strlen("SIMCOM_SIM7100C")) == 0)
				{
#ifdef _DBUG_
					printf("OK...........\n");
					dbug_log(__FILE__,__LINE__,"OK..............\n");
#endif
					st_gsm_at.at.state = AT_ANSWER_OK;
					memset(&st_gsm_at.rx,0x00,sizeof(ST_AT_RX));	
					break;
				}
				else if (memcmp((char *)"SIMCOM_SIM7100E", &st_gsm_at.rx.rx_data[i], strlen("SIMCOM_SIM7100E")) == 0)
				{
#ifdef _DBUG_
					printf("OK...........\n");
					dbug_log(__FILE__,__LINE__,"OK..............\n");
#endif
					st_gsm_state.init_step++; // ����AT_CSCODELָ��
					st_gsm_at.at.state = AT_ANSWER_OK;
					memset(&st_gsm_at.rx,0x00,sizeof(ST_AT_RX));	
					break;
				}
				else if (memcmp((char *)"SIMCOM_SIM7100A", &st_gsm_at.rx.rx_data[i], strlen("SIMCOM_SIM7100A")) == 0)
				{
#ifdef _DBUG_
					printf("OK...........\n");
					dbug_log(__FILE__,__LINE__,"OK..............\n");
#endif
					st_gsm_state.init_step++; // ����AT_CSCODELָ��
					st_gsm_at.at.state = AT_ANSWER_OK;
					memset(&st_gsm_at.rx,0x00,sizeof(ST_AT_RX));	
				}
				else if (memcmp((char *)"SIMCOM_SIM7500A", &st_gsm_at.rx.rx_data[i], strlen("SIMCOM_SIM7500A")) == 0)
				{
#ifdef _DBUG_
					printf("OK...........\n");
					dbug_log(__FILE__,__LINE__,"OK..............\n");
#endif
					st_gsm_state.init_step++; // ����AT_CSCODELָ��
					st_gsm_at.at.state = AT_ANSWER_OK;
					memset(&st_gsm_at.rx,0x00,sizeof(ST_AT_RX));	
				}
			}
			break;			
#if 1
		case AT_CIMI:
		case AT_COPS:
		case AT_CGSN:
			fwrite(st_gsm_at.rx.rx_data, 1, st_gsm_at.rx.rx_length, atorder_file);
			fflush(atorder_file);
			st_gsm_at.at.order = AT_INIT;
			st_gsm_at.at.state = AT_ANSWER_OK;
			memset(&st_gsm_at.rx, 0x00, sizeof(ST_AT_RX));	
			break;	
		case AT_CSUB:
			fwrite(st_gsm_at.rx.rx_data, 1, st_gsm_at.rx.rx_length, atorder_file);
			fflush(atorder_file);
			//fclose(atorder_file);
			st_gsm_at.at.order = AT_INIT;
			st_gsm_at.at.state = AT_ANSWER_OK;
			memset(&st_gsm_at.rx, 0x00, sizeof(ST_AT_RX));	
			break;	
		case AT_CSQ:
			inser_atorder_file(AT_ORDER_FILE, st_gsm_at.rx.rx_data, st_gsm_at.rx.rx_length);
			st_gsm_at.at.order = AT_INIT;
			st_gsm_at.at.state = AT_ANSWER_OK;
			memset(&st_gsm_at.rx, 0x00, sizeof(ST_AT_RX));	
			break;
		case AT_CNSMOD:
			insert_atorder_cnsmod(AT_ORDER_FILE, st_gsm_at.rx.rx_data, st_gsm_at.rx.rx_length);
			st_gsm_at.at.order = AT_INIT;
			st_gsm_at.at.state = AT_ANSWER_OK;
			memset(&st_gsm_at.rx, 0x00, sizeof(ST_AT_RX));	
			break;
#endif
		default:
			break; 
	}
}

void get_call_number(char *callnum)
{
	strncpy(callnum,st_gsm_at.callin_nu,strlen(st_gsm_at.callin_nu));
	//strncpy(callnum,"13016054035",strlen("13016054035"));
}

void gsm_sms_handler()
{
	int i = 0;
	for (i = 0; i < st_gsm_at.rx.rx_length; i++)
	{
		if (memcmp((char *)"+CMTI: \"SM\"",&st_gsm_at.rx.rx_data[i],strlen((char *)"+CMTI: \"SM\"")) == 0)
		{
			//�ж������ˣ���Ҫ���¶�����
			sms_update = 1;
			memset(&st_gsm_at.rx, 0x00, sizeof(ST_AT_RX));
		}
	}
}

void gsm_rx_query_handler()
{
	int i,j;
	if (get_gsm_sub_state() == GSM_SUBSTATE_IDLE)			// ��״̬Ϊ����״̬������״̬Ϊ��̬ &&(get_gsm_mainstate() == HS_NORMAL)
	{
		for (i = 0; i < st_gsm_at.rx.rx_length; i++ )
		{
			if (memcmp((char *)"CLIP",&st_gsm_at.rx.rx_data[i],4) == 0)
			{
				i += 4;
				for ( ; i < st_gsm_at.rx.rx_length; i++ )
				{
					if (memcmp((char *)"\"",&st_gsm_at.rx.rx_data[i],1) == 0)
					{
						i += 1;
						j = i;	
						for ( ; i < st_gsm_at.rx.rx_length; i++ )
						{
							if (memcmp((char *)"\"",&st_gsm_at.rx.rx_data[i],1) == 0)
							{
								memset(&st_gsm_at.callin_nu[0],0x00,14);
								if ( (i-j) > 14 )
								{
									memcpy(&st_gsm_at.callin_nu[0],&st_gsm_at.rx.rx_data[j],14);
								}
								else
								{
									memcpy(&st_gsm_at.callin_nu[0],&st_gsm_at.rx.rx_data[j],i-j);
								}
#ifdef _DBUG_
								printf("st_gsm_at.callin_nu=%s\n",st_gsm_at.callin_nu);							
#endif
								dbug_log(__FILE__,__LINE__,"st_gsm_at.callin_nu=%s\n",st_gsm_at.callin_nu);
								set_gsm_sub_state(GSM_SUBSTATE_RING);			// ���ô�״̬Ϊ����ģʽ							 
								memset(&st_gsm_at.rx,0x00,sizeof(ST_AT_RX));
								return ;
							}
						}
					}
				}
			}
		}
	}
	else if (get_gsm_sub_state() == GSM_SUBSTATE_DIALING)
	{
		for ( i = 0; i < st_gsm_at.rx.rx_length; i++ )
		{
			if (memcmp((char *)"BEGIN",&st_gsm_at.rx.rx_data[i],5)==0)
			{
#ifdef _DBUG_
				printf("ATD OK\n");
#endif
				dbug_log(__FILE__,__LINE__,"ATD OK\n");
				set_gsm_sub_state(GSM_SUBSTATE_CALLING_CONNECT);
				memset(&st_gsm_at.rx,0x00,sizeof(ST_AT_RX));	
				return ;
			}			
			else if (memcmp((char *)"NO CARRIER",&st_gsm_at.rx.rx_data[i],10) == 0)
			{
#ifdef _DBUG_
				printf("ATD NO CARRIER\n");		
#endif
				dbug_log(__FILE__,__LINE__,"ATD NO CARRIER\n");
				set_gsm_sub_state(GSM_SUBSTATE_CALLING_DISCONNECT);
				memset(&st_gsm_at.rx,0x00,sizeof(ST_AT_RX));	
				return ;
			}
		}
	}
	else if ((get_gsm_sub_state() == GSM_SUBSTATE_RING) ||(get_gsm_sub_state() == GSM_SUBSTATE_ANSWERING )) //(get_gsm_sub_state() == GSM_SUBSTATE_RING_ANSWER)	// �ȴ����������������
	{
		for ( i = 0; i < st_gsm_at.rx.rx_length; i++ )
		{
			if (!memcmp((char *)"NO CAR",&st_gsm_at.rx.rx_data[i],3) ||
			!memcmp((char *)"MISSED_CALL",&st_gsm_at.rx.rx_data[i],11)) //����
			{
#ifdef _DBUG_
				printf("NO CAR\n");
#endif
				dbug_log(__FILE__,__LINE__,"NO CAR\n");
				set_gsm_sub_state(GSM_SUBSTATE_HANG_UP);
				memset(&st_gsm_at.rx, 0x00, sizeof(ST_AT_RX)); 
				return ;
			}
		}
	}
	else if (get_gsm_sub_state() == GSM_SUBSTATE_SEND_SMS)
	{
		for ( i = 0; i < st_gsm_at.rx.rx_length; i++ )
		{
			if (memcmp((char *)"+CMGW:", &st_gsm_at.rx.rx_data[i], 6) == 0) // +CMGW: 19
			{
#ifdef _DBUG_
				printf("OK...........\n");
				dbug_log(__FILE__,__LINE__,"OK..............\n");
#endif			
				i += 7; 
				st_gsm_at.sms_num = atoi(&st_gsm_at.rx.rx_data[i]);
				st_gsm_at.at.state = AT_ANSWER_OK;
				set_gsm_sub_state(GSM_SUBSTATE_IDLE);	
				memset(&st_gsm_at.rx,0x00,sizeof(ST_AT_RX));	
				return ;
			}
		}
	}
	at_swicth();
}

void *gsm_send_handler(void *arg)
{
	while (1)
	{
		send_at_order();
	}
	pthread_exit(NULL);
}

void *gsm_recv_handler(void *arg)
{
	int ret = 0;

	while (1)
	{
		usleep(10000);
		ret = gsm_recv();
		if (ret > 0)
		{
#ifdef SMS_ENABLE
			gsm_sms_handler();
#endif
			gsm_rx_query_handler();	
		}
	}

	pthread_exit(NULL);
}

void *gsm_audio_recv(void *arg)
{
#if 0
	int res,i;
	FILE *fp;
	uchar audiobuf[320];
	fp=fopen("./audio","w+");
	if (fp==NULL)
	{
		printf("fopen failed\n");
		pthread_exit(NULL);
	}
	while (1)
	{
		if (pcm_flag==1)
		{
			memset(audiobuf,0,sizeof(audiobuf));
			res=uart_rev(fd2,audiobuf,sizeof(audiobuf));	//gsm audio
			if (res<0)
				printf("no PCM\n");
			if (res>0)
			{
				uart_send(fd3, audiobuf, res);// ���͵�slic audio
			}
		}
	}
#endif
	pthread_exit(NULL);
}

