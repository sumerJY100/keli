
void change_pack();					  //�ѽ��յ������ݰ�ת�������0X55 ת���0X54 0X01 0X54 �滻��0X54 02
extern data unsigned char stac_a;		      //ȫ��ר�ñ���
extern unsigned char stac_b;			  //ȫ��ר�ñ���
extern xdata unsigned char send_buf[64];	                     //���ͻ�����
extern idata unsigned char recd_buf[64];						 //���ջ�����
extern data unsigned char temp_buf[72];
extern unsigned char two_lab;
extern void make_AT_CIP3(void);
extern code unsigned char AT_CIP3[];
extern code unsigned char cip3_lcport[];
extern unsigned char esp_dev_id[8]; 
extern unsigned char esp_user_data[14];
extern unsigned char at_send_len_ox[20];
extern code unsigned char temp_bander[];
extern code unsigned char CIPSEND_LEN[];
extern code unsigned char AT_CLDSEND[];
extern bit t_o;	
												 //  �ڹ���һ����00123 ������ʱ��ȥ��ǰ���00���123 ����������0����1
//��16�����������16�����ַ�����10���A
extern char back_char(unsigned char user_d);

////���� ���ֻ��������¸�ʽUDP����
////RPT:"0x00FE6738","0xB8B3C281","192.168.0.123","light","123456a"
extern void make_bander_data();


extern void make_AT_SEND_LEN(unsigned char a_len,unsigned char DATA_S);   //�����ұ�������ָ�������a_len �ĳ�10���ƣ��ұ�����ָ��  "AT+CIPSEND=XX" XX�Ƿ��͵�����

extern void at_uart_send_str(unsigned char *str);//����AT�ַ���������

extern void at_uart_send_buf(unsigned char *str,unsigned char len);//�������ݻ������ķ��ַ�����Ϣ����������Ϣ������


extern void init_uart(void);

extern unsigned char	TX1_Cnt;	//���ͼ���
extern unsigned char	RX1_Cnt;	//���ռ���
extern bit	B_TX1_Busy;	//����æ��־

#define	RX1_Lenth		32			//���ڽ��ջ��峤��
#define	BaudRate1		115200UL	//ѡ������
#define	Timer1_Reload	(65536UL -(MAIN_Fosc / 4 / BaudRate1))		//Timer 1 ��װֵ�� ��Ӧ300KHZ
#define	Timer2_Reload	(65536UL -(MAIN_Fosc / 4 / BaudRate1))		//Timer 2 ��װֵ�� ��Ӧ300KHZ
#define LOC_data 0
#define RMT_data 1
#define MAIN_Fosc		22118400L	//������ʱ��


