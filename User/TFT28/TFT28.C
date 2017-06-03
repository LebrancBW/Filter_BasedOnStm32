/****************************************************************************************************  
�ļ�����������320x240TFT�������򣬿���TFTʵ�ֺ���,�ַ���ʾ,���㹦�ܡ�
****************************************************************************************************/
#include "TFT28.h"

//**************�����ⲿ�����ͱ���**************

void GPIO_Config(void)

{

	/* 配置数据口的GPIO管脚模式，输出模式*/
		GPIO_InitTypeDef GPIO_InitStructure;
  	RCC_APB2PeriphClockCmd(RCC_GPIO_TFT, ENABLE); 					//使能GPIO时钟

  	GPIO_InitStructure.GPIO_Pin = DS1_PIN|DS2_PIN|DS3_PIN|DS4_PIN|DS5_PIN|DS6_PIN|DS7_PIN|DS8_PIN; //使用PC0~PC7

  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;				//GPIO工作在输出模式

  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;

  	GPIO_Init(GPIO_TFT_DATA, &GPIO_InitStructure);  				//相关的GPIO口初始化

	

	/* 配置控制口A的GPIO管脚模式，输出模式*/

  	RCC_APB2PeriphClockCmd(RCC_GPIO_CTRA, ENABLE); 					//使能GPIO时钟

  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;  //使用PA0、PA1

  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;				//GPIO工作在输出模式

  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;

  	GPIO_Init(GPIO_CTRA, &GPIO_InitStructure);  						//相关的GPIO口初始化

		

		/* 配置LED灯使用的GPIO管脚模式，输出模式*/

  	RCC_APB2PeriphClockCmd(RCC_GPIO_CTRB, ENABLE); 					//使能GPIO时钟

  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 |GPIO_Pin_2 |GPIO_Pin_8 ; //使用PB0.1.2.8

  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;				//GPIO工作在输出模式

  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;

  	GPIO_Init(GPIO_CTRB, &GPIO_InitStructure);  						//相关的GPIO口初始化

		

		GPIO_ResetBits(GPIO_CTRB,GPIO_Pin_8);

}
/****************************************************************************************************
//	ʵ�ֹ��ܣ�	��ʱ
//	���������	count ������ʱʱ��
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
//	ʵ�ֹ��ܣ�	д����
//	���������  DH ��Ҫ����16bits����ĸ�8λ
//              DL ��Ҫ����16bits����ĵ�8λ
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

		//注意：当使用8位数据口驱动16位数据模式时，使用74HC573作为IO扩展，程序如下           
		GPIO_Write(DataPort,(u16)(DL | 0xff00));     	//送低8位命令给573待锁存
		LE_SET;                   										//锁存位
		LE_CLR;                   										//断开锁存,位选573的Q7~Q0仍保持           
		GPIO_Write(DataPort,(u16)(DH | 0xff00));			//送高8位命令给TFT
	}
	else if(Device_code==0x9320)
	{
		//注意：当使用8位数据口驱动16位数据模式时，使用74HC573作为IO扩展，程序如下           
		GPIO_Write(DataPort,(u16)(DH | 0xff00));     	//送低8位命令给573待锁存
		LE_SET;                   										//锁存位
		LE_CLR;                   										//断开锁存,位选573的Q7~Q0仍保持           
		GPIO_Write(DataPort,(u16)(DL | 0xff00));			//送高8位命令给TFT
	}

	/*
    //如果使用16位数据口驱动16位数据模式，则无需IO扩展，直接将数据送到数据口即可
    DataPort_L=DL; 
    DataPort_H=DH;
	*/

	RW_SET;
	CS_SET;
}


/****************************************************************************************************
//	ʵ�ֹ��ܣ�	д����(2*8bits)
//	���������  DH ��Ҫ����16bits���ݵĸ�8λ
//              DL ��Ҫ����16bits���ݵĵ�8λ
****************************************************************************************************/
void Write_Data(unsigned char DH,unsigned char DL)
{
	int Device_code = 0x9320;	
	CS_CLR;
	RS_SET;
	if(Device_code==0x9328)
	{
   //注意：当使用8位数据口驱动16位数据模式时，使用74HC573作为IO扩展，程序如下           
	 GPIO_Write(DataPort,(u16)(DL | 0xff00)); //送低8位数据给573待锁存
   LE_SET;                   //锁存位
   LE_CLR;                   //断开锁存,位选573的Q7~Q0仍保持           
	 GPIO_Write(DataPort,(u16)(DH | 0xff00));  //送高8位数据给TFT
	 //delayms(1);
	}
	
	else if(Device_code==0x9320)
	{
		//注意：当使用8位数据口驱动16位数据模式时，使用74HC573作为IO扩展，程序如下           
		GPIO_Write(DataPort,(u16)(DH | 0xff00));     	//送低8位命令给573待锁存
		LE_SET;                   										//锁存位
		LE_CLR;                   										//断开锁存,位选573的Q7~Q0仍保持           
		GPIO_Write(DataPort,(u16)(DL | 0xff00));			//送高8位命令给TFT
	}

	/*
    //如果使用16位数据口驱动16位数据模式，则无需IO扩展，直接将数据送到数据口即可
    DataPort_L=DL; 
    DataPort_H=DH;
	*/

	RW_CLR;
	RW_SET;
	CS_SET;
}

