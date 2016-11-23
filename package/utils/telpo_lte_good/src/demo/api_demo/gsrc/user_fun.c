#include "gsm.h"
#include "si3217x.h"
extern Si3217x_General_Cfg Si3217x_General_Configuration;
extern Si3217x_Impedance_Cfg Si3217x_Impedance_Presets [];// ��������������ⲿ�����
#define MAX_INT_STRINGS_MAIN 38
char *int_map_strings[] = 
{
    "IRQ_OSC1_T1",
    "IRQ_OSC1_T2",
    "IRQ_OSC2_T1",
    "IRQ_OSC2_T2",
    "IRQ_RING_T1",
    "IRQ_RING_T2",
    "IRQ_PM_T1",
    "IRQ_PM_T2",
    "IRQ_FSKBUF_AVAIL", /**< FSK FIFO depth reached */
    "IRQ_VBAT", 
    "IRQ_RING_TRIP", /**< Ring Trip detected */
    "IRQ_LOOP_STATUS",  /**< Loop Current changed */
    "IRQ_LONG_STAT",
    "IRQ_VOC_TRACK",
    "IRQ_DTMF",         /**< DTMF Detected - call @ref proslic_dtmf_read_digit to decode the value */
    "IRQ_INDIRECT",     /**< Indirect/RAM access completed */
    "IRQ_TXMDM",
    "IRQ_RXMDM",
    "IRQ_PQ1",          /**< Power alarm 1 */
    "IRQ_PQ2",          /**< Power alarm 2 */
    "IRQ_PQ3",          /**< Power alarm 3 */
    "IRQ_PQ4",          /**< Power alarm 4 */
    "IRQ_PQ5",          /**< Power alarm 5 */
    "IRQ_PQ6",          /**< Power alarm 6 */
    "IRQ_RING_FAIL",
    "IRQ_CM_BAL",
    "IRQ_USER_0",
    "IRQ_USER_1",
    "IRQ_USER_2",
    "IRQ_USER_3",
    "IRQ_USER_4",
    "IRQ_USER_5",
    "IRQ_USER_6",
    "IRQ_USER_7",
    "IRQ_DSP",
    "IRQ_MADC_FS",
    "IRQ_P_HVIC",
    "IRQ_P_THERM", /**< Thermal alarm */
    "IRQ_P_OFFLD"
};

#define MAX_INT_IMPEDANCE 30
char *impedance_match_table[] = {
	"600_0_0",					// 0   US PBX ���� ̨��
	"900_0_0",					// 1	  ͨ�ñ�׼	
	"900+2.16uf",				// 2	����ʵ����
	"270_750_150",			// 3	CTR21
	"200_680_100",			// 4	China CO
	"200_560_100",			// 5	China PBX	
	"370_620_310",			// 6	ӡ�� ������
	"220_820_115",			// 7	�¹�(��������)
	"320_1050_230",		// 8 Ӣ��(legacy)
	"220_820_120",			// 9 �Ĵ�����	
	"600+2.16uf",				// 10 �仯�ģ�������
	//"600+1uf",							// Japan CO  ������
	//"100_1000_100",					// Japan PBX ������
};
//Si3217x_Impedance_Presets[];

int gain_num[] = {-9};// dB
int rxgain_num[] = {-7, -6, -5, -4, -3};// dB

char *tone_math_table[] = { // ��������æ��������һ��
	"Australia", 		// �Ĵ�����
	"Austria",			// �µ���
	"Brazil",			// ����
	"Belgium",		// ����ʱ
	"China",			// �й�
	"Czech",			// �ݿ�
	"Denmark",		// ����
	"Finland",			// ����
	"France",			// ����
	"Germany", 		// �¹�
	"Britain",			// Ӣ��
	"Greece",			// ϣ��
	"Hungary",		// ������
	"Lithuania",		// ������
	"India",			// ӡ��
	"Italy",				// �����
	"Japan",			// �ձ�
	"Korean",			// ����
	"Mexico",			// ī����
	"New Zealand",	// ������
	"Netherlands",		// ����
	"Norway",		// Ų��
	"Portugal",		// ������
	"Spain",			// ������
	"Switzerland",	// ��ʿ
	"Sweden",		// ���
	"Russia",			// ����˹
	"USA",				// ����
};

char *ring_math_table[];

void time_handler(int k)
{
	gsm_delay_time();
	dial_delay_time();
}

