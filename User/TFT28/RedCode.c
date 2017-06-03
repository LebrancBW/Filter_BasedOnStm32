#include "stm32f10x.h"                   //STM32�����Ĵ�������ͷ�ļ����������
#include "user_Config.h"                  //�û�����ͷ�ļ�����������Ӳ��������Դ,λ����Ŀ�ļ�����

//***********************************************************************
//			ң��������
//***********************************************************************

#define KEY_0     0x16                 //��0
#define KEY_1     0x0C                 //��1
#define KEY_2     0x18                 //��2
#define KEY_3     0x5E                 //��3
#define KEY_4     0x08                 //��4
#define KEY_5     0x1C                 //��5
#define KEY_6     0x5A                 //��6
#define KEY_7     0x42                 //��7
#define KEY_8     0x52                 //��8
#define KEY_9     0x4A                 //��9
#define KEY_POWER 0x45                 //��Դ
#define KEY_MODE  0x46                 //ģʽ
#define KEY_SOUND 0x47                 //����

//�������ͷH1838���ƽţ����ſ���
#define RCC_GPIO_1838                RCC_APB2Periph_GPIOB    //����PB7
#define GPIO_1838                    GPIOB                   //ʹ�õ�GPIOΪPB
#define GPIO_Pin_1838 							 GPIO_Pin_7

#define RED_L	        GPIO_ResetBits(GPIO_1838,GPIO_Pin_1838)	    //�õ͵�ƽ
#define RED_H	        GPIO_SetBits(GPIO_1838,GPIO_Pin_1838)	      //�øߵ�ƽ
#define RED_R	        GPIO_ReadInputDataBit(GPIO_1838, GPIO_Pin_1838)		      //����ƽ

void delay_us(u32 Nus);
void delay_mms(u16 nms);
void LCD_PutString24(unsigned short x , unsigned short y, unsigned char *s, unsigned int fColor, unsigned int bColor);

u8 KEY_VAL=0;
u8 code_value=0;
u8 key_code[4]; 

//*************************************************************************
//			ң������ֵ������ֵ�ӳ���
//*************************************************************************
 void Key_Icode(void)
 {
   switch(key_code[2])                    //��3���ֽ������ݣ���4���ֽ��Ƿ���
   {                                      //Ϊ�˸��ȶ������Լ��ϵ�4���ֽ����ݵ��ж�
   case KEY_0:KEY_VAL=0;break;
   case KEY_1:KEY_VAL=1;break;
   case KEY_2:KEY_VAL=2;break;
   case KEY_3:KEY_VAL=3;break;
   case KEY_4:KEY_VAL=4;break;
   case KEY_5:KEY_VAL=5;break;
   case KEY_6:KEY_VAL=6;break;
   case KEY_7:KEY_VAL=7;break;
   case KEY_8:KEY_VAL=8;break;
   case KEY_9:KEY_VAL=9;break;
   default:KEY_VAL=10;break;
   }

   
 }
//*************************************************************************
//	ң���������ӳ���
//*************************************************************************

void Red_Code()
{
u8 i,j,k = 0;

 for(i = 0;i < 19;i++)
{
  delay_us(400);           //��ʱ400us
  if(RED_R)                //9ms���иߵ�ƽ�����ж�Ϊ���ţ��˳��������
  {
      return;
  }
}

while(!RED_R);            //�ȴ�9ms�͵�ƽ��ȥ



for(i=0;i<5;i++)          //�Ƿ�������
{
  delay_us(500);
  if(!RED_R)
   {
     return;
   }
}


while(RED_R);               //�ȴ�4.5ms�ߵ�ƽ��ȥ

for(i = 0;i < 4;i++)        //����4���ֽڵ�����
{
  for(j = 0;j < 8;j++)      //ÿ���ֽڵ�����8λ
  {
   

   while(!RED_R);           //�ȴ���ߵ�ƽ
   while(RED_R)             //����ߵ�ƽʱ��
   {
    delay_us(100);
    k++;
    if(k >22)               //�ߵ�ƽʱ����������˳��������
    {
     return;         
    }
    
   }

   code_value>>=1;          //����һλ����
   if(k >= 7)
   {
    code_value|=0x80;       //�ߵ�ƽʱ�����0.56����Ϊ����1
   }
   k = 0;                  //��ʱ����
  }
  key_code[i]=code_value;  //�ĸ��ֽڵ�����
}

  Key_Icode();             //���ø�ֵ����

  //Display_Key(KEY_VAL);     //��ʾ����ֵ
  GPIO_Write(DataPort,(u16)(KEY_VAL | 0xff00));			//��ʾ

  delay_mms(50);

}
