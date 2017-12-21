#include"UART.H"
#include"STC15FXXXX.H"

/********************* 字符串过滤函数初始参数************************/
/*
bit a_vec=0;		          //出现指定的字串，并动态过滤到指定的字符后 置1
unsigned char ceng=0;         //最多 多少字节内 会出现指定字串,静态变量，
unsigned char str_len_limt=12;//设置一个限定参考数值，在这个参考个数内必定出现指定字符串
unsigned char str_len_num=5;  //字符个数
char str_ref=':';
code char test_code[]="+IPD,";
*/

void fit_stb(unsigned char *str_p) // 每次串口接收到一个字符都会进入本过滤函数，看是否过滤到指定的字符串。
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
		if(a_vec==0)//上次未曾过滤到指定的字符串中所有的字符出现，每次只过滤一个字符
		{
		 switch(uart_rec_sta)	      //根据系统的状态，决定过滤哪个字符	 
		 {
		  case uart_rmt_tcp_udp_data: //串口进入正常的UDP TCP 数据收发
		  fit_stb(PCLD_code);		  //过滤数据接收头
		  break;
		  case  uart_rec_csysid:	  //串口进入获得模块内部ID 和KEY 状态
		  fit_stb(CYSYS_code);	      //过滤指定的头部
		  break;
		  case uart_rec_smartlink:	  //串口进入获得客户的路由器账户密码状态
		  fit_stb(smartlink_code);
		  break;
		  case uart_rec_bander:
		  fit_stb(bander_code);
		 }
		}
		else//////////////////////////////过滤到指定的头部
		{
		
		 recd_buf[RX1_Cnt] = SBUF;		//保存一个字节
		 if(RX1_Cnt<62)	/////////////////////防止64字节的缓冲区溢出
		 RX1_Cnt++;
		 else			///////每次收到的指令超过64字节，就把数据清空，接收指针指向开头
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
				    if((recd_buf[5])==':')	      /////////////////////////////////////简单得到手机的开关指令////////////////////
					{
					 if((recd_buf[6])=='0')
					 {
					  DK1=0;
					  LED1=1;	  //关灯指令
					  led_sta=0;
					  led_data=1;
					 }
					 else if((recd_buf[6])=='1')
					 {
					  DK1=1;
					  led_sta=1;
				      LED1=0;    //开灯指令
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
		B_TX1_Busy = 0;		//清除发送忙标志
	}
}