void set_timer()
{
	struct itimerval tim;
	tim.it_interval.tv_sec=0; 
	tim.it_interval.tv_usec=10000;// 10 ms ,���ʱ��

	tim.it_value.tv_sec=0;
	tim.it_value.tv_usec=10000; //��ʼʱ�䣬��ʱ�䳬ʱʱ���ͻ��Զ��Ѽ��ʱ�丳ֵ����
	setitimer(ITIMER_REAL, &tim,NULL);// ����ʵ��ʱ�����
}


int getdata_from_sys_file(char *conf_path,char *get_value)
{
	FILE *conf_fp;
	int ret,i=0;
	if (!(conf_fp=fopen(conf_path,"r"))) // open file pointer 
	{	
		printf("%s opened failed\n",conf_path);
		return 0;
	}
	fseek( conf_fp, 0, SEEK_SET );
	while (feof(conf_fp) == 0)
	{
			ret=fread(&get_value[i],1,100,conf_fp);
			if (ret)
			{
				i+=ret;
			}
			
	}
	get_value[i+ret+1]='\0';
	return 1;
}

int setvar_to_sys_file(char *conf_path, char *set_name, char *set_value)
{
	 FILE *fp,*fp1;
	 char buf[CALL_COUNT][100];
	 char temp[100]={0};
	 int k=0,i=0;
	 int len=strlen(set_name);
	 
    fp=fopen(conf_path,"a+");
 	 if (!fp)
 	 {
	 	perror(" ");
		return -1;
 	 }
	
	while ((fgets(temp,50,fp))!=0)
	{		
		if (memcmp(temp,set_name,len)==0)
		{
			memset(temp,0x0,sizeof(temp));
			sprintf(temp,"%s=%s\n",set_name,set_value);
		}	
		strcpy(buf[k],temp);
		k++;
	}
	
	fclose(fp);
	remove(conf_path);
	if ((fp1=fopen(conf_path,"w+"))==0)
	{
		printf("can not recreate file \n");
		return -1 ;
	}
	else
	{
		for(i=0;i<k;i++)
		{
			fputs(buf[i],fp1); 
		}			
	}
	fflush(fp1);
	fclose(fp1);
	return 0;
}

int getvar_from_sys_file(char *conf_path, char *get_name, char **get_value)
{
	static int i;

	FILE *conf_fp;
	static char renamecmd[100];

	if (!(conf_fp = fopen(conf_path,"r"))) // open file pointer 
	{	
		printf("%s opened failed\n",conf_path);
		return 0;
	}
	
	fseek(conf_fp, 0, SEEK_SET);
	 i= strlen(get_name);
	while (feof(conf_fp) == 0)
	{
		fscanf(conf_fp, "%s\n", renamecmd);	//Read next record		
		if (!memcmp(renamecmd, &get_name[0], i))
		{
			*get_value = &renamecmd[i+1];
			fclose(conf_fp);
			return 1;
		}
	}
	return 0;	
}

int get_config_file(char *conf_path, char *get_name, char *option_name, char **get_value)
{
	FILE *conf_fp;
	char *p1 = NULL;
	char *p2 = NULL;
	int i, result = 0;
	char renamecmd[1024];
	if (!(conf_fp = fopen(conf_path,"r"))) // open file pointer 
	{	
		printf("%s opened failed\n",conf_path);
		return 0;
	}
	fseek(conf_fp, 0, SEEK_SET);
	i= strlen(get_name);
	while (feof(conf_fp) == 0)
	{
		fgets(renamecmd, 1024, conf_fp);  //Read next record 
		if (!strstr(renamecmd, get_name)) // gainset
		{	
			continue;
		}
		else 
		{
			result = 1;
			break;
		}
	}
	if (result == 1)
	{
		while (feof(conf_fp) == 0)
		{		
			memset(renamecmd, 0x0, sizeof(renamecmd));
			fgets(renamecmd, 1024, conf_fp);  //Read next record 
			if (!strstr(renamecmd, option_name)) // gainset
			{
				if (strstr(renamecmd, "config"))
				{
					return -1;
				}
				else
				{
					continue;
				}
			}
			else
			{
				p1 = strchr(renamecmd, '\'');
				p2 = strchr(p1 + 1, '\'');
				*p2 = '\0';
				*get_value = p1 + 1;
				return 0;
			}
		}
	}
	return -1;
}

