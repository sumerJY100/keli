#include"UART.H"
#include"STC15FXXXX.H"

/********************* �ַ������˺�����ʼ����************************/
/*
bit a_vec=0;		          //����ָ�����ִ�������̬���˵�ָ�����ַ��� ��1
unsigned char ceng=0;         //��� �����ֽ��� �����ָ���ִ�,��̬������
unsigned char str_len_limt=12;//����һ���޶��ο���ֵ��������ο������ڱض�����ָ���ַ���
unsigned char str_len_num=5;  //�ַ�����
char str_ref=':';
code char test_code[]="+IPD,";
*/

void fit_stb(unsigned char *str_p) // ÿ�δ��ڽ��յ�һ���ַ�������뱾���˺��������Ƿ���˵�ָ�����ַ�����
{
   if((a_vec==0)&&(ceng<str_len_limt))
   {
      if(ceng<str_len_num)
	  {
      if(SBUF==(*(str_p+ceng)))
	  ceng++; 
	  else 
	  ceng=0;
	  }
	  else
	  {
		ceng++;
		if(SBUF==str_ref)
		{
		  a_vec=1;
		  ceng=0;	
		}
	  }
	  RX1_Cnt=0;
   }
   else if(ceng>=str_len_limt)
   {
     	  a_vec=0;
		  ceng=0;
   }
}

void UART1_int (void) interrupt UART1_VECTOR
{
	if(RI)
	{
		RI = 0;
		if(a_vec==0)//�ϴ�δ�����˵�ָ�����ַ��������е��ַ����֣�ÿ��ֻ����һ���ַ�
		{
		 switch(uart_rec_sta)	      //����ϵͳ��״̬�����������ĸ��ַ�	 
		 {
		  case uart_rmt_tcp_udp_data: //���ڽ���������UDP TCP �����շ�
		  fit_stb(PCLD_code);		  //�������ݽ���ͷ
		  break;
		  case  uart_rec_csysid:	  //���ڽ�����ģ���ڲ�ID ��KEY ״̬
		  fit_stb(CYSYS_code);	      //����ָ����ͷ��
		  break;
		  case uart_rec_smartlink:	  //���ڽ����ÿͻ���·�����˻�����״̬
		  fit_stb(smartlink_code);
		  break;
		  case uart_rec_bander:
		  fit_stb(bander_code);
		 }
		}
		else//////////////////////////////���˵�ָ����ͷ��
		{
		
		 recd_buf[RX1_Cnt] = SBUF;		//����һ���ֽ�
		 if(RX1_Cnt<62)	/////////////////////��ֹ64�ֽڵĻ��������
		 RX1_Cnt++;
		 else			///////ÿ���յ���ָ���64�ֽڣ��Ͱ�������գ�����ָ��ָ��ͷ
		 {
		   RX1_Cnt=0;
		   a_vec=0;
		   two_lab=0;
		 }
		 switch(uart_rec_sta)
		  {
		    case  uart_rmt_tcp_udp_data:
				if(RX1_Cnt>=7)
				 {
				   a_vec=0;
				   rec_len=RX1_Cnt;
				   two_lab=0;
				   RX1_Cnt=0;
                   LED1=0;
				   LED2=0;
				   LED3=0;
				   //change_pack();
				   have_data=1;
				    if((recd_buf[5])==':')	      /////////////////////////////////////�򵥵õ��ֻ��Ŀ���ָ��////////////////////
					{
					 if((recd_buf[6])=='0')
					 {
					  DK1=0;
					  LED1=1;	  //�ص�ָ��
					  led_sta=0;
					  led_data=1;
					 }
					 else if((recd_buf[6])=='1')
					 {
					  DK1=1;
					  led_sta=1;
				      LED1=0;    //����ָ��
					  led_data=1;
					 }
					}
				 }
			break;
		   case  uart_rec_csysid:
		   if(RX1_Cnt>40)
		   have_id=1;
		   break;
		   case  uart_rec_smartlink:
		   //if(RX1_Cnt>10)
		   have_smartlink=1;
		   break;
		   case  uart_rec_bander:
		   if(RX1_Cnt>15)
		   have_bander=1;
		   break;
		 };
	  }
	}
	if(TI)
	{
		TI = 0;
		B_TX1_Busy = 0;		//�������æ��־
	}
}