/****************************************************************************************************
//	ʵ�ֹ��ܣ�	д����(16λ)
//	���������  y ��Ҫ����16bits����
****************************************************************************************************/
void  Write_Data_U16(unsigned int y)
{
	unsigned char m,n;
	m=y>>8;
	n=y;
	Write_Data(m,n);
}

/****************************************************************************************************
//	ʵ�ֹ��ܣ�	��x�Ĵ���д��y����
//	���������  x ��Ҫ��������� 16λ
//              y ��Ҫ��������� 16λ
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
//	ʵ�ֹ��ܣ�	��������
//  ���������  x0��y0 ��ʼ����
//              x1��y1 ��������
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
//	ʵ�ֹ��ܣ�	TFT����
//	���������	bColor ������ʹ�õı���ɫ
****************************************************************************************************/
void CLR_Screen(unsigned int bColor)
{
 unsigned int i,j;
 LCD_SetPos(0,239,0,319);													//320x240�������0��ʼ
 for (i=0;i<240;i++)
	{
	   for (j=0;j<320;j++)
	       Write_Data_U16(bColor);
	}
}

/****************************************************************************************************
//	ʵ�ֹ��ܣ�	��ʾAscii�ַ�
//  ���������  x ������
//              y ������
//		        c ��Ҫ��ʾ���ַ�
//		        fColor �ַ���ɫ
//		        bColor �ַ�������ɫ
****************************************************************************************************/
void LCD_PutChar(unsigned int x, unsigned int y, char c, unsigned int fColor, unsigned int bColor)
{
 unsigned int i,j;
 LCD_SetPos(x,x+16-1,y-8+1,y);                    //�����ַ���ʾλ��
 for(i=0; i<16;i++) {                             //ѭ��д��16�ֽڣ�һ���ַ�Ϊ16�ֽ�
		unsigned char m=Font8x16[(c-0x20)*16+i];  //��ȡc�ַ��ĵ�i���ֽ���,c��ȥ0x20������Ascii����е�0~1f��ȥ��
		for(j=0;j<8;j++) {                        //ѭ��д��8λ��һ���ֽ�Ϊ8λ
			if((m&0x80)==0x80) {                  //�ж����λ�Ƿ�Ϊ1
				Write_Data_U16(fColor);           //���λΪ1��д���ַ���ɫ
				}
			else {
				Write_Data_U16(bColor);           //���λΪ0��д�뱳����ɫ
				}
			m<<=1;                                //����1λ��׼��д��һλ
			}
		}
}

/****************************************************************************************************
//	ʵ�ֹ��ܣ�	��ʾ����Ascii�ַ�����Ҫ��Ϊ����������
//  ��������� x ������
//            y ������
//		        c ��Ҫ��ʾ���ַ�
//		        fColor �ַ���ɫ
//		        bColor �ַ�������ɫ
****************************************************************************************************/
void LCD_PutSingleChar(unsigned int x, unsigned int y, char c, unsigned int fColor, unsigned int bColor)
{
 unsigned int i,j;
	
	i=y;
	j=x;
	x=i;
	y=319-j;
	
 LCD_SetPos(x,x+16-1,y-8+1,y);                    //�����ַ���ʾλ��
 for(i=0; i<16;i++) {                             //ѭ��д��16�ֽڣ�һ���ַ�Ϊ16�ֽ�
		unsigned char m=Font8x16[(c-0x20)*16+i];  		//��ȡc�ַ��ĵ�i���ֽ���,c��ȥ0x20������Ascii����е�0~1f��ȥ��
		for(j=0;j<8;j++) {                        		//ѭ��д��8λ��һ���ֽ�Ϊ8λ
			if((m&0x80)==0x80) {                  			//�ж����λ�Ƿ�Ϊ1
				Write_Data_U16(fColor);           				//���λΪ1��д���ַ���ɫ
				}
			else {
				Write_Data_U16(bColor);           				//���λΪ0��д�뱳����ɫ
				}
			m<<=1;                                			//����1λ��׼��д��һλ
			}
		}
}