int get_misc_config()
{
	char *temp = NULL;
	if (get_config_file(MISCELLANEOUS_FILE, "misc", "dialing_key", &temp) < 0)
	{
		printf("get_config_file of MISCELLANEOUS_FILE dialing_key failed\n");
		return 0;
	}	
#ifdef _DBUG_
	printf("get_dialing_key temp=%s\n",temp);
#endif
	if (*temp == '#')
		dialing_key = 1;
	else if (*temp == '*')
		dialing_key = 2;
	else
		dialing_key = 0;
	printf("dialing_key = %d\n", dialing_key);

	if (get_config_file(MISCELLANEOUS_FILE, "misc", "dial_up_timeout", &temp) < 0)
	{
		printf("get_config_file of MISCELLANEOUS_FILE dial_up_timeout failed\n");
		return 0;
	}	
#ifdef _DBUG_
	printf("get_dial_up_timeout temp=%s\n",temp);
#endif
	dial_up_timeout = atoi(temp);
	printf("dial_up_timeout = %d\n", dial_up_timeout);

	if (get_config_file(MISCELLANEOUS_FILE, "misc", "call_id_enable", &temp) < 0)
	{
		printf("get_config_file of MISCELLANEOUS_FILE call_id_enable failed\n");
		return 0;
	}	
#ifdef _DBUG_
	printf("get_call_id_enable temp=%s\n",temp);
#endif
	if (atoi(temp) == 1)
		call_id_enable = 1;
	else 
		call_id_enable = 0;
	printf("call_id_enable = %d\n", call_id_enable);

	if (get_config_file(MISCELLANEOUS_FILE, "misc", "call_id_mode", &temp) < 0)
	{
		printf("get_config_file of MISCELLANEOUS_FILE call_id_mode failed\n");
		return 0;
	}	
#ifdef _DBUG_
	printf("get_call_id_mode temp=%s\n",temp);
#endif
	if (memcmp(temp, "DTMF", 4) == 0)
		call_id_mode = 1;
	else if (memcmp(temp, "FSK", 3) == 0)
		call_id_mode = 0;
	printf("call_id_mode = %d\n", call_id_mode);
#if 0
	if (get_config_file(MISCELLANEOUS_FILE, "misc", "call_antipole", &temp) < 0) //���ŷ���
	{
		printf("get_config_file of MISCELLANEOUS_FILE call_antipole failed\n");
		return 0;
	}
	
#ifdef _DBUG_
	printf("get_call_antipole temp=%s\n",temp);
#endif
	if (atoi(temp) == 1)
		call_antipole = 1;
	else 
		call_antipole = 0;
	printf("call_antipole = %d\n", call_antipole);
#endif
#if 0
	if (get_config_file(MISCELLANEOUS_FILE, "misc", "jitter_delay", &temp) < 0)
	{
		printf("get_config_file of MISCELLANEOUS_FILE jitter_delay failed\n");
		return 0;
	}
	
#ifdef _DBUG_
	printf("get_jitter_delay  temp=%s\n",temp);
#endif
	jitter_delay = atoi(temp) /10;

	printf("jitter_delay = %d\n", jitter_delay);
#endif
#if 0
	if (get_config_file(MISCELLANEOUS_FILE, "misc", "max_jitter_delay", &temp) < 0)
	{
		printf("get_config_file of MISCELLANEOUS_FILE max_jitter_delay failed\n");
		return 0;
	}	
#ifdef _DBUG_
	printf("get_max_jitter_delay  temp=%s\n",temp);
#endif
	max_jitter_delay = atoi(temp) /10;

	printf("max_jitter_delay = %d\n", max_jitter_delay);
#endif
	return 1;
}

int get_net_enable()
{
	char *temp = NULL;
	if (get_config_file(NETWORK_FILE, "LTE", "lte_network", &temp) < 0)
	{
		printf("get_config_file of NETWORK_FILE enable failed\n");
		return 0;
	}
	
#ifdef _DBUG_
	printf("get_net_enable temp=%s\n",temp);
#endif
	if (atoi(temp) == 1)
		net_enable = 1;
	else
		net_enable = 0;
	printf("net_enable = %d\n", net_enable);
	return 1;
}

