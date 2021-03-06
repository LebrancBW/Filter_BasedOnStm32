/****************************************************************************************************  
文件功能描述：320x240TFT驱动程序，控制TFT实现汉字,字符显示,画点功能。
****************************************************************************************************/
#include "TFT28.h"

//**************声明外部函数和变量**************

void GPIO_Config(void)

{

	/* 閰嶇疆鏁版嵁鍙ｇ殑GPIO绠¤剼妯″紡锛岃緭鍑烘ā寮�*/
		GPIO_InitTypeDef GPIO_InitStructure;
  	RCC_APB2PeriphClockCmd(RCC_GPIO_TFT, ENABLE); 					//浣胯兘GPIO鏃堕挓

  	GPIO_InitStructure.GPIO_Pin = DS1_PIN|DS2_PIN|DS3_PIN|DS4_PIN|DS5_PIN|DS6_PIN|DS7_PIN|DS8_PIN; //浣跨敤PC0~PC7

  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;				//GPIO宸ヤ綔鍦ㄨ緭鍑烘ā寮�

  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;

  	GPIO_Init(GPIO_TFT_DATA, &GPIO_InitStructure);  				//鐩稿叧鐨凣PIO鍙ｅ垵濮嬪寲

	

	/* 閰嶇疆鎺у埗鍙鐨凣PIO绠¤剼妯″紡锛岃緭鍑烘ā寮�*/

  	RCC_APB2PeriphClockCmd(RCC_GPIO_CTRA, ENABLE); 					//浣胯兘GPIO鏃堕挓

  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;  //浣跨敤PA0銆丳A1

  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;				//GPIO宸ヤ綔鍦ㄨ緭鍑烘ā寮�

  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;

  	GPIO_Init(GPIO_CTRA, &GPIO_InitStructure);  						//鐩稿叧鐨凣PIO鍙ｅ垵濮嬪寲

		

		/* 閰嶇疆LED鐏娇鐢ㄧ殑GPIO绠¤剼妯″紡锛岃緭鍑烘ā寮�*/

  	RCC_APB2PeriphClockCmd(RCC_GPIO_CTRB, ENABLE); 					//浣胯兘GPIO鏃堕挓

  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 |GPIO_Pin_2 |GPIO_Pin_8 ; //浣跨敤PB0.1.2.8

  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;				//GPIO宸ヤ綔鍦ㄨ緭鍑烘ā寮�

  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;

  	GPIO_Init(GPIO_CTRB, &GPIO_InitStructure);  						//鐩稿叧鐨凣PIO鍙ｅ垵濮嬪寲

		

		GPIO_ResetBits(GPIO_CTRB,GPIO_Pin_8);

}
/****************************************************************************************************
//	实现功能：	延时
//	输入参数：	count 设置延时时间
****************************************************************************************************/
void delayms(unsigned int count)
{
    int i,j;                                                                                
    for(i=0;i<count;i++)                                                                    
       {
	     for(j=0;j<0x640;j++);
       }                                                                                     
}



/****************************************************************************************************
//	实现功能：	写命令
//	输入参数：  DH 需要输入16bits命令的高8位
//              DL 需要输入16bits命令的低8位
****************************************************************************************************/
	void Write_Cmd(unsigned char DH,unsigned char DL)
{
	int Device_code = 0x9320;
	CS_CLR;
	RS_CLR;
  RD_SET;
  RW_CLR;
	
	if(Device_code==0x9328)
	{

		//娉ㄦ剰锛氬綋浣跨敤8浣嶆暟鎹彛椹卞姩16浣嶆暟鎹ā寮忔椂锛屼娇鐢�74HC573浣滀负IO鎵╁睍锛岀▼搴忓涓�           
		GPIO_Write(DataPort,(u16)(DL | 0xff00));     	//閫佷綆8浣嶅懡浠ょ粰573寰呴攣瀛�
		LE_SET;                   										//閿佸瓨浣�
		LE_CLR;                   										//鏂紑閿佸瓨,浣嶉��573鐨凲7~Q0浠嶄繚鎸�           
		GPIO_Write(DataPort,(u16)(DH | 0xff00));			//閫侀珮8浣嶅懡浠ょ粰TFT
	}
	else if(Device_code==0x9320)
	{
		//娉ㄦ剰锛氬綋浣跨敤8浣嶆暟鎹彛椹卞姩16浣嶆暟鎹ā寮忔椂锛屼娇鐢�74HC573浣滀负IO鎵╁睍锛岀▼搴忓涓�           
		GPIO_Write(DataPort,(u16)(DH | 0xff00));     	//閫佷綆8浣嶅懡浠ょ粰573寰呴攣瀛�
		LE_SET;                   										//閿佸瓨浣�
		LE_CLR;                   										//鏂紑閿佸瓨,浣嶉��573鐨凲7~Q0浠嶄繚鎸�           
		GPIO_Write(DataPort,(u16)(DL | 0xff00));			//閫侀珮8浣嶅懡浠ょ粰TFT
	}

	/*
    //濡傛灉浣跨敤16浣嶆暟鎹彛椹卞姩16浣嶆暟鎹ā寮忥紝鍒欐棤闇�IO鎵╁睍锛岀洿鎺ュ皢鏁版嵁閫佸埌鏁版嵁鍙ｅ嵆鍙�
    DataPort_L=DL; 
    DataPort_H=DH;
	*/

	RW_SET;
	CS_SET;
}


