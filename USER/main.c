#include "sys.h"
#include "delay.h"
#include "usart.h" 
#include "led.h" 		 	 
#include "lcd.h"  
#include "key.h"     
#include "usmart.h" 
#include "malloc.h"
#include "sdio_sdcard.h"  
#include "w25qxx.h"    
#include "ff.h"  
#include "exfuns.h"   
#include "text.h"	
#include "touch.h"		
#include "usart3.h"
#include "sim800c.h" 
#include "sr04.h"
#include "beep.h"
#include "string.h"
#include "w25qxx.h" 	
#include "touch.h" 	 
#include "timer.h"
 
 
/************************************************
 ALIENTEK STM32F103开发板 扩展实验17
 ATK-SIM800C GSM/GPRS模块测试实验  
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com 
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/



int main(void)
 {	 
	float length;
	u8 x=0;
	u8 *p;
	u8 safeDis = 100;//安全距离
	u8 warningDis = 50;//危险距离
	u8* ipaddr = "103.46.128.21";
	u8* port = "50082";
	const u8 *modetbl[2]={"TCP","UDP"};//连接模式
	u8 lcd_id[12];			//存放LCD ID字符串
	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(115200);	 	//串口初始化为115200
	usart3_init(115200);
 	LED_Init();			     //LED端口初始化
	LCD_Init();
	Hcsr04Init();
	BEEP_Init();
	printf("%d", (u16)length);
	LCD_Clear(WHITE);
	POINT_COLOR=RED;	
	sprintf((char*)lcd_id,"LCD ID:%04X",lcddev.id);//将LCD ID打印到lcd_id数组。				 	
	if(sim800c_send_cmd("AT","OK",1000)==0) 
	{
		LCD_Clear(RED);
		delay_ms(1000);
	}
	if(sim800c_send_cmd("AT+CGCLASS=\"B\"","OK",1000)==0)
	{
		LCD_Clear(BLUE);
		delay_ms(1000);
	}
	if(sim800c_send_cmd("AT+CGDCONT=1,\"IP\",\"CMNET\"","OK",1000)==0) 
	{
		LCD_Clear(WHITE);
		delay_ms(1000);
	}
	if(sim800c_send_cmd("AT+CGATT=1","OK",500)==0)
	{
		LCD_Clear(BLACK);
		delay_ms(1000);
	}
	if(sim800c_send_cmd("AT+CIPCSGP=1,\"CMNET\"","OK",500)==0)
	{
		LCD_Clear(GRAY);
		delay_ms(1000);
	}
	if(sim800c_send_cmd("AT+CLPORT=\"TCP\",\"2000\"","OK",500)==0)
	{
		LCD_Clear(RED);
		delay_ms(1000);
	}

	if(sim800c_send_cmd("AT+CIPSTART=\"TCP\",\"103.46.128.21\",\"50082\"", "OK", 1000)==0)
	{
		LCD_Clear(YELLOW);
		delay_ms(1000);
	}

  while(1) 
	{		 
		LCD_Clear(WHITE);
		LCD_ShowString(30,110,200,16,16,"length:");
		length = Hcsr04GetLength();
		LCD_ShowxNum(30,130,(u16)length, 16, 16, 1); 
		if(length > safeDis){
			BEEP = 0;
			LED0 = 0;
		}
		else if(length < safeDis && length > warningDis)
		{
			BEEP = 1;
			LED0 = !LED0;
			delay_ms(200);
			BEEP = 0;
		}
		else 
		{
			BEEP = 1;
			LED0 = !LED0;
			delay_ms(100);
			BEEP = 0;

//			if(sim800c_send_cmd("AT+CIPSEND",">",500)==0)		//发送数据
//			{ 
// 				printf("CIPSEND DATA:%d\r\n",(u8)length);	 			//发送数据打印到串口
//				u3_printf("%d\r\n",(u8)length);
//				delay_ms(10);
//				if(sim800c_send_cmd((u8*)0X1A,"SEND OK",1000)==0)Show_Str(30+30,80,200,12,"WARNING!",12,0);//最长等待10s
//				else Show_Str(30+30,80,200,12,"数据发送失败!",12,0);
//				delay_ms(1000); 
//			}else sim800c_send_cmd((u8*)0X1B,0,0);	//ESC,取消发送 else sim800c_send_cmd((u8*)0X1B,0,0);	//ESC,取消发送 
		}
		

//		sim800c_send_cmd("AT+CIPCLOSE=1","CLOSE OK",500);	//关闭连接
//		sim800c_send_cmd("AT+CIPSHUT","SHUT OK",500);		//关闭移动场景 

		     	
	  x++;
		if(x==12)x=0;
		//LED0=!LED0;				   		 
		//delay_ms(500);	

	} 
}


