int get_map_enable()
{
	char *temp = NULL;
#if 0
	if (!getvar_from_sys_file(VOLUME_SYS_FILE,"map_enable",&temp))
		return 0;
#endif
	if (get_config_file(DIGITAL_MAP, "dialset", "enable", &temp) < 0)
	{
		printf("get_config_file of DIGITAL_MAP enable failed\n");
		return 0;
	}
#ifdef _DBUG_
		printf("temp=%s\n",temp);
#endif
	if (atoi(temp) == 1)
		map_enable = TRUE;
	else
		map_enable = FALSE;
#if 0	
	if (!getvar_from_sys_file(VOLUME_SYS_FILE,"unmatch_undial",&temp))
		return 0;
#endif

	if (get_config_file(DIGITAL_MAP, "dialset", "policy", &temp) < 0)
	{
		printf("get_config_file of DIGITAL_MAP policy failed\n");
		return 0;
	}
#ifdef _DBUG_
		printf("temp = %s\n",temp);
#endif
	if (atoi(temp) == 1)
		unmatch_undial = TRUE;
	else
		unmatch_undial = FALSE;
	return 1;	
}

int get_sms_flag()
{
	char *temp = NULL;
	if (get_config_file(SMS_CONFIG_FILE, "sms", "flag", &temp) < 0)
	{
		printf("get_config_file of SMS_CONFIG_FILE enable failed\n");
		return 0;
	}
	if (atoi(temp) == 0) 
	{		
		sms_operation = SMS_IDLE; //ɶ������
	}
	else if (atoi(temp) == 1) // ���Ͷ���
	{
		sms_operation = SMS_SEND;
	}
	else if (atoi(temp) == 2)
	{
		sms_operation = SMS_DELETE;
	}
}

int get_time_of_day(call_log *call,int flag)
{
	time_t timep;
	struct tm *p;
	time(&timep);
	p = localtime(&timep);
	if (flag == 1)
	{
		call->start_time.tm_year = p->tm_year + 1900;
		call->start_time.tm_mon = p->tm_mon + 1;
		call->start_time.tm_mday = p->tm_mday;
		call->start_time.tm_hour = p->tm_hour;
		call->start_time.tm_min = p->tm_min;
		call->start_time.tm_sec = p->tm_sec;
	}
	else
	{
		call->end_time.tm_year = p->tm_year + 1900;
		call->end_time.tm_mon = p->tm_mon + 1;
		call->end_time.tm_mday = p->tm_mday;
		call->end_time.tm_hour = p->tm_hour;
		call->end_time.tm_min = p->tm_min;
		call->end_time.tm_sec = p->tm_sec;
	}
	return 0;
}

struct calltime time_compare(call_log *call)
{
	struct calltime comtime;
	unsigned long ms,ms1,ms2;

	ms1 = call->start_time.tm_hour * 3600 + call->start_time.tm_min * 60 + call->start_time.tm_sec;
	ms2 = call->end_time.tm_hour * 3600 + call->end_time.tm_min * 60 + call->end_time.tm_sec;
	ms = ms2-ms1;
	comtime.tm_hour = ms/3600;
	comtime.tm_min = ms%3600/60;
	comtime.tm_sec = ms%3600%60;
	return comtime;
}

void insert_call_log(const char* logfile, call_log *call)
{
	FILE *fp,*fp1;
	char buf[CALL_COUNT][100];
	char temp[100] = {0};
	struct calltime comtime;
	comtime = time_compare(call);
	int k = 0;
    fp = fopen(logfile,"a+");
 	 if (!fp)
	 	perror(" ");

	while ((fgets(temp,50,fp)) !=0)
	{		
		strcpy(buf[k], temp);
		k++;
	}
	
	if (k == CALL_COUNT)// ��¼����
	{
		fclose(fp);
		//remove(logfile); //����ɾ���������н�������
		if ((fp1=fopen(logfile,"w+"))==0)
		{
			printf("can not recreate file \n");
			return ;
		}
		else
		{
			for(k = 1; k < CALL_COUNT; k++)
			{
				fputs(buf[k],fp1); 
			}
			fprintf(fp1,"%s|%02d.%02d %02d:%02d|%02d:%02d:%02d\n",call->call_number,call->start_time.tm_mon,call->start_time.tm_mday,
				call->start_time.tm_hour,call->start_time.tm_min,comtime.tm_hour,comtime.tm_min,comtime.tm_sec);//����һ���µļ�¼
			fclose(fp1);
			return ;
		}
	}
	else
	{
		fseek(fp, 0, SEEK_END);
		fprintf(fp,"%s|%02d.%02d %02d:%02d|%02d:%02d:%02d\n",call->call_number,call->start_time.tm_mon,call->start_time.tm_mday,
				call->start_time.tm_hour,call->start_time.tm_min,comtime.tm_hour,comtime.tm_min,comtime.tm_sec);
		sprintf(temp,"%s|%02d.%02d %02d:%02d|%02d:%02d:%02d\n",call->call_number,call->start_time.tm_mon,call->start_time.tm_mday,
				call->start_time.tm_hour,call->start_time.tm_min,comtime.tm_hour,comtime.tm_min,comtime.tm_sec);

#ifdef _DBUG_
		printf("%s",temp);
#endif
	}
	
	fclose(fp);
	return ;
}

