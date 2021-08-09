#include "show.h"
#include "oledfont.h"
#include "oled.h"
#include "stick.h"
#include "key.h"


#define Line1_Begin 29
#define Line2_Begin 5
#define Line3_Begin 5
#define Line4_Begin 30
#define Line5_Begin 0

#define X_Begin 0
#define Y_Begin 47
#define Z_Begin 95

#define Line1_Begin1 0
#define Line2_Begin1 0
#define Line3_Begin1 40
#define Line4_Begin1 0
#define Line5_Begin1 0

#define Y0 0
#define Y1 14
#define Y2 Y1+12
#define Y3 Y2+12
#define Y4 Y3+12
#define Y5 Y4+12

Show_t Show;

extern int key_page;
/**************************************************************************
函数功能：OLED显示
**************************************************************************/
void oled_show(void)
{
	u8 temp,cnt = 0,i;
	u16 Att_temp;
	u32 GPS_temp;
	float dd,mm,size;
	static u8 page,page_temp ;
	
	page = key_page;
	if(page != page_temp)//切换页面先清屏
	{
		page_temp = page;
		OLED_Clear();
	}

	switch(page)
	{
		case 0:
			OLED_Show_CH(Line1_Begin+24,Y0,0,12,1);
			OLED_Show_CH(Line1_Begin+36,Y0,1,12,1);
			OLED_Show_CH(Line1_Begin+48,Y0,2,12,1);

			OLED_ShowString(Line2_Begin+00,Y1,"R-V:",12,1);OLED_ShowString(Line2_Begin+36,Y1,".",12,1);
			OLED_ShowNumber(Line2_Begin+30,Y1,Show.Battery_Rc/100,1,12);OLED_ShowNumber(Line2_Begin+42,Y1,Show.Battery_Rc % 100,2,12);
			
			OLED_ShowString(Line2_Begin+64,Y1,"F-V:",12,1);OLED_ShowString(Line2_Begin+100,Y1,".",12,1);
			OLED_ShowNumber(Line2_Begin+94,Y1,Show.Battery_Fly/100,1,12);OLED_ShowNumber(Line2_Begin+106,Y1,Show.Battery_Fly%100,2,12);
			
			//显示遥控数据
			OLED_ShowString(Line3_Begin+00,Y2,"THR:",12,1);
			temp = (RC_Scan.THR-1000)/41;
			OLED_Show_progress_bar(temp,12,24,Line4_Begin+6 ,Y2,12,1);
			temp = (RC_Scan.THR-1500)/41;
			OLED_Show_progress_bar(temp,12,12,Line4_Begin+18,Y2,12,1);
			
			OLED_ShowString(Line3_Begin+64,Y2,"ROL:",12,1);
			temp = (RC_Scan.ROL-1000)/41;
			OLED_Show_progress_bar(temp,12,24,Line4_Begin+70,Y2,12,1);
			temp = (RC_Scan.ROL-1500)/41;
			OLED_Show_progress_bar(temp,12,12,Line4_Begin+82,Y2,12,1);
			
			OLED_ShowString(Line3_Begin+00,Y3,"YAW:",12,1);
			temp = (RC_Scan.YAW-1000)/41;
			OLED_Show_progress_bar(temp,12,24,Line4_Begin+6 ,Y3,12,1);
			temp = (RC_Scan.YAW-1500)/41;
			OLED_Show_progress_bar(temp,12,12,Line4_Begin+18,Y3,12,1);
			
			OLED_ShowString(Line3_Begin+64,Y3,"PIT:",12,1);
			temp = (RC_Scan.PIT-1000)/41;
			OLED_Show_progress_bar(temp,12,24,Line4_Begin+70,Y3,12,1);
			temp = (RC_Scan.PIT-1500)/41;
			OLED_Show_progress_bar(temp,12,12,Line4_Begin+82,Y3,12,1);
			
			OLED_ShowString(Line5_Begin+00,Y4,"L:",12,1);
			temp = (RC_Scan.AUX1-1000)/41;
			OLED_Show_progress_bar(temp,12,24,Line5_Begin+15 ,Y4,12,1);
			temp = (RC_Scan.AUX1-1500)/41;
			OLED_Show_progress_bar(temp,12,12,Line5_Begin+27,Y4,12,1);
				
			OLED_ShowString(Line5_Begin+44,Y4,"M:",12,1);
			temp = (RC_Scan.AUX2-1000)/41;
			OLED_Show_progress_bar(temp,12,24,Line5_Begin+59 ,Y4,12,1);
			temp = (RC_Scan.AUX2-1500)/41;
			OLED_Show_progress_bar(temp,12,12,Line5_Begin+71,Y4,12,1);
			
			OLED_ShowString(Line5_Begin+88,Y4,"R:",12,1);
			temp = (RC_Scan.AUX3-1000)/41;
			OLED_Show_progress_bar(temp,12,24,Line5_Begin+103 ,Y4,12,1);
			temp = (RC_Scan.AUX3-1500)/41;
			OLED_Show_progress_bar(temp,12,12,Line5_Begin+115,Y4,12,1);
			break;
			
		//机体状态	
		case 1:
			for(i=0;i<8;i++) OLED_Show_CH_String(40+i*6,cnt,oled_CH0[i],12,1);
		
			for(i=0;i<8;i++) OLED_Show_CH_String(i*6,Y1,oled_CH1[i],12,1);
			if(1) for(i=0;i<4;i++) OLED_Show_CH_String(80+i*6,Y1,oled_CH2[i],12,1);
			else  for(i=0;i<4;i++) OLED_Show_CH_String(80+i*6,Y1,oled_CH3[i],12,1);
			OLED_ShowString(Line5_Begin,Y2,"ROLL:",12,1);
			Att_temp = 17956;
			dd = Att_temp/100;
			mm = Att_temp % 100;
			size = sizeof(dd);
			OLED_ShowString(Line2_Begin+(size+5)*6,Y2,".",12,1);
			OLED_ShowNumber(Line5_Begin+37,Y2,dd,3,12);
			OLED_ShowNumber(Line5_Begin+62,Y2,mm,2,12);
		
			OLED_ShowString(Line5_Begin,Y3,"PIT:",12,1);
			Att_temp = 17956;
			dd = Att_temp/100;
			mm = Att_temp % 100;
			size = sizeof(dd);
			OLED_ShowString(Line2_Begin+(size+5)*6,Y3,".",12,1);
			OLED_ShowNumber(Line5_Begin+37,Y3,dd,3,12);
			OLED_ShowNumber(Line5_Begin+62,Y3,mm,2,12);
		
			OLED_ShowString(Line5_Begin,Y4,"YAW:",12,1);
			Att_temp = 17956;
			dd = Att_temp/100;
			mm = Att_temp % 100;
			size = sizeof(dd);
			OLED_ShowString(Line2_Begin+(size+5)*6,Y4,".",12,1);
			OLED_ShowNumber(Line5_Begin+37,Y4,dd,3,12);
			OLED_ShowNumber(Line5_Begin+62,Y4,mm,2,12);
		break;
		
		//GPS数据
		case 2:
			for(i=0;i<7;i++) OLED_Show_CH_String(40+i*6,cnt,oled_CH4[i],12,1);
			
			for(i=0;i<12;i++) OLED_Show_CH_String(i*6,Y1,oled_CH5[i],12,1);
			if(1) for(i=0;i<4;i++) OLED_Show_CH_String(80+i*6,Y1,oled_CH2[i],12,1);
			else  for(i=0;i<4;i++) OLED_Show_CH_String(80+i*6,Y1,oled_CH3[i],12,1);
			
			OLED_ShowString(Line5_Begin,Y2,"LON:",12,1);
			GPS_temp = 3012321;
			dd = GPS_temp/100000;
			mm = GPS_temp % 100000;
			size = sizeof(dd);
			OLED_ShowString(Line5_Begin+(size+5)*6,Y2,".",12,1);
			OLED_ShowNumber(Line5_Begin+37,Y2,dd,2,12);
			OLED_ShowNumber(Line5_Begin+56,Y2,mm,5,12);
		
			OLED_ShowString(Line5_Begin,Y3,"LAT:",12,1);
			GPS_temp = 12012321;
			dd = GPS_temp/100000;
			mm = GPS_temp % 100000;
			size = sizeof(dd);
			OLED_ShowString(Line5_Begin+(size+5)*6,Y3,".",12,1);
			OLED_ShowNumber(Line5_Begin+37,Y3,dd,3,12);
			OLED_ShowNumber(Line5_Begin+56,Y3,mm,5,12);
			
			OLED_ShowString(Line5_Begin,Y4,"Star_Num:",12,1);
			GPS_temp = 3;
			size = sizeof(GPS_temp);
			OLED_ShowNumber(Line5_Begin+70,Y4,GPS_temp,2,12);
		break;
	}

	
	OLED_Refresh_Gram();//开始显示
}