/****************************************************************************************************
//	实现功能：	写数据(2*8bits)
//	输入参数：  DH 需要输入16bits数据的高8位
//              DL 需要输入16bits数据的低8位
****************************************************************************************************/
void Write_Data(unsigned char DH,unsigned char DL)
{
	int Device_code = 0x9320;	
	CS_CLR;
	RS_SET;
	if(Device_code==0x9328)
	{
   //娉ㄦ剰锛氬綋浣跨敤8浣嶆暟鎹彛椹卞姩16浣嶆暟鎹ā寮忔椂锛屼娇鐢�74HC573浣滀负IO鎵╁睍锛岀▼搴忓涓�           
	 GPIO_Write(DataPort,(u16)(DL | 0xff00)); //閫佷綆8浣嶆暟鎹粰573寰呴攣瀛�
   LE_SET;                   //閿佸瓨浣�
   LE_CLR;                   //鏂紑閿佸瓨,浣嶉��573鐨凲7~Q0浠嶄繚鎸�           
	 GPIO_Write(DataPort,(u16)(DH | 0xff00));  //閫侀珮8浣嶆暟鎹粰TFT
	 //delayms(1);
	}
	
	else if(Device_code==0x9320)
	{
		//娉ㄦ剰锛氬綋浣跨敤8浣嶆暟鎹彛椹卞姩16浣嶆暟鎹ā寮忔椂锛屼娇鐢�74HC573浣滀负IO鎵╁睍锛岀▼搴忓涓�           
		GPIO_Write(DataPort,(u16)(DH | 0xff00));     	//閫佷綆8浣嶅懡浠ょ粰573寰呴攣瀛�
		LE_SET;                   										//閿佸瓨浣�
		LE_CLR;                   										//鏂紑閿佸瓨,浣嶉��573鐨凲7~Q0浠嶄繚鎸�           
		GPIO_Write(DataPort,(u16)(DL | 0xff00));			//閫侀珮8浣嶅懡浠ょ粰TFT
	}

	/*
    //濡傛灉浣跨敤16浣嶆暟鎹彛椹卞姩16浣嶆暟鎹ā寮忥紝鍒欐棤闇�IO鎵╁睍锛岀洿鎺ュ皢鏁版嵁閫佸埌鏁版嵁鍙ｅ嵆鍙�
    DataPort_L=DL; 
    DataPort_H=DH;
	*/

	RW_CLR;
	RW_SET;
	CS_SET;
}

/****************************************************************************************************
//	实现功能：	写数据(16位)
//	输入参数：  y 需要输入16bits数据
****************************************************************************************************/
void  Write_Data_U16(unsigned int y)
{
	unsigned char m,n;
	m=y>>8;
	n=y;
	Write_Data(m,n);
}

/****************************************************************************************************
//	实现功能：	向x寄存器写入y数据
//	输入参数：  x 需要输入的命令 16位
//              y 需要输入的数据 16位
****************************************************************************************************/
void  Write_Cmd_Data (unsigned char x,unsigned int y)
{
	unsigned char m,n;
	m=y>>8;
	n=y;
	Write_Cmd(0x00,x);
	Write_Data(m,n);
}

