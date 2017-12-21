/*
本测试程序在安信可科技50元 的大测试板上正确运行，所有安信可出的的模块从ESP-01到ESP-14均可以正确运行，与远程控制。
*/
#include"SYS.H"
#include"AT_PRC.H"
#include"delay.h"
void main(void)
{
	char tt,n,z=0;
	unsigned int h=0;
	BEEP=0;
	//////////////////////////////////////////////////////////////////////////////////////下面部分为定时器以及串口初始化/////////////////////
	init_uart();
	Delay2(1000);
	if(K1==0)           //开机的时候发现按键1也就是MCU_P1.3 被按下，那么清除单片机中EEPROM存的数据
	{

			LED2=0;
			BEEP=1;
			Delay2(200);
		    LED2=1;
			BEEP=0;      //闪亮心跳指示灯，和心跳音
			have_config=1;
	}

	///////////////////////////////////////////////////////////////////////////////////以上部分主要完成串口的初始化////////////////////////////
	for(;;)
	{	 
        a_vec=0;		          //出现指定的字串，并动态过滤到指定的字符后 置1
        ceng=0;                   //最多 多少字节内 会出现指定字串,静态变量，
		RX1_Cnt=0;
        str_len_limt=11;          //设置一个限定参考数值，在这个参考个数内必定出现指定字符串
        str_len_num=8;           //要过滤连续字符个数
        str_ref='I';			  // 过滤到字符串后，接着要出现的字符，这个字符出现的位置才是绝对0位置
		uart_rec_sta=uart_rec_smartlink; //设置在串口中的过滤分支条件，串口中断中，根据这个标志调用不同的字符串过滤参数
	//////////////////////////以上变量为字符串过滤参数，设置过滤条件///////////////////////////////////////////////////////////////////////////
		at_uart_send_str(AT_MODE);		 //设置模块进入STATION 模式
		Delay2(500);
		if(have_config)					 //需要配置 模块连入本地的路由器账户和密码
		{
		at_uart_send_str(AT_SMARTLINK);	 //发送进入SMARTLINK AT指令
		Delay2(500);
		}
		do								 //此循环完成SMARTLINK 的配置
		{
		LED1=0;
		LED2=0;
		LED3=0;
		Delay2(500);
		LED1=1;
		LED2=1;
		LED3=1;
		Delay2(500);
	    }while(have_smartlink==0); //此循环完成SMARTLINK 的配置
		//////////////////////////////////////////////////////////上面面的语句发送AT指令加入内网，并链接到安信可物联网服务器/////////////////////
		at_uart_send_str(AT_CLDSTART);
		a_vec=0;		          //出现指定的字串，并动态过滤到指定的字符后 置1
		RX1_Cnt=0;				  //
        ceng=0;                   //最多 多少字节内 会出现指定字串,静态变量，
        str_len_limt=12;          //设置一个限定参考数值，在这个参考个数内必定出现指定字符串
        str_len_num=5;            //字符个数
        str_ref=':';
		uart_rec_sta=uart_rmt_tcp_udp_data;                      //将构造好的AT发送数据到互联网的动态发送数据长度
		Delay2(1000);
		at_uart_send_str(AT_LANSTART);
		Delay2(500);
		while(1)
		{													 //主循环体中，每30秒构造一个上传数据，上传数据每次加1. 
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
						need_seed_len=8;              //构造上传数据到云，转给手机的温度数据包，符合基本数据格式
					    make_AT_SEND_LEN(need_seed_len,RMT_data); 					 //动态构造发送AT指令
						at_uart_send_str(at_send_len_ox);                //发送构造好的发送指令
					    Delay2(200);
					    at_uart_send_buf(switch_sta1,need_seed_len);	     //经过WIFI发送数据
					}
				}
				else
				{  	
						for(n=0;n<3;n++)
						{
						need_seed_len=8;              //构造上传数据到云，转给手机的温度数据包，符合基本数据格式
					    make_AT_SEND_LEN(need_seed_len,RMT_data); 					 //动态构造发送AT指令
						at_uart_send_str(at_send_len_ox);                //发送构造好的发送指令
					    Delay2(200);
					    at_uart_send_buf(switch_sta0,need_seed_len);	     //经过WIFI发送数据 
						}
				}
			}
			Delay2(100);
		    }
		    need_seed_len=9;                                 //构造上传数据到云，转给手机的温度数据包，符合基本数据格式
		    make_AT_SEND_LEN(need_seed_len,RMT_data); 		 //动态构造发送AT指令
			at_uart_send_str(at_send_len_ox);                //发送构造好的发送指令
		    Delay2(2000);
		    at_uart_send_buf(temp_cd,need_seed_len);	     //经过WIFI发送数据

			LED2=0;
			BEEP=1;
			Delay2(200);
		    LED2=1;
			BEEP=0;                                          //闪亮心跳指示灯，和心跳音
															 //每30秒会运行到这里一次。更新一次温度数值
			time++;
			temp_cd[5]=(((time%1000)/100)+'0');
			temp_cd[6]=(((time%100)/10)+'0');
		    temp_cd[7]=(((time%10))+'0');
	   }
	}
	Delay2(2000);
}

