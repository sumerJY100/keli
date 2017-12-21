/*
�����Գ����ڰ��ſɿƼ�50Ԫ �Ĵ���԰�����ȷ���У����а��ſɳ��ĵ�ģ���ESP-01��ESP-14��������ȷ���У���Զ�̿��ơ�
*/
#include"SYS.H"
#include"AT_PRC.H"
#include"delay.h"
void main(void)
{
	char tt,n,z=0;
	unsigned int h=0;
	BEEP=0;
	//////////////////////////////////////////////////////////////////////////////////////���沿��Ϊ��ʱ���Լ����ڳ�ʼ��/////////////////////
	init_uart();
	Delay2(1000);
	if(K1==0)           //������ʱ���ְ���1Ҳ����MCU_P1.3 �����£���ô�����Ƭ����EEPROM�������
	{

			LED2=0;
			BEEP=1;
			Delay2(200);
		    LED2=1;
			BEEP=0;      //��������ָʾ�ƣ���������
			have_config=1;
	}

	///////////////////////////////////////////////////////////////////////////////////���ϲ�����Ҫ��ɴ��ڵĳ�ʼ��////////////////////////////
	for(;;)
	{	 
        a_vec=0;		          //����ָ�����ִ�������̬���˵�ָ�����ַ��� ��1
        ceng=0;                   //��� �����ֽ��� �����ָ���ִ�,��̬������
		RX1_Cnt=0;
        str_len_limt=11;          //����һ���޶��ο���ֵ��������ο������ڱض�����ָ���ַ���
        str_len_num=8;           //Ҫ���������ַ�����
        str_ref='I';			  // ���˵��ַ����󣬽���Ҫ���ֵ��ַ�������ַ����ֵ�λ�ò��Ǿ���0λ��
		uart_rec_sta=uart_rec_smartlink; //�����ڴ����еĹ��˷�֧�����������ж��У����������־���ò�ͬ���ַ������˲���
	//////////////////////////���ϱ���Ϊ�ַ������˲��������ù�������///////////////////////////////////////////////////////////////////////////
		at_uart_send_str(AT_MODE);		 //����ģ�����STATION ģʽ
		Delay2(500);
		if(have_config)					 //��Ҫ���� ģ�����뱾�ص�·�����˻�������
		{
		at_uart_send_str(AT_SMARTLINK);	 //���ͽ���SMARTLINK ATָ��
		Delay2(500);
		}
		do								 //��ѭ�����SMARTLINK ������
		{
		LED1=0;
		LED2=0;
		LED3=0;
		Delay2(500);
		LED1=1;
		LED2=1;
		LED3=1;
		Delay2(500);
	    }while(have_smartlink==0); //��ѭ�����SMARTLINK ������
		//////////////////////////////////////////////////////////���������䷢��ATָ����������������ӵ����ſ�������������/////////////////////
		at_uart_send_str(AT_CLDSTART);
		a_vec=0;		          //����ָ�����ִ�������̬���˵�ָ�����ַ��� ��1
		RX1_Cnt=0;				  //
        ceng=0;                   //��� �����ֽ��� �����ָ���ִ�,��̬������
        str_len_limt=12;          //����һ���޶��ο���ֵ��������ο������ڱض�����ָ���ַ���
        str_len_num=5;            //�ַ�����
        str_ref=':';
		uart_rec_sta=uart_rmt_tcp_udp_data;                      //������õ�AT�������ݵ��������Ķ�̬�������ݳ���
		Delay2(1000);
		at_uart_send_str(AT_LANSTART);
		Delay2(500);
		while(1)
		{													 //��ѭ�����У�ÿ30�빹��һ���ϴ����ݣ��ϴ�����ÿ�μ�1. 
			for(tt=0;tt<120;tt++)
			{
			Delay2(100);
			if(led_data)
			{
				led_data=0;
				if(led_sta)
				{
					for(n=0;n<3;n++)
					{
						need_seed_len=8;              //�����ϴ����ݵ��ƣ�ת���ֻ����¶����ݰ������ϻ������ݸ�ʽ
					    make_AT_SEND_LEN(need_seed_len,RMT_data); 					 //��̬���췢��ATָ��
						at_uart_send_str(at_send_len_ox);                //���͹���õķ���ָ��
					    Delay2(200);
					    at_uart_send_buf(switch_sta1,need_seed_len);	     //����WIFI��������
					}
				}
				else
				{  	
						for(n=0;n<3;n++)
						{
						need_seed_len=8;              //�����ϴ����ݵ��ƣ�ת���ֻ����¶����ݰ������ϻ������ݸ�ʽ
					    make_AT_SEND_LEN(need_seed_len,RMT_data); 					 //��̬���췢��ATָ��
						at_uart_send_str(at_send_len_ox);                //���͹���õķ���ָ��
					    Delay2(200);
					    at_uart_send_buf(switch_sta0,need_seed_len);	     //����WIFI�������� 
						}
				}
			}
			Delay2(100);
		    }
		    need_seed_len=9;                                 //�����ϴ����ݵ��ƣ�ת���ֻ����¶����ݰ������ϻ������ݸ�ʽ
		    make_AT_SEND_LEN(need_seed_len,RMT_data); 		 //��̬���췢��ATָ��
			at_uart_send_str(at_send_len_ox);                //���͹���õķ���ָ��
		    Delay2(2000);
		    at_uart_send_buf(temp_cd,need_seed_len);	     //����WIFI��������

			LED2=0;
			BEEP=1;
			Delay2(200);
		    LED2=1;
			BEEP=0;                                          //��������ָʾ�ƣ���������
															 //ÿ30������е�����һ�Ρ�����һ���¶���ֵ
			time++;
			temp_cd[5]=(((time%1000)/100)+'0');
			temp_cd[6]=(((time%100)/10)+'0');
		    temp_cd[7]=(((time%10))+'0');
	   }
	}
	Delay2(2000);
}