int init_machine()
{
	fd = fd2 = -1;
	map_enable = TRUE;
	unmatch_undial = TRUE;
	net_enable = 0;
	dialing_key = 1; 			// Ĭ����1
	dial_up_timeout = 5; 	// Ĭ����5s
	call_id_enable = 1; 		// Ĭ��ʹ��
	call_id_mode = 0;			// Ĭ��fsk
	if (!get_map_enable())
	{
#ifdef _DBUG_
		printf("get map_enable failed\n");
#endif
	}
	if (!get_digital_map())
	{
#ifdef _DBUG_
		printf("get_digital_map failed\n");
#endif
	}
	if (!get_net_enable())
	{
#ifdef _DBUG_
		printf("get_net_enable failed\n");
#endif
	}
#if 1// ��ȡ�������ò���
	if (!get_misc_config())
	{
		printf("get_misc_config failed\n");
	}
#endif
	//watchdog_state = 0x2;
	//ioctl(fd5,WATCHDOG_ENABLE);// ʹ�ܿ��Ź�
	return 0;
}

/*��ȡ��ͼ*/
int get_digital_map()
{
	char *map = NULL;
#if 0
	if (!getvar_from_sys_file(VOLUME_SYS_FILE,"digital_map",&map))
		return 0;
#endif
	if (get_config_file(DIGITAL_MAP, "dialset", "dial", &map) < 0)
	{
		printf("get_config_file of DIGITAL_MAP failed\n");
		return 0;
	}
	int len = strlen(map) + 1;//�����һ�ֽڿռ�
	pattern = (char *)malloc(len);
	memcpy(pattern, map, len);
	dbug_log(__FILE__,__LINE__,"pattern = %s\n", pattern);
	pattern[len-1]='\0'; //���ӽ�����
#ifdef _DBUG_
	printf("pattern = %s\n",pattern);
#endif
	return 1;
}

/*�������*/
void volume_adjust()
{
	char volume[2];
	volume[0] = '3';
#if 0
	if (!getvar_from_sys_file(VOLUME_SYS_FILE, "volume", &volume))// ��ȡ���ַ���
	{
		printf("read file error\n");
		volume="3";
	}
	if (get_config_file(VOLUME_SYS_FILE, "gainset", "db", &volume) < 0)
	{
		printf("get_config_file of DIGITAL_MAP failed\n");
		return 0;
	}
#endif
#ifdef _DBUG_
	printf("volume = %s\n", volume);
#endif
	if (st_gsm_at.at.state == AT_IDLE)
	{
		memset(st_gsm_at.at.tx_data,0x00,sizeof(st_gsm_at.at.tx_data));
		st_gsm_at.at.state = AT_WAIT_SEND;
		st_gsm_at.at.order = AT_CLVL;
		memcpy(st_gsm_at.at.tx_data,volume,strlen(volume));
		memcpy(&st_gsm_at.at.tx_data[strlen(volume)],(char *)"\r",strlen("\r"));
#ifdef _DBUG_
		printf("st_gsm_at.at.tx_data=%s\n",st_gsm_at.at.tx_data);
#endif
	}
	return ;
}

int uart_open(char *arg)
{
	int fd, val, times = 0;
	struct termios newtio;
	
	while (access(arg, F_OK) == -1) //����ļ��Ƿ���ڣ���������ھ͵ȴ�
	{
		perror("");
		printf (" file not exit\n");
		sleep(3);
		times ++;
		if (times >= 10)
			//system("reboot"); // �������30s ��ϵͳ��λ
			exit(1);
	}
	
	//fd=open(arg,O_RDWR|O_NOCTTY|O_NDELAY);
	fd=open(arg, O_RDWR);
	if (fd<0)
	{
		printf("open failed\n");
		goto error_exit;
	}
#if 1
	val = fcntl(fd, F_GETFL, 0);
	if(val < 0)
	{
		perror("fcntl get error");
		goto error_exit;
	}
	val |= O_NONBLOCK; //����Ϊ������	
	 if (fcntl(fd,F_SETFL,val)<0)// 0 ��ʾ�����ļ�״̬��־���ָ���Ĭ��״̬
	 {
		printf("set failed\n");
		goto error_exit;
	 }
#endif
	 //����豸�����Ƿ�Ϊ�ն�
	 if (isatty(fd) == 0)	 
	 {
		printf("%s is not a termios device\n", arg);
		goto error_exit;
	 }
	else
	{
		printf("isatty success!\n");
		printf("termios device name = %s\n", ttyname(fd));
	}
#if 1
	if(tcgetattr(fd, &newtio) != 0)
	{
		goto error_exit;
	}

	cfmakeraw(&newtio);

	/*set baudrate*/
	cfsetispeed(&newtio, B115200);
	cfsetospeed(&newtio, B115200);

	/*set char bit size*/
	newtio.c_cflag &= ~CSIZE;
	newtio.c_cflag |= CS8;

	/*set check sum*/
	newtio.c_cflag &= ~PARENB;

	/*set stop bit*/
	newtio.c_cflag &= ~CSTOPB;
	newtio.c_cflag |= CLOCAL |CREAD;
	newtio.c_cflag &= ~(PARENB | PARODD);

	newtio.c_iflag &= ~(INPCK | BRKINT |PARMRK | ISTRIP | INLCR | IGNCR | ICRNL);
	newtio.c_iflag |= IGNBRK;
	newtio.c_iflag &= ~(IXON|IXOFF|IXANY);

	newtio.c_oflag = 0;

	newtio.c_lflag = 0;

	/*set wait time*/
	newtio.c_cc[VMIN] = 0;
	newtio.c_cc[VTIME] = 0;

	tcflush(fd, TCIFLUSH);
	tcflush(fd, TCOFLUSH);

	if(tcsetattr(fd, TCSANOW, &newtio) !=0 )
	{
		goto error_exit;
	}

	return fd;

error_exit:

	if(fd >= 0)
	{
		close(fd);
		fd = -1;
	}
	return -1;
#endif
	
}

int pcm_channel_open(char *arg)
{
	int fd = -1;
	fd = uart_open2(arg);// ��������
	if (fd < 0)
	{
#ifdef _DBUG_
		printf("uart_open failed\n");
#endif
		return -1;
	}
	return fd;
}

int uart_open2(char *arg)
{
	int fd, val, times = 0;
	struct termios newtio;

	while (access(arg, F_OK) == -1) //����ļ��Ƿ���ڣ���������ھ͵ȴ�
	{
		sleep(3);
		times ++;
		if (times >= 10)
			//system("reboot"); // �������30s ��ϵͳ��λ
			exit(1);
	}
	
	fd = open(arg, O_RDWR);
	if (fd < 0)
	{
		printf("open failed\n");
		goto error_exit;
	}
#if 0
	val = fcntl(fd, F_GETFL, 0); //��ȡ��ǰ�ļ�״̬
	if(val < 0)
	{
		perror("fcntl get error");
		goto error_exit;
	}
	val |= O_NONBLOCK; //����Ϊ������	
	 if (fcntl(fd,F_SETFL,val)<0)// 
	 {
		printf("set failed\n");
		goto error_exit;
	 }
#endif
	 //����豸�����Ƿ�Ϊ�ն�
	 if (isatty(fd) == 0)	 
	 {
		printf("%s is not a termios device\n", arg);
		goto error_exit;
	 }
	else
	{
		printf("isatty success!\n");
		printf("termios device name = %s\n", ttyname(fd));
	}
#if 1
	if(tcgetattr(fd, &newtio) != 0)
	{
		goto error_exit;
	}

	cfmakeraw(&newtio);

	/*set baudrate*/
	cfsetispeed(&newtio, B115200);
	cfsetospeed(&newtio, B115200);

	/*8n1*/
	newtio.c_cflag &= ~CSIZE;
	newtio.c_cflag |= CS8;
	newtio.c_cflag &= ~(PARENB | PARODD);
	newtio.c_cflag &= ~CSTOPB;

	newtio.c_cflag |= CLOCAL |CREAD; // no modem control, enable receiver

	newtio.c_iflag &= ~(INPCK | BRKINT |PARMRK | ISTRIP | INLCR | IGNCR | ICRNL);
	newtio.c_iflag |= IGNBRK;
	newtio.c_iflag &= ~(IXON|IXOFF|IXANY); 
	newtio.c_oflag = 0;

	newtio.c_lflag = 0;

	/*set wait time*/
	newtio.c_cc[VMIN] = 0;	// �ж��յ����ֽ���Ҫ����1��������Զ������Ϊ0 ��ʱʱ�䵽��������
	newtio.c_cc[VTIME] = 0;  // 1/10 s ����1s�Ķ�ʱ ֻ��read���ã���writeû������
	newtio.c_oflag = 0;

	tcflush(fd, TCIFLUSH);
	tcflush(fd, TCOFLUSH);

	if(tcsetattr(fd, TCSANOW, &newtio) !=0 )
	{
		goto error_exit;
	}

	return fd;

error_exit:
	if(fd >= 0)
	{
		close(fd);
		fd = -1;
	}
	return -1;
#endif
	
}