/****************************************************************************************************
//	ʵ�ֹ��ܣ�	��ʾ16x16����
//  ��������� x ������
//            y ������
//		        g ��Ҫ��ʾ���ַ�����
//		        fColor �ַ���ɫ
//		        bColor �ַ�������ɫ
****************************************************************************************************/


/****************************************************************************************************
��������: ��ʾ����24x24
���������x ������
         y ������
         c ��Ҫ��ʾ�ĺ�����
	       fColor �ַ���ɫ
         bColor �ַ�������ɫ
****************************************************************************************************/


/****************************************************************************************************
//	ʵ�ֹ��ܣ�	��ʾ��Ӣ���ַ���
//  ��������� x ������
//            y ������
//		        *s ����ʾ���ַ���,����LCD_PutString(24,16,"123Eee",White,Blue);����"123Eee"�ĵ�һ���ַ���ַ����ָ�����s.
//		        bColor �ַ�������ɫ
****************************************************************************************************/
void LCD_PutString(unsigned short x, unsigned short y, unsigned char *s, unsigned int fColor, unsigned int bColor) 
{
	 unsigned char l=0;                            	//��ʾ��λ������
	 unsigned short i,j;
	
	i=y;                                            //�����������任���������������Ҫ�任
	j=x;
	x=i;
	y=319-j;
	
     while(*s) 
	 {
		if( *s < 0x80)                             		//�ж�sָ����ַ����е�ĳ�ַ��ı���ֵ�Ƿ�С��128,���С��,��ΪASCII�ַ�
		    {
			 LCD_PutChar(x,y-l*8,*s,fColor,bColor);			//��ʾ���ַ�
		     s++;l++;                              		//ָ���1,λ�ü�1
			}
	 }
}

/****************************************************************************************************
��������:��ʾ�ַ���24x24��С,������Ӣ��ͬʱ��ʾ
���������x ������
         y ������
         *s ��Ҫ��ʾ���ַ���
				 fColor �ַ���ɫ
         bColor �ַ�������ɫ
****************************************************************************************************/
void LCD_PutString24(unsigned short x , unsigned short y, unsigned char *s, unsigned int fColor, unsigned int bColor) {
	 unsigned char l=0;
	 unsigned short i,j;
	i=y;																											//����任
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
//	ʵ�ֹ��ܣ�	ָ��λ����ʾRGB��ɫ
//  ��������� x0��y0 ��ʼ����
//            x1��y1 ��������
//		        Color  ������ɫ
****************************************************************************************************/

void Show_RGB (unsigned short x0,unsigned short x1,unsigned short y0,unsigned short y1,unsigned int Color)
{
	unsigned int i,j;
	
	LCD_SetPos(y0,y1,319-x1,319-x0);												//��������Ҫ��������任���������������ֱ����x0,x1,y0,y1������
	for (i=x0;i<=x1;i++)
	{
	   for (j=y0;j<=y1;j++)
	       Write_Data_U16(Color);
	}
}

/****************************************************************************************************
//	ʵ�ֹ��ܣ�	TFT��ʼ��
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
	Write_Cmd_Data(0x00,0x0001);   		//Set the OSC bit as ‘1’ to start the internal oscillator
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
	Write_Cmd_Data(0x11, 0x0007);   //Write final user’s setting values to VC bit
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
	Write_Cmd_Data(0x00,0x0001);   //Set the OSC bit as ‘1’ to start the internal oscillator
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
	Write_Cmd_Data(0x11, 0x0007);   //Write final user’s setting values to VC bit
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
	 Write_Cmd_Data(0x0001,0x0100);   //set SS and SM bit //设置扫描方向  100
   Write_Cmd_Data(0x0002,0x0700);   //EOR=1 and B/C=1 to set the line inversion  //设置行反转
   Write_Cmd_Data(0x0003,0x1098);   //set Entry Mode  //设置进入模式    1030
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
//	ʵ�ֹ��ܣ�	����
//  ��������� x��y ��Ҫ��������
//            color �����ɫ
****************************************************************************************************/
void Put_pixel(unsigned int x,unsigned int y,unsigned int color)
{
	LCD_SetPos(y,y,319-x,319-x);												//��������Ҫ��������任���������������ֱ����x0,x1,y0,y1������

	Write_Data_U16(color);      												//��ָ����д����ɫ����
}