/****************************************************************************************************
//	实现功能：	设置坐标
//  输入参数：  x0，y0 起始坐标
//              x1，y1 结束坐标
****************************************************************************************************/
void LCD_SetPos(unsigned int x0,unsigned int x1,unsigned int y0,unsigned int y1)
//void LCD_SetPos(int x0,int x1,int y0,int y1)
{

  Write_Cmd_Data(0x50,x0);  // Horizontal GRAM Start Address
  Write_Cmd_Data(0x51,x1);  // Horizontal GRAM End Address
  Write_Cmd_Data(0x52,y0);  // Vertical GRAM Start Address
  Write_Cmd_Data(0x53,y1);  // Vertical GRAM Start Address
  Write_Cmd_Data(0x20,x0);  // GRAM horizontal Address
  Write_Cmd_Data(0x21,y0);  // GRAM Vertical Address
  Write_Cmd (0x00,0x22);    // 0x0022,Start to Write Data to GRAM 
}

/****************************************************************************************************
//	实现功能：	TFT清屏
//	输入参数：	bColor 清屏所使用的背景色
****************************************************************************************************/
void CLR_Screen(unsigned int bColor)
{
 unsigned int i,j;
 LCD_SetPos(0,239,0,319);													//320x240，坐标从0开始
 for (i=0;i<240;i++)
	{
	   for (j=0;j<320;j++)
	       Write_Data_U16(bColor);
	}
}

/****************************************************************************************************
//	实现功能：	显示Ascii字符
//  输入参数：  x 横坐标
//              y 纵坐标
//		        c 需要显示的字符
//		        fColor 字符颜色
//		        bColor 字符背景颜色
****************************************************************************************************/
void LCD_PutChar(unsigned int x, unsigned int y, char c, unsigned int fColor, unsigned int bColor)
{
 unsigned int i,j;
 LCD_SetPos(x,x+16-1,y-8+1,y);                    //设置字符显示位置
 for(i=0; i<16;i++) {                             //循环写入16字节，一个字符为16字节
		unsigned char m=Font8x16[(c-0x20)*16+i];  //提取c字符的第i个字节以,c减去0x20是由于Ascii码库中的0~1f被去掉
		for(j=0;j<8;j++) {                        //循环写入8位，一个字节为8位
			if((m&0x80)==0x80) {                  //判断最高位是否为1
				Write_Data_U16(fColor);           //最高位为1，写入字符颜色
				}
			else {
				Write_Data_U16(bColor);           //最高位为0，写入背景颜色
				}
			m<<=1;                                //左移1位，准备写下一位
			}
		}
}

/****************************************************************************************************
//	实现功能：	显示单个Ascii字符，主要是为横屏而设置
//  输入参数： x 横坐标
//            y 纵坐标
//		        c 需要显示的字符
//		        fColor 字符颜色
//		        bColor 字符背景颜色
****************************************************************************************************/
void LCD_PutSingleChar(unsigned int x, unsigned int y, char c, unsigned int fColor, unsigned int bColor)
{
 unsigned int i,j;
	
	i=y;
	j=x;
	x=i;
	y=319-j;
	
 LCD_SetPos(x,x+16-1,y-8+1,y);                    //设置字符显示位置
 for(i=0; i<16;i++) {                             //循环写入16字节，一个字符为16字节
		unsigned char m=Font8x16[(c-0x20)*16+i];  		//提取c字符的第i个字节以,c减去0x20是由于Ascii码库中的0~1f被去掉
		for(j=0;j<8;j++) {                        		//循环写入8位，一个字节为8位
			if((m&0x80)==0x80) {                  			//判断最高位是否为1
				Write_Data_U16(fColor);           				//最高位为1，写入字符颜色
				}
			else {
				Write_Data_U16(bColor);           				//最高位为0，写入背景颜色
				}
			m<<=1;                                			//左移1位，准备写下一位
			}
		}
}

/****************************************************************************************************
//	实现功能：	显示16x16汉字
//  输入参数： x 横坐标
//            y 纵坐标
//		        g 需要显示的字符编码
//		        fColor 字符颜色
//		        bColor 字符背景颜色
****************************************************************************************************/


/****************************************************************************************************
函数功能: 显示汉字24x24
输入参数：x 横坐标
         y 纵坐标
         c 需要显示的汉字码
	       fColor 字符颜色
         bColor 字符背景颜色
****************************************************************************************************/


/****************************************************************************************************
//	实现功能：	显示中英文字符串
//  输入参数： x 横坐标
//            y 纵坐标
//		        *s 待显示的字符串,例如LCD_PutString(24,16,"123Eee",White,Blue);即把"123Eee"的第一个字符地址赋给指针变量s.
//		        bColor 字符背景颜色
****************************************************************************************************/
void LCD_PutString(unsigned short x, unsigned short y, unsigned char *s, unsigned int fColor, unsigned int bColor) 
{
	 unsigned char l=0;                            	//显示屏位置增量
	 unsigned short i,j;
	
	i=y;                                            //这里进行坐标变换，由竖屏变横屏需要变换
	j=x;
	x=i;
	y=319-j;
	
     while(*s) 
	 {
		if( *s < 0x80)                             		//判断s指向的字符串中的某字符的编码值是否小于128,如果小于,即为ASCII字符
		    {
			 LCD_PutChar(x,y-l*8,*s,fColor,bColor);			//显示该字符
		     s++;l++;                              		//指针加1,位置加1
			}
	 }
}

/****************************************************************************************************
函数功能:显示字符串24x24大小,可以中英文同时显示
输入参数：x 横坐标
         y 纵坐标
         *s 需要显示的字符串
				 fColor 字符颜色
         bColor 字符背景颜色
****************************************************************************************************/
void LCD_PutString24(unsigned short x , unsigned short y, unsigned char *s, unsigned int fColor, unsigned int bColor) {
	 unsigned char l=0;
	 unsigned short i,j;
	i=y;																											//坐标变换
	j=x;
	x=i;
	y=319-j;
	while(*s) {
		if( *s < 0x80) 
		    {
			LCD_PutChar(x,y-l*8,*s,fColor,bColor);
			s++;l++;
			}
		}
	}

/****************************************************************************************************
//	实现功能：	指定位置显示RGB颜色
//  输入参数： x0，y0 起始坐标
//            x1，y1 结束坐标
//		        Color  背景颜色
****************************************************************************************************/

void Show_RGB (unsigned short x0,unsigned short x1,unsigned short y0,unsigned short y1,unsigned int Color)
{
	unsigned int i,j;
	
	LCD_SetPos(y0,y1,319-x1,319-x0);												//横屏这里要进行坐标变换，如果是竖屏，则直接是x0,x1,y0,y1做参数
	for (i=x0;i<=x1;i++)
	{
	   for (j=y0;j<=y1;j++)
	       Write_Data_U16(Color);
	}
}

