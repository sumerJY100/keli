
#define MAIN_Fosc		22118400L	//������ʱ��
#include"STC15FXXXX.H"
#define uart_rec_tcp_udp_data 0	//ϵͳ�����������������շ�״̬
#define uart_rec_csysid       1	//ϵͳ��ȡESP8266WIFIģ������ɺ�״̬����ȡ�ĸ��ֽڵ�оƬID �� 4���ֽڵ���Ȩ��
#define uart_rec_smartlink    2	//ϵͳ�����ȡ�����˺�����״̬
#define uart_rec_bander       3	//ϵͳ���뽫��WIFI�豸 ��APP �ֻ��󶨿���״̬
typedef 	unsigned char	u8;
typedef 	unsigned int	u16;
typedef 	unsigned long	u32;

extern bit have_tok;
extern bit have_data;
extern unsigned char rec_len;
extern bit                   a_vec;		   //����һ����־λ�����ڳ��� ����ָ�����ִ�������̬���˵�ָ�����ַ��� ��1
extern unsigned char          ceng;         //��� �����ֽ��� �����ָ���ִ�,��̬������
extern unsigned char str_len_limt;         //����һ���޶��ο���ֵ��������ο������ڱض�����ָ���ַ���
extern unsigned char   str_len_num;        //�ַ�����
extern char str_ref;
extern unsigned char uart_rec_sta;

extern code char CYSYS_code[];
extern code char PIPD_code[];
extern code char PCLD_code[];

extern code char bander_code[];//���������©��
extern code char smartlink_code[];

extern u8	TX1_Cnt;	//���ͼ���
extern u8	RX1_Cnt;	//���ռ���
extern bit	B_TX1_Busy;	//����æ��־

extern xdata unsigned char send_buf[64];	                     //���ͻ�����
extern idata unsigned char recd_buf[64];						 //���ջ�����
extern data unsigned char temp_buf[72];
extern unsigned char two_lab;
extern bit unlink;
extern unsigned char count_relink;
sbit LED1=P1^0;//LED1
sbit LED2=P1^1;//LED2
sbit LED3=P3^7;//LED3

sbit DK1=P3^3;//�̵���
sbit BEEP=P3^4;//������
sbit K1=P1^3;//����1
sbit K2=P1^2;//����2
sbit K3=P1^4;//����3
bit led_sta=0;
bit led_data=0;
extern void change_pack();					  //�ѽ��յ������ݰ�ת�������0X55 ת���0X54 0X01 0X54 �滻��0X54 02

extern idata unsigned char toke[8];//���豸�ڷ������ϻ�õ����ư�
extern bit have_id;					  //��¼�Ƿ���ATָ���ȡ�������ģ���ID ��KEY��Ϣ
extern bit have_smartlink;			  //��¼�Ƿ���˵� �Ӵ�������TCP UDP ����  smartlink 
extern bit have_bander;				  //��¼�ǴӴ�������������������й��˵� �ֻ���UDP������������Ϣ

/////���ǰ��ſ���ƽ̨���ݰ���ȫ����ɽṹ��ϸ�ڿɲ��հ��ſɵ���ƽ̨V1.0�汾����飬���ṹ�½�
#define O_PF    0X00  //��ͷ1�ֽڹ̶�0X55 
#define O_LEN_L 0X01  //�����������ֽ�
#define O_LEN_H 0X02  //�����������ֽ� ע��ת���� ��������ֻ����һ�����ݴ�����
#define O_CMD_T 0X03  //��������
#define O_CMD_C 0X04  //��������
#define O_CIX_L 0X05  //���������б�ŵ��ֽ�
#define O_CIX_H 0X06  //���������б�Ÿ��ֽ�
#define O_EXMSH 0X07  //��չ��Ϣ���ֽ�
#define O_EXMSL 0X08  //��չ��Ϣ���ֽ�
#define O_RESTA 0X09  //���ݰ�״̬��Ϣ���ɹ� ʧ�� ���� δ֪
#define O_DEVID_START 0x0A  //8�ֽ��豸ʶ��  ���ֽ���ǰ
#define O_DEVID 0x0A  //8�ֽ��豸ʶ��
#define O_TK_LEN      0X12 //1 BYTS TL_LEN	  //��õ��豸��ʱͨѶ���Ƴ���
#define O_TOKE_START  0X13
#define O_DATAS_START 0X1B

#define uart_rec_tcp_udp_data 0	//ϵͳ�����������������շ�״̬
#define uart_rec_csysid       1	//ϵͳ��ȡESP8266WIFIģ������ɺ�״̬����ȡ�ĸ��ֽڵ�оƬID �� 4���ֽڵ���Ȩ��
#define uart_rec_smartlink    2	//ϵͳ�����ȡ�����˺�����״̬
#define uart_rec_bander       3	//ϵͳ���뽫��WIFI�豸 ��APP �ֻ��󶨿���״̬
#define uart_rmt_tcp_udp_data 4
//N BYTS DATAS	  //�ͻ������ݰ���
//CRC_L    1BYTE	  //CRC16 ���ֽ�
//CRC_H    1BYTE	  //CRC16 ���ֽ�
//PACK_END 1BYTE	  //��β1�ֽڹ̶�0X55

////////////////////////////////////////////////////////////////////////���϶����˰��еĸ������ľ���λ��//////////////////////////////////
/////////////////////////////////////////////////////////////////////�����Ǵ�WIFIģ�鴮�����������Ƭ����������󣬱���Ƭ��Ҫ���˵ĸ���ͷ��////////////////////