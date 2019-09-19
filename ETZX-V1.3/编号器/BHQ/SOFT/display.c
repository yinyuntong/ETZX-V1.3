
#define _IN_DISPLAY_
#include <config.h>



uchar xdata Display_data[64];




void OpenLCDBLK(void)
{
   LCD_BLK=0;
}


void CloseLCDBLK(void)
{
   LCD_BLK=1;
}

void LCD_WRITE_DATA(uchar write_data,uchar R_W)
{    
   if(R_W==0)    //写命令
    {
      LCD_A0=0;
	}
    else		//写数据
	{
	  LCD_A0=1;
	}
    LCD_DATA = write_data;
	/*LCD_WR  = 0;
	Delay_1us(1);
	LCD_RD  = 1;
	Delay_1us(1);
	LCD_RD  = 0;
    Delay_1us(1);*/
	LCD_RD  = 1;
	Delay_1us(1);
	LCD_WR  = 0;
	Delay_1us(1);
	LCD_WR  = 1;
    Delay_1us(1);
}
void LCD_page(uchar page)
{
    LCD_WRITE_DATA((page+0xB0),0);
}


void InitLCD(void)
{
   LCD_A0  = 0;
   LCD_RD  = 1;
   LCD_WR  = 1;
   LCD_RST = 0;
   LCD_BLK = 0;
   Delay_1ms(1);
   LCD_RST = 1;
   LCD_WRITE_DATA(0xE2,0);
   LCD_WRITE_DATA(0xAE,0);
   LCD_WRITE_DATA(0xA3,0);
   LCD_WRITE_DATA(0xC8,0);
   LCD_WRITE_DATA(0xA0,0);
   LCD_WRITE_DATA(0xA6,0); 
   LCD_WRITE_DATA(0x2C,0);
   LCD_WRITE_DATA(0x2F,0);
   LCD_WRITE_DATA(0x81,0);
   LCD_WRITE_DATA(0x18,0);
   LCD_WRITE_DATA(0x00,0);
   LCD_WRITE_DATA(0x20,0);
   LCD_WRITE_DATA(0xAF,0);
   DisplayWelcome();
} 
void  read_code_number(uchar code_data,uchar positive)
{
    uchar  i,*n_buf;
    switch(code_data)
     {
      case 0:   n_buf=number_0;  break;
	  case 1:   n_buf=number_1;  break;
	  case 2:   n_buf=number_2;  break;
	  case 3:   n_buf=number_3;  break;
	  case 4:   n_buf=number_4;  break;
	  case 5:   n_buf=number_5;  break;
	  case 6:   n_buf=number_6;  break;
	  case 7:   n_buf=number_7;  break;
	  case 8:   n_buf=number_8;  break;
	  case 9:   n_buf=number_9;  break;
	  case 10:  n_buf=number_10; break;
	  case 11:  n_buf=number_11; break;
	  case 12:  n_buf=number_12; break;
	  case 13:  n_buf=number_13; break;
     }  
    if(positive==0)
	   {for(i=0;i<16;i++){Display_data[i]=*n_buf;n_buf++;};}
	else{for(i=0;i<16;i++){Display_data[i]=~*n_buf;n_buf++;};}
}
void read_code_chinese(uchar code_data,uchar positive)
{
    uchar  i,*c_buf,j;
    switch(code_data)
	{ 
	  case 1:   c_buf=chiness_1;  break;
	  case 2:   c_buf=chiness_2;  break;
	  case 3:   c_buf=chiness_3;  break;
	  case 4:   c_buf=chiness_4;  break;
	  case 5:   c_buf=chiness_5;  break;
	  case 6:   c_buf=chiness_6;  break;
	  case 7:   c_buf=chiness_7;  break;
	  case 8:   c_buf=chiness_8;  break;
	  case 9:   c_buf=chiness_9;  break;
	  case 10:  c_buf=chiness_10; break;
	  case 11:  c_buf=chiness_11; break;
	  case 12:  c_buf=chiness_12; break;
	  case 13:  c_buf=chiness_13; break;
	  case 14:  c_buf=chiness_14; break;
	  case 15:  c_buf=chiness_15; break;
	  case 16:  c_buf=chiness_16; break;
	  case 17:  c_buf=chiness_17; break;
	  case 18:  c_buf=chiness_18; break;
	  case 19:  c_buf=chiness_19; break;
	  case 20:  c_buf=chiness_20; break;
	  case 21:  c_buf=chiness_21; break;
	  case 22:  c_buf=chiness_22; break;
	  case 23:  c_buf=chiness_23; break;
	  case 24:  c_buf=chiness_24; break;
	  case 25:  c_buf=chiness_25; break;
	  case 26:  c_buf=chiness_26; break;
	  case 27:  c_buf=chiness_27; break;
	  case 28:  c_buf=chiness_28; break;
	  case 29:  c_buf=chiness_29; break;
	  case 30:  c_buf=chiness_30; break;
	  case 31:  c_buf=chiness_31; break;
	  case 32:  c_buf=chiness_32; break;
	  case 33:  c_buf=chiness_33; break;
	  case 34:  c_buf=chiness_34; break;
	  case 35:  c_buf=chiness_35; break;
	  case 36:  c_buf=chiness_36; break;
	  case 37:  c_buf=chiness_37; break;
	  case 38:  c_buf=chiness_38; break;
	  case 39:  c_buf=chiness_39; break;
	  case 40:  c_buf=chiness_40; break;
      case 41:  c_buf=chiness_41; break;
	  case 42:  c_buf=chiness_42; break;
	  case 43:  c_buf=chiness_43; break;
	  case 44:  c_buf=chiness_44; break;
	  case 45:  c_buf=chiness_45; break;
      case 46:  c_buf=chiness_46; break;
	  case 47:  c_buf=chiness_47; break;
	  case 48:  c_buf=chiness_48; break;
	  case 49:  c_buf=chiness_49; break;
	  case 50:  c_buf=chiness_50; break;
	  case 51:  c_buf=chiness_51; break;
   	  case 52:  c_buf=chiness_52; break;
	  case 53:  c_buf=chiness_53; break;
	  case 54:  c_buf=chiness_54; break;
	  case 55:  c_buf=chiness_55; break;
	  case 56:  c_buf=chiness_56; break;
	  case 57:  c_buf=chiness_57; break;
	  case 58:  c_buf=chiness_58; break;
	  case 59:  c_buf=chiness_59; break;
	  case 60:  c_buf=chiness_60; break;
	  case 61:  c_buf=chiness_61; break;
	  case 62:  c_buf=chiness_62; break;
	  case 63:  c_buf=chiness_63; break;
      case 64:  c_buf=chiness_64; break;
	  case 65:  c_buf=chiness_65; break;
	  case 66:  c_buf=chiness_66; break;
	  case 67:  c_buf=chiness_67; break;
	  case 68:  c_buf=chiness_68; break;
	  case 69:	c_buf=chiness_69; break;
	  case 70:	c_buf=chiness_70; break;
	  case 71:	c_buf=chiness_71; break;
	  case 72:	c_buf=chiness_72; break;	  
	 }
   if(positive==0)
      {for(i=0;i<32;i++){ j=*c_buf,Display_data[i]=j ;c_buf++;};}
   else{for(i=0;i<32;i++){j=*c_buf;Display_data[i]=~j;c_buf++;};}  
}
////////////////////////////////////////////////////////////////////////////////////////////////
//程序名：  show_number（）
//用途：    在指定的位置显示一个字符
//入口参数：page = 开始页，y = 开始列，number = 要显示的字符，positive = 1反显，positive=0正显
//返回参数：无
////////////////////////////////////////////////////////////////////////////////////////////////
void show_number(uchar page,uchar y,uchar number,uchar positive)
{
   uchar i,buf;
   buf = number;
   if(buf<0x30)
     { read_code_number(number,positive);}
   if(buf==0x30)
     { for(i=0;i<16;i++){Display_data[i]=0;};}
   if(buf==0x31)
     { for(i=0;i<16;i++){Display_data[i]=0xff;};}
   LCD_WRITE_DATA(0x40,0);
   LCD_page(page);
   buf= y;
   y= y>>4;
   y= y&0x0F;
   y= y+0x10;
   LCD_WRITE_DATA(y,0);
   y= buf;
   y= y&0x0F;
   LCD_WRITE_DATA(y,0);
   for(i=0;i<8;i++)
      {LCD_WRITE_DATA(Display_data[i],1);}
   LCD_WRITE_DATA(0x40,0);
   LCD_page(page+1);
   y= buf;
   y= y>>4;
   y= y&0x0F;
   y= y+0x10;
   LCD_WRITE_DATA(y,0);
   y= buf;
   y= y&0x0F;
   LCD_WRITE_DATA(y,0);
   for(i=8;i<16;i++)
      {LCD_WRITE_DATA(Display_data[i],1);}      
}
void show_chinese(uchar page,uchar y,uchar number,uchar positive)
{  
   uchar i,buf;
   buf = number;
   if((number!=0x00)&&(number!=0xff))
     { read_code_chinese(buf,positive);}
   if(number==0x00)
     { for(i=0;i<32;i++){Display_data[i]=0;};}
   if(number==0xff)
     { for(i=0;i<32;i++){Display_data[i]=0xff;};}
   LCD_WRITE_DATA(0x40,0);
   LCD_page(page);
   buf= y;
   y= y>>4;
   y= y&0x0F;
   y= y+0x10;
   LCD_WRITE_DATA(y,0);
   y= buf;
   y= y&0x0F;
   LCD_WRITE_DATA(y,0);
   for(i=0;i<16;i++)
      {LCD_WRITE_DATA(Display_data[i],1);}
   LCD_WRITE_DATA(0x40,0);
   LCD_page(page+1);
   y= buf;
   y= y>>4;
   y= y&0x0F;
   y= y+0x10;
   LCD_WRITE_DATA(y,0);
   y= buf;
   y= y&0x0F;
   LCD_WRITE_DATA(y,0);
   for(i=16;i<32;i++)
      {LCD_WRITE_DATA(Display_data[i],1);}      
}
void clear_screen(void)
{
  uchar i;
  for(i=0;i<8;i++)show_chinese(0,(i*16),0x00,0);
  for(i=0;i<8;i++)show_chinese(2,(i*16),0x00,0);
}
void clear_screen_line(uchar page,uchar positive)
{
  uchar i,j;
  if(positive==0)j=0x00;
  else j=0xFF;
  for(i=0;i<8;i++)show_chinese(page,(i*16),j,positive);
}
void DisplayWelcome(void)	 //欢迎使用！
{
   clear_screen();
   show_chinese(1,24,9,0);
   show_chinese(1,40,10,0);
   show_chinese(1,56,11,0);
   show_chinese(1,72,12,0);
   show_chinese(1,88,13,0);
}
void DisplayMainMenu1(void)	 //1:录入指纹
{
   clear_screen();
   show_number(1,24,1,1);
   show_number(1,32,10,1);
   show_chinese(1,40,17,1);
   show_chinese(1,56,18,1);
   show_chinese(1,72,6,1);
   show_chinese(1,88,7,1);
}
void DisplayMainMenu2(void) //2：密码修改
{
   clear_screen();
   show_number(1,24,2,1);
   show_number(1,32,10,1);
   show_chinese(1,40,15,1);
   show_chinese(1,56,16,1);
   show_chinese(1,72,23,1);
   show_chinese(1,88,24,1);
}
void DisplayMainMenu3(void)//3:密码验证
{
   clear_screen();
   show_number(1,24,3,1);
   show_number(1,32,10,1);
   show_chinese(1,40,15,1);
   show_chinese(1,56,16,1);
   show_chinese(1,72,19,1);
   show_chinese(1,88,20,1);
}
void DisplayMainMenu4(void)//4:取消密码验证
{
   clear_screen();
   show_number(1,8,4,1);
   show_number(1,16,10,1);
   show_chinese(1,24,35,1);
   show_chinese(1,40,36,1);
   show_chinese(1,56,15,1);
   show_chinese(1,72,16,1);
   show_chinese(1,88,19,1);
   show_chinese(1,104,20,1);
}
void DisplayMainMenu5(void)//5：指纹恢复密码
{
   clear_screen();
   show_number(1,8,5,1);
   show_number(1,16,10,1);
   show_chinese(1,24,6,1);
   show_chinese(1,40,7,1);
   show_chinese(1,56,31,1);
   show_chinese(1,72,32,1);
   show_chinese(1,88,15,1);
   show_chinese(1,104,16,1);
}
void DisplayMainMenu6(void)//6：密码清除指纹库
{
   clear_screen();
   show_number(1,0,6,1);
   show_number(1,8,10,1);
   show_chinese(1,16,15,1);
   show_chinese(1,32,16,1);
   show_chinese(1,48,29,1);
   show_chinese(1,64,40,1);
   show_chinese(1,80,6,1);
   show_chinese(1,96,7,1);
   show_chinese(1,112,27,1);
}
void DisplayMainMenu7(void)//7：设置报警手机号
{
   clear_screen();
   show_number(1,0,7,1);
   show_number(1,8,10,1);
   show_chinese(1,16,52,1);
   show_chinese(1,32,53,1);
   show_chinese(1,48,54,1);
   show_chinese(1,64,55,1);
   show_chinese(1,80,56,1);
   show_chinese(1,96,57,1);
   show_chinese(1,112,58,1);
}
void DisplayCallNumberMenu(uchar a,uchar b)//1:设置手机号一  2:设置手机号二
{
   clear_screen();
   show_number(0,8,1,a);
   show_number(0,16,10,a);
   show_chinese(0,24,52,a);
   show_chinese(0,40,53,a);
   show_chinese(0,56,56,a);
   show_chinese(0,72,57,a);
   show_chinese(0,88,58,a);
   show_chinese(0,104,66,a);
   show_number(2,8,2,b);
   show_number(2,16,10,b);
   show_chinese(2,24,52,b);
   show_chinese(2,40,53,b);
   show_chinese(2,56,56,b);
   show_chinese(2,72,57,b);
   show_chinese(2,88,58,b);
   show_chinese(2,104,67,b);
}
void DisplayFinger_HaveEmpty(void)//指纹库已清空!
{
   clear_screen();
   show_chinese(1,8,6,0);
   show_chinese(1,24,7,0);
   show_chinese(1,40,27,0);
   show_chinese(1,56,28,0);
   show_chinese(1,72,29,0);
   show_chinese(1,88,30,0);
   show_chinese(1,104,13,0);
}
void DisplayEnter_OldPassword(void)//请输入原密码：
{
   clear_screen();
   show_chinese(0,12,1,0);
   show_chinese(0,28,4,0);
   show_chinese(0,44,5,0);
   show_chinese(0,60,68,0);
   show_chinese(0,76,15,0);
   show_chinese(0,92,16,0);
   show_number(0,108,12,0);
}
void DisplayEnter_NewPassword(void)//请输入新密码：
{
   clear_screen();
   show_chinese(0,12,1,0);
   show_chinese(0,28,4,0);
   show_chinese(0,44,5,0);
   show_chinese(0,60,14,0);
   show_chinese(0,76,15,0);
   show_chinese(0,92,16,0);
   show_number(0,108,12,0);
}
void DisplayEnter_Password(void)//请输入密码：
{
   clear_screen();
   show_chinese(0,20,1,0);
   show_chinese(0,36,4,0);
   show_chinese(0,52,5,0);
   show_chinese(0,68,15,0);
   show_chinese(0,84,16,0);
   show_number(0,100,12,0);
}
void DisplayCheckPasswordError(void)//密码验证失败!
{
   clear_screen();
   show_chinese(1,8,15,0);
   show_chinese(1,24,16,0);
   show_chinese(1,40,19,0);
   show_chinese(1,56,20,0);
   show_chinese(1,72,21,0);
   show_chinese(1,88,22,0);
   show_chinese(1,104,13,0);
}
void DisplayCheckPasswordOK(void)//密码验证成功!
{
   clear_screen();
   show_chinese(1,8,15,0);
   show_chinese(1,24,16,0);
   show_chinese(1,40,19,0);
   show_chinese(1,56,20,0);
   show_chinese(1,72,25,0);
   show_chinese(1,88,26,0);
   show_chinese(1,104,13,0);
}
void DisplayModifyPasswordOK(void)//密码修改成功！
{
   clear_screen();
   show_chinese(1,8,15,0);
   show_chinese(1,24,16,0);
   show_chinese(1,40,23,0);
   show_chinese(1,56,24,0);
   show_chinese(1,72,25,0);
   show_chinese(1,88,26,0);
   show_chinese(1,104,13,0);
}
void DisplayEnterPhoneNumber(void)//请输入手机号：
{
   clear_screen();
   show_chinese(0,12,1,0);
   show_chinese(0,28,4,0);
   show_chinese(0,44,5,0);
   show_chinese(0,60,56,0);
   show_chinese(0,76,57,0);
   show_chinese(0,92,58,0);
   show_number(0,108,12,0);
}
void DisplayManagerFineger(void) //请输入车主指纹:
{
    clear_screen();
	show_chinese(1,4,1,0);
	show_chinese(1,20,4,0);
	show_chinese(1,36,5,0);
	show_chinese(1,52,42,0);
	show_chinese(1,68,43,0);
	show_chinese(1,84,6,0);
	show_chinese(1,100,7,0);
	show_number(1,116,12,0);

}
void DisplayFirstEnterFinger(void)	 //请输入指纹：
{
	clear_screen();
	show_chinese(1,20,1,0);
	show_chinese(1,36,4,0);
	show_chinese(1,52,5,0);
	show_chinese(1,68,6,0);
	show_chinese(1,84,7,0);
	show_number(1,100,12,0);
}
void DisplaySecondEnterFinger(void) //请再次输入指纹：
{
	clear_screen();
	show_chinese(1,4,1,0);
	show_chinese(1,20,2,0);
	show_chinese(1,36,3,0);
	show_chinese(1,52,4,0);
	show_chinese(1,68,5,0);
	show_chinese(1,84,6,0);
	show_chinese(1,100,7,0);
	show_number(1,116,12,0);
}	
void DisplayEnterFingerOK(void)//指纹录入成功！
{
   clear_screen();
   show_chinese(1,8,6,0);
   show_chinese(1,24,7,0);
   show_chinese(1,40,17,0);
   show_chinese(1,56,18,0);
   show_chinese(1,72,25,0);
   show_chinese(1,88,26,0);
   show_chinese(1,104,13,0);

} 
void DisplayEnterFingerError(void)//指纹录入失败！
{
   clear_screen();
   show_chinese(1,8,6,0);
   show_chinese(1,24,7,0);
   show_chinese(1,40,17,0);
   show_chinese(1,56,18,0);
   show_chinese(1,72,21,0);
   show_chinese(1,88,22,0);
   show_chinese(1,104,13,0);
}
void DisplayCheckFingerOK(void)	//指纹验证成功！
{
   clear_screen();
   show_chinese(1,8,6,0);
   show_chinese(1,24,7,0);
   show_chinese(1,40,19,0);
   show_chinese(1,56,20,0);
   show_chinese(1,72,25,0);
   show_chinese(1,88,26,0);
   show_chinese(1,104,13,0);
}
void DisplayCheckFingerError(void)	//指纹验证失败！
{
   clear_screen();
   show_chinese(1,8,6,0);
   show_chinese(1,24,7,0);
   show_chinese(1,40,19,0);
   show_chinese(1,56,20,0);
   show_chinese(1,72,21,0);
   show_chinese(1,88,22,0);
   show_chinese(1,104,13,0);
}
void DisplayRenewPassword(void) //密码已恢复！
{
   clear_screen();
   show_chinese(1,0,37,0);
   show_chinese(1,16,38,0);
   show_chinese(1,32,15,0);
   show_chinese(1,48,16,0);
   show_chinese(1,64,28,0);
   show_chinese(1,80,31,0);
   show_chinese(1,96,32,0);
   show_chinese(1,112,13,0);
}
void DisplayRenewFunction(void) //系统功能已恢复！
{
   clear_screen();
   show_chinese(1,0,37,0);
   show_chinese(1,16,38,0);
   show_chinese(1,32,33,0);
   show_chinese(1,48,34,0);
   show_chinese(1,64,28,0);
   show_chinese(1,80,31,0);
   show_chinese(1,96,32,0);
   show_chinese(1,112,13,0);

}
void DisplayCancelFunction(void) //系统功能已取消！
{
   clear_screen();
   show_chinese(1,0,37,0);
   show_chinese(1,16,38,0);
   show_chinese(1,32,33,0);
   show_chinese(1,48,34,0);
   show_chinese(1,64,28,0);
   show_chinese(1,80,35,0);
   show_chinese(1,96,36,0);
   show_chinese(1,112,13,0);
}
void DisplayEnterPhoneNumberOK(void) //手机号设置成功！
{
   clear_screen();
   show_chinese(1,0,56,0);
   show_chinese(1,16,57,0);
   show_chinese(1,32,58,0);
   show_chinese(1,48,52,0);
   show_chinese(1,64,53,0);
   show_chinese(1,80,25,0);
   show_chinese(1,96,26,0);
   show_chinese(1,112,13,0);
}
void DisplayNetOK(void)	//网络检测成功！
{
   clear_screen();
   show_chinese(1,8,60,0);
   show_chinese(1,24,61,0);
   show_chinese(1,40,62,0);
   show_chinese(1,56,63,0);
   show_chinese(1,72,25,0);
   show_chinese(1,88,26,0);
   show_chinese(1,104,13,0);
}
void DisplayNetError(void)	//无网络信号！
{
   clear_screen();
   show_chinese(1,16,59,0);
   show_chinese(1,32,60,0);
   show_chinese(1,48,61,0);
   show_chinese(1,64,64,0);
   show_chinese(1,80,65,0);
   show_chinese(1,96,13,0);
}