/****************************************************************************************************
//	实现功能：	TFT初始化
****************************************************************************************************/
void TFT_Initial()
{      
	int Device_code = 0x9320;
	GPIO_Config();
	RST_SET;    
	delayms(1);                    			// Delay 1ms 
	RST_CLR;  
	delayms(10);                   			// Delay 10ms            
	RST_SET;  
	delayms(50);                   			// Delay 50 ms  
  
	if(Device_code==0x9320)
     {

    //************* Start Initial Sequence **********//
	Write_Cmd_Data(0x00,0x0001);   		//Set the OSC bit as 鈥�1鈥� to start the internal oscillator
  Write_Cmd_Data(0x01,0x0100);   	// set SS and SM bit
	Write_Cmd_Data(0x02,0x0700);   		// set 1 line inversion
	Write_Cmd_Data(0x03,0x1098);   		//set GRAM Write direction and BGR=1
  Write_Cmd_Data(0x04,0x0000);   	// Resize register
	Write_Cmd_Data(0x08,0x0202);   		// set the back porch and front porch
  Write_Cmd_Data(0x09,0x0000);   	// set non-display area refresh cycle ISC[3:0]
  Write_Cmd_Data(0x0A,0x0000);   	// FMARK function
  Write_Cmd_Data(0x0C,0x0000);   	// RGB interface setting
	Write_Cmd_Data(0x0D,0x0000);   		// Frame marker Position
  Write_Cmd_Data(0x0F,0x0000);   	// RGB interface polarity
  delayms(30);
	//*************Power On sequence ****************//
	Write_Cmd_Data(0x10, 0x16b0);   // SAP, BT[3:0], AP, DSTB, SLP, STB
	delayms(30);
	Write_Cmd_Data(0x11, 0x0007);   //Write final user鈥檚 setting values to VC bit
	Write_Cmd_Data(0x12, 0x013a);   // set Internal reference voltage
	Write_Cmd_Data(0x13, 0x1a00);   // VDV[4:0] for VCOM amplitude
  delayms(30);
  Write_Cmd_Data(0x29, 0x000c);   // Set VCM[5:0] for VCOMH
	delayms(30); // Delay 50ms

	// ----------- Adjust the Gamma Curve ----------//
	Write_Cmd_Data(0x0030, 0x0000);
	Write_Cmd_Data(0x0031, 0x0505);
	Write_Cmd_Data(0x0032, 0x0304);
	Write_Cmd_Data(0x0035, 0x0006);
	Write_Cmd_Data(0x0036, 0x0707);
	Write_Cmd_Data(0x0037, 0x0105);
	Write_Cmd_Data(0x0038, 0x0002);
	Write_Cmd_Data(0x0039, 0x0707);
	Write_Cmd_Data(0x003C, 0x0704);
	Write_Cmd_Data(0x003D, 0x0807);

	//------------------ Set GRAM area ---------------//
	Write_Cmd_Data(0x0050, 0x0000); // Horizontal GRAM Start Address
	Write_Cmd_Data(0x0051, 0x00EF); // Horizontal GRAM End Address
	Write_Cmd_Data(0x0052, 0x0000); // Vertical GRAM Start Address
	Write_Cmd_Data(0x0053, 0x013F); // Vertical GRAM Start Address
	Write_Cmd_Data(0x0060, 0x2700); // Gate Scan Line
	Write_Cmd_Data(0x0061, 0x0001); // NDL,VLE, REV
	Write_Cmd_Data(0x006A, 0x0000); // set scrolling line
  Write_Cmd_Data(0x20, 0x0000);   // GRAM horizontal Address
	Write_Cmd_Data(0x21, 0x0000);   // GRAM Vertical Address

	//-------------- Partial Display Control ---------//
	Write_Cmd_Data(0x0080, 0x0000);
	Write_Cmd_Data(0x0081, 0x0000);
	Write_Cmd_Data(0x0082, 0x0000);
	Write_Cmd_Data(0x0083, 0x0000);
	Write_Cmd_Data(0x0084, 0x0000);
	Write_Cmd_Data(0x0085, 0x0000);

	//-------------- Panel Control ---------//
  Write_Cmd_Data(0x90,0x0010);   //Frame Cycle Contral
	Write_Cmd_Data(0x92,0x0000);   //Panel Interface Contral
	Write_Cmd_Data(0x93,0x0003);   //Panel Interface Contral 3. 
	Write_Cmd_Data(0x95,0x0110);   //Frame Cycle Contral
	Write_Cmd_Data(0x97,0x0000);   // 
	Write_Cmd_Data(0x98,0x0000);   //Frame Cycle Contral.     

	//-------------- Display on ---------//
  Write_Cmd_Data(0x07,0x0173); 

	}

	else if(Device_code==0x1505 )
     {

    //************* Start Initial Sequence **********//
	Write_Cmd_Data(0x00,0x0001);   //Set the OSC bit as 鈥�1鈥� to start the internal oscillator
  Write_Cmd_Data(0x01,0x0100);   // set SS and SM bit
	Write_Cmd_Data(0x02,0x0700);   // set 1 line inversion
	Write_Cmd_Data(0x03,0x1030);   //set GRAM Write direction and BGR=1
  Write_Cmd_Data(0x04,0x0000);   // Resize register
	Write_Cmd_Data(0x08,0x0202);   // set the back porch and front porch
  Write_Cmd_Data(0x09,0x0000);   // set non-display area refresh cycle ISC[3:0]
  Write_Cmd_Data(0x0A,0x0000);   // FMARK function
  Write_Cmd_Data(0x0C,0x0000);   // RGB interface setting
	Write_Cmd_Data(0x0D,0x0000);   // Frame marker Position
  Write_Cmd_Data(0x0F,0x0000);   // RGB interface polarity
  delayms(30);
	//*************Power On sequence ****************//
	Write_Cmd_Data(0x10, 0x16b0);   // SAP, BT[3:0], AP, DSTB, SLP, STB
	delayms(30);
	Write_Cmd_Data(0x11, 0x0007);   //Write final user鈥檚 setting values to VC bit
	Write_Cmd_Data(0x12, 0x013a);   // set Internal reference voltage
	Write_Cmd_Data(0x13, 0x1a00);   // VDV[4:0] for VCOM amplitude
  delayms(30);
  Write_Cmd_Data(0x29, 0x000c);   // Set VCM[5:0] for VCOMH
	delayms(30); // Delay 50ms

	// ----------- Adjust the Gamma Curve ----------//
	Write_Cmd_Data(0x0030, 0x0000);
	Write_Cmd_Data(0x0031, 0x0505);
	Write_Cmd_Data(0x0032, 0x0304);
	Write_Cmd_Data(0x0035, 0x0006);
	Write_Cmd_Data(0x0036, 0x0707);
	Write_Cmd_Data(0x0037, 0x0105);
	Write_Cmd_Data(0x0038, 0x0002);
	Write_Cmd_Data(0x0039, 0x0707);
	Write_Cmd_Data(0x003C, 0x0704);
	Write_Cmd_Data(0x003D, 0x0807);

	//------------------ Set GRAM area ---------------//
	Write_Cmd_Data(0x0050, 0x0000); // Horizontal GRAM Start Address
	Write_Cmd_Data(0x0051, 0x00EF); // Horizontal GRAM End Address
	Write_Cmd_Data(0x0052, 0x0000); // Vertical GRAM Start Address
	Write_Cmd_Data(0x0053, 0x013F); // Vertical GRAM Start Address
	Write_Cmd_Data(0x0060, 0x2700); // Gate Scan Line
	Write_Cmd_Data(0x0061, 0x0001); // NDL,VLE, REV
	Write_Cmd_Data(0x006A, 0x2700); // set scrolling line
  Write_Cmd_Data(0x20, 0x0000);   // GRAM horizontal Address
	Write_Cmd_Data(0x21, 0x0000);   // GRAM Vertical Address

	//-------------- Partial Display Control ---------//
	Write_Cmd_Data(0x0080, 0x0000);
	Write_Cmd_Data(0x0081, 0x0000);
	Write_Cmd_Data(0x0082, 0x0000);
	Write_Cmd_Data(0x0083, 0x0000);
	Write_Cmd_Data(0x0084, 0x0000);
	Write_Cmd_Data(0x0085, 0x0000);

	//-------------- Panel Control ---------//
  Write_Cmd_Data(0x90,0x0010);   //Frame Cycle Contral
	Write_Cmd_Data(0x92,0x0000);   //Panel Interface Contral
	Write_Cmd_Data(0x93,0x0003);   //Panel Interface Contral 3. 
	Write_Cmd_Data(0x95,0x0110);   //Frame Cycle Contral
	Write_Cmd_Data(0x97,0x0000);   // 
	Write_Cmd_Data(0x98,0x0000);   //Frame Cycle Contral.     

	//-------------- Display on ---------//
  Write_Cmd_Data(0x07,0x0173); 

	}

  else if(Device_code==0x9328)
     {

    //************* Start Initial Sequence **********//
	 Write_Cmd_Data(0x0001,0x0100);   //set SS and SM bit //璁剧疆鎵弿鏂瑰悜  100
   Write_Cmd_Data(0x0002,0x0700);   //EOR=1 and B/C=1 to set the line inversion  //璁剧疆琛屽弽杞�
   Write_Cmd_Data(0x0003,0x1098);   //set Entry Mode  //璁剧疆杩涘叆妯″紡    1030
   Write_Cmd_Data(0x0004,0x0000);   //
   Write_Cmd_Data(0x00A4,0x0001);
   Write_Cmd_Data(0x0008,0x0202); // set the back porch and front porch
   Write_Cmd_Data(0x0009,0x0000); // set non-display area refresh cycle ISC[3:0]
   Write_Cmd_Data(0x000A,0x0000); // FMARK function
   Write_Cmd_Data(0x000C,0x0000); // RGB interface setting
   Write_Cmd_Data(0x000D, 0x0000); // Frame marker Position
   Write_Cmd_Data(0x000F, 0x0000); // RGB interface polarity



//*************Power On sequence ****************//
    Write_Cmd_Data(0x0010, 0x0000); // SAP, BT[3:0], AP, DSTB, SLP, STB 
    Write_Cmd_Data(0x0011, 0x0007); // DC1[2:0], DC0[2:0], VC[2:0]
    Write_Cmd_Data(0x0012, 0x0000); // VREG1OUT voltage
    Write_Cmd_Data(0x0013, 0x0000); // VDV[4:0] for VCOM amplitude 
    delayms(30);
    Write_Cmd_Data(0x0010, 0x1690); // SAP, BT[3:0], AP, DSTB, SLP, STB 
    Write_Cmd_Data(0x0011, 0x0227); // R11h=0x0221 at VCI=3.3V, DC1[2:0], DC0[2:0], VC[2:0]
    delayms(30);
    Write_Cmd_Data(0x0012, 0x001C); // External reference voltage= Vci;
    delayms(30); 
    Write_Cmd_Data(0x0013, 0x1800); // R13=1200 when R12=009D;VDV[4:0] for VCOM amplitude
    Write_Cmd_Data(0x0029, 0x001C); // R29=000C when R12=009D;VCM[5:0] for VCOMH
    Write_Cmd_Data(0x002B, 0x000D); // Frame Rate = 91Hz
    delayms(30);    
    Write_Cmd_Data(0x0020, 0x0000); // GRAM horizontal Address
    Write_Cmd_Data(0x0021, 0x0000); // GRAM Vertical Address
// ----------- Adjust the Gamma Curve ----------// 		 
	  Write_Cmd_Data(0x0030, 0x0007);
	  Write_Cmd_Data(0x0031, 0x0302);
    Write_Cmd_Data(0x0032, 0x0105);
	  Write_Cmd_Data(0x0035, 0x0206);
    Write_Cmd_Data(0x0036, 0x0808);	          
    Write_Cmd_Data(0x0037, 0x0206);
    Write_Cmd_Data(0x0038, 0x0504);
    Write_Cmd_Data(0x0039, 0x0007);
    Write_Cmd_Data(0x003C, 0x0105);
    Write_Cmd_Data(0x003D, 0x0808);
//------------------ Set GRAM area ---------------//
    Write_Cmd_Data(0x0050, 0x0000); // Horizontal GRAM Start Address
    Write_Cmd_Data(0x0051, 0x00EF); // Horizontal GRAM End Address
    Write_Cmd_Data(0x0052, 0x0000); // Vertical GRAM Start Address
	  delayms(30);
    Write_Cmd_Data(0x0053, 0x013F); // Vertical GRAM End Address
	  delayms(30);
    Write_Cmd_Data(0x0060, 0xA700); // Gate Scan Line
    Write_Cmd_Data(0x0061, 0x0001); // NDL,VLE, REV  
    Write_Cmd_Data(0x006A, 0x0000); // set scrolling line
//-------------- Partial Display Control ---------//
    Write_Cmd_Data(0x0080, 0x0000);
    Write_Cmd_Data(0x0081, 0x0000);
    Write_Cmd_Data(0x0082,0x0000); 
    Write_Cmd_Data(0x0083,0x0000); 
    Write_Cmd_Data(0x0084,0x0000); 
    Write_Cmd_Data(0x0085,0x0000); 
//-------------- Panel Control -------------------//        
    Write_Cmd_Data(0x0090, 0x0010);
    Write_Cmd_Data(0x0092, 0x0000);
    Write_Cmd_Data(0x0093, 0x0003);
    Write_Cmd_Data(0x0095, 0x0110);
    Write_Cmd_Data(0x0097, 0x0000);
    Write_Cmd_Data(0x0098, 0x0000);
    Write_Cmd_Data(0x0007, 0x0133); // 262K color and display ON

	}
}

/****************************************************************************************************
//	实现功能：	画点
//  输入参数： x，y 需要画点坐标
//            color 点的颜色
****************************************************************************************************/
void Put_pixel(unsigned int x,unsigned int y,unsigned int color)
{
	LCD_SetPos(y,y,319-x,319-x);												//横屏这里要进行坐标变换，如果是竖屏，则直接是x0,x1,y0,y1做参数

	Write_Data_U16(color);      												//在指定点写入颜色数据
}