//进度条显示函数
void OLED_Show_progress_bar(u8 temp,u8 chr_star,u8 chr_default,u8 x,u8 y,u8 size,u8 mode)
{
	switch(temp)
	{
		case  0:OLED_Show_CH(x,y,chr_star+temp,size,size);break;
		case  1:OLED_Show_CH(x,y,chr_star+temp,size,size);break;
		case  2:OLED_Show_CH(x,y,chr_star+temp,size,size);break;
		case  3:OLED_Show_CH(x,y,chr_star+temp,size,size);break;
		case  4:OLED_Show_CH(x,y,chr_star+temp,size,size);break;
		case  5:OLED_Show_CH(x,y,chr_star+temp,size,size);break;
		case  6:OLED_Show_CH(x,y,chr_star+temp,size,size);break;
		case  7:OLED_Show_CH(x,y,chr_star+temp,size,size);break;
		case  8:OLED_Show_CH(x,y,chr_star+temp,size,size);break;
		case  9:OLED_Show_CH(x,y,chr_star+temp,size,size);break;
		case 10:OLED_Show_CH(x,y,chr_star+temp,size,size);break;
		case 11:OLED_Show_CH(x,y,chr_star+temp,size,size);break;
		case 12:OLED_Show_CH(x,y,chr_star+temp,size,size);break;
		
		default:OLED_Show_CH(x,y,chr_default,size,size);break;
	}
}

void Show_Duty(void)
{
	static u8 temp;
	
	if(Show.windows!=temp)
	{
		temp = Show.windows;
		OLED_Clear();
	}
	switch(0)//Show.windows
	{
		case 0:	oled_show();	break;
//		case 1:	OLED_Show_Seting();	break;
//		case 2:	OLED_Show_Test();	break;
//		default:break;
	}
}
