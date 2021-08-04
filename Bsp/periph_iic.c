#include "periph_iic.h"  
#include "periph_gpio.h"
#include "fc_board.h"

#if IIC_ENABLE
void IIC_GPIO_Config (void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(IIC_SCL_CLK | IIC_SDA_CLK , ENABLE);	

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 		
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	

	GPIO_InitStructure.GPIO_Pin = IIC_SCL_PIN; 		
	GPIO_Init(IIC_SCL_PORT, &GPIO_InitStructure);				   	

	GPIO_InitStructure.GPIO_Pin = IIC_SDA_PIN; 		
	GPIO_Init(IIC_SDA_PORT, &GPIO_InitStructure);				   

	GPIO_ResetBits(IIC_SCL_PORT,IIC_SCL_PIN);		
	GPIO_ResetBits(IIC_SDA_PORT,IIC_SDA_PIN);	
}
		
static void periph_IIC_Delay(uint16_t time)
{
	uint8_t delay_counter;
	while (time --)				
	{
		for (delay_counter = 0; delay_counter < 10; delay_counter++);
	}
}

void periph_IIC_SDA_Out(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;     
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;		    
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   
	
	GPIO_InitStructure.GPIO_Pin = IIC_SDA_PIN ;  		
	GPIO_Init(IIC_SDA_PORT, &GPIO_InitStructure);		
}

void periph_IIC_SDA_In(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;   		
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;		
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_InitStructure.GPIO_Pin = IIC_SDA_PIN ;  	
	GPIO_Init(IIC_SDA_PORT, &GPIO_InitStructure);
}

void IIC_Start(void)
{
	periph_IIC_SDA_Out();	
	
	IIC_SDA(1);		
	IIC_SCL(1);
	periph_IIC_Delay( IIC_DelayVaule );
	
	IIC_SDA(0);
	periph_IIC_Delay( IIC_DelayVaule );
	IIC_SCL(0);
	periph_IIC_Delay( IIC_DelayVaule );
}

void IIC_Stop(void)
{
	periph_IIC_SDA_Out();	
	
	IIC_SCL(0);
	periph_IIC_Delay( IIC_DelayVaule );
	IIC_SDA(0);
	periph_IIC_Delay( IIC_DelayVaule );
	
	IIC_SCL(1);
	periph_IIC_Delay( IIC_DelayVaule );
	IIC_SDA(1);
	periph_IIC_Delay( IIC_DelayVaule );
}

void IIC_ACK(void)
{
	periph_IIC_SDA_Out();
	
	IIC_SCL(0);
	periph_IIC_Delay( IIC_DelayVaule );
	IIC_SDA(0);
	periph_IIC_Delay( IIC_DelayVaule );	
	IIC_SCL(1);
	periph_IIC_Delay( IIC_DelayVaule );
	IIC_SCL(0);
	periph_IIC_Delay( IIC_DelayVaule );
	IIC_SDA(1);	
}

void IIC_NoACK(void)
{
	periph_IIC_SDA_Out(); 
	
	IIC_SCL(0);	
	periph_IIC_Delay( IIC_DelayVaule );
	IIC_SDA(1);
	periph_IIC_Delay( IIC_DelayVaule );
	IIC_SCL(1);
	periph_IIC_Delay( IIC_DelayVaule );
	IIC_SCL(0);
	periph_IIC_Delay( IIC_DelayVaule );
}

u8 IIC_WaitACK(void)
{
	periph_IIC_SDA_In();	

	IIC_SCL(1);
	periph_IIC_Delay( IIC_DelayVaule );	
	IIC_SCL(0);	
	
	if( GPIO_ReadInputDataBit(IIC_SDA_PORT,IIC_SDA_PIN) != 0)
		return (ACK_ERR);	
	else
		return (ACK_OK);	
}

u8 IIC_WriteByte(u8 IIC_Data)
{
	u8 i;

	periph_IIC_SDA_Out(); 
	
	for (i = 0; i < 8; i++)
	{
		IIC_SDA(IIC_Data & 0x80);
		
		periph_IIC_Delay( IIC_DelayVaule );
		IIC_SCL(1);
		periph_IIC_Delay( IIC_DelayVaule );
		IIC_SCL(0);		
		
		IIC_Data <<= 1;
	}
	return (IIC_WaitACK()); 
}

u8 IIC_ReadByte(u8 ACK_Mode)
{
	u8 IIC_Data;
	u8 i;
	
	IIC_SDA(1);
	IIC_SCL(0);
	periph_IIC_SDA_In(); 
	
	for (i = 0; i < 8; i++)
	{
		IIC_Data <<= 1;
		
		IIC_SCL(1);
		periph_IIC_Delay( IIC_DelayVaule );
		IIC_Data |= (GPIO_ReadInputDataBit(IIC_SDA_PORT,IIC_SDA_PIN) & 0x01);	
		IIC_SCL(0);
		periph_IIC_Delay( IIC_DelayVaule );
	}
	
	if ( ACK_Mode == 1 )		
		IIC_ACK();
	else
		IIC_NoACK();		 
	
	return (IIC_Data); 
}
#endif