int national_standard_match_handler(chanState port)
{
	proslicChanType *pSlic;
	pSlic = port.ProObj;
	char *temp = NULL;
	int i = 0, preset = 0, num = 0, result = 0;
	
	if (get_config_file(VOLUME_SYS_FILE, "gainset", "impedance", &temp) < 0)
	{
		printf("get_config_file of impedance_math impedance failed\n");
		return 0;
	}
	printf("temp = %s, %d\n", temp, strlen(temp));
	for (i = 0; i < MAX_INT_IMPEDANCE; i++)
	{
		if (memcmp(temp, impedance_match_table[i], strlen(temp)) == 0)
		{
			result = 1;
			break;
		}
	}
	if (result == 0)
		preset = 0;
	else
		preset = i;
	printf("preset = %d\n", preset);
#if 0
	if (get_config_file(VOLUME_SYS_FILE, "gainset", "db1", &temp) < 0)
	{
		printf("get_config_file of gainset db failed\n");
		return 0;
	}
	num = atoi(temp);
	printf("tx_gain = %d\n", num);
#endif
	num = 0;
	printf("tx_gain_num[%d] = %d\n", num, gain_num[num]);
	Si3217x_Impedance_Presets[preset].txgain_db = gain_num[num];// �ı䷢������,�������治��	
	if (get_config_file(VOLUME_SYS_FILE, "gainset", "rx_db", &temp) < 0)
	{
		printf("get_config_file of gainset db failed\n");
		num = 5;
		//return 0;
	}
	else
		num = atoi(temp);
	printf("rx_gain = %d\n", num);
	Si3217x_Impedance_Presets[preset].rxgain_db = rxgain_num[num - 1];	// �ı��������
	printf("rx_gain_num[%d] = %d\n", num - 1, rxgain_num[num - 1]);
	//Si3217x_Impedance_Presets[preset].rxgain_db = gain_num[num];	// �ı��������
	ProSLIC_ZsynthSetup(pSlic, preset);
	// ��ѯ��·������ұ�׼
	// �趨��Χ��20~30
	if (get_config_file(MISCELLANEOUS_FILE, "misc", "codec_current", &temp) < 0)
	{
		printf("get_config_file of miscellaneous codec_current failed\n");
		preset = 0;
		//return 0;
	}
	else
	{
		printf("codec_current = %s\n", temp);
		preset = atoi(temp) - 20;
	}
	ProSLIC_DCFeedSetup(pSlic, preset);  
#if 0
	// ��ѯ�������ȹ��ұ�׼
	if (get_config_file(TONE_DC_RING_FILE, "xxxx", "xxxx", &temp) < 0)
	{
		printf("get_config_file of impedance_math impedance failed\n");
		return 0;
	}
	printf("temp = %s, %d\n", temp, strlen(temp));
	for (i = 0; i < MAX_INT_IMPEDANCE; i++)
	{
		if (memcmp(temp, tone_math_table[i], strlen(temp)) == 0)
		{
			result = 1;
			break;
		}
	}
	if (result == 0)
		preset = 0;
	else
		preset = i;
	printf("preset = %d\n", preset);
	proslic_tone_gen_setup(pSlic, preset); 
	ProSLIC_RingSetup(pSlic, preset);	
#endif	
	return 1;
}