void DisplayInputSec(void)	//请输入区号:
{
   clear_screen();
   show_chinese(0, 0,1,0);
   show_chinese(0,16,4,0);
   show_chinese(0,32,5,0);
   show_chinese(0,48,69,0);
   show_chinese(0,64,72,0);
   show_chinese(0,80,8,0);	
}


void DisplayInputRoom(void)	//请输入房号:
{
   clear_screen();
   show_chinese(0, 0,1,0);
   show_chinese(0,16,4,0);
   show_chinese(0,32,5,0);
   show_chinese(0,48,70,0);
   show_chinese(0,64,72,0);
   show_chinese(0,80,8,0);	
}


void DisplayInputBed(void)	//请输入床号:
{
   clear_screen();
   show_chinese(0, 0,1,0);
   show_chinese(0,16,4,0);
   show_chinese(0,32,5,0);
   show_chinese(0,48,71,0);
   show_chinese(0,64,72,0);
   show_chinese(0,80,8,0);	
}


void DisplayInputNumber(void)	//输入具体数字
{

	show_number(2,96,BCD[0],0);
	show_number(2,104,BCD[1],0);
	show_number(2,112,BCD[2],0);
	
}


void ClearInputSec(void)
{
	show_number(2,96,13,0);
	show_number(2,104,13,0);
	show_number(2,112,13,0);
}