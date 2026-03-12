
#include "stm32f10x_lib.h"
/******************************************************
 1.主动发送   
    nrf24l01_init();
    i=SPI_Read(EN_AA);//读寄存器,从而判断I2C是否正确,I2C在第一次
                        //收ACK,能正确收到说明I2C没问题
	RX_Mode();
    然后循环
            TX_BUF[0]='S';

            TX_BUF[13]='E';

			TX_Mode(TX_BUF);//可以重复2次TX_Mode函数
			LED1=~LED1;
			delay_ms(166);
			TX_Mode(TX_BUF);
			SPI_RW_Reg(WRITE_REG+STATUS,0xff);//发送后,要清除这个，才能不断发数据,而不是重新上电
			RX_Mode();
			delay_ms(166);

            if(NRF24L01_RxPacket(RX_BUF))
            {
            
                if((RX_BUF[0]=='S')&&(RX_BUF[3]=='E'))
                {
                    LED2=~LED2;
				}
            }
 2.接收方
    nrf24l01_init();
    i=SPI_Read(EN_AA);//读寄存器,从而判断sPI是否正确
	RX_Mode();
    然后循环

        if(NRF24L01_RxPacket(RX_BUF))
        {
            if((RX_BUF[0]=='S')&&(RX_BUF[13]=='E'))
            {
                LED1=~LED1;
				TX_BUF[0]='S';
				TX_BUF[3]='E';
                TX_Mode(TX_BUF);
                delay_ms(166);
				SPI_RW_Reg(WRITE_REG+STATUS,0xff);//发送后,要清除这个，才能不断发数据,而不是重新上电
                RX_Mode();


            }
        }
 3.测试可以一个发,一个收,然后颠倒,测通了什么都好说




******************************************************/





#define CE   PAout(6)  // Chip Enable pin signal (output)
#define SCK  PAout(7)  // Master Out, Slave In pin (output)
#define MISO PBin(0)  // Master In, Slave Out pin (input)


#define CSN  PBout(1)  // Slave Select pin, (output to CSN, nRF24L01)
#define MOSI PBout(2)  // Serial Clock pin, (output)

#define IRQ  PBin(10)  // Interrupt signal, from nRF24L01 (input)

// SPI(nRF24L01) commands
#define READ_REG    0x00  // Define read command to register
#define WRITE_REG   0x20  // Define write command to register
#define RD_RX_PLOAD 0x61  // Define RX payload register address
#define WR_TX_PLOAD 0xA0  // Define TX payload register address
#define FLUSH_TX    0xE1  // Define flush TX register command
#define FLUSH_RX    0xE2  // Define flush RX register command
#define REUSE_TX_PL 0xE3  // Define reuse TX payload register command
#define NOP         0xFF  // Define No Operation, might be used to read status register

// SPI(nRF24L01) registers(addresses)
#define N2401_CONFIG      0x00  // 'N2401_CONFIG' register address
#define EN_AA       0x01  // 'Enable Auto Acknowledgment' register address
#define EN_RXADDR   0x02  // 'Enabled RX addresses' register address
#define SETUP_AW    0x03  // 'Setup address width' register address
#define SETUP_RETR  0x04  // 'Setup Auto. Retrans' register address
#define RF_CH       0x05  // 'RF channel' register address
#define RF_SETUP    0x06  // 'RF setup' register address
#define STATUS      0x07  // 'Status' register address
#define OBSERVE_TX  0x08  // 'Observe TX' register address
#define CD          0x09  // 'Carrier Detect' register address
#define RX_ADDR_P0  0x0A  // 'RX address pipe0' register address
#define RX_ADDR_P1  0x0B  // 'RX address pipe1' register address
#define RX_ADDR_P2  0x0C  // 'RX address pipe2' register address
#define RX_ADDR_P3  0x0D  // 'RX address pipe3' register address
#define RX_ADDR_P4  0x0E  // 'RX address pipe4' register address
#define RX_ADDR_P5  0x0F  // 'RX address pipe5' register address
#define TX_ADDR     0x10  // 'TX address' register address
#define RX_PW_P0    0x11  // 'RX payload width, pipe0' register address
#define RX_PW_P1    0x12  // 'RX payload width, pipe1' register address
#define RX_PW_P2    0x13  // 'RX payload width, pipe2' register address
#define RX_PW_P3    0x14  // 'RX payload width, pipe3' register address
#define RX_PW_P4    0x15  // 'RX payload width, pipe4' register address
#define RX_PW_P5    0x16  // 'RX payload width, pipe5' register address
#define FIFO_STATUS 0x17  // 'FIFO Status Register' register address



/***************************************************/
#define TX_ADR_WIDTH   5  // 5字节宽度的发送/接收地址
#define TX_PLOAD_WIDTH 16  // 数据通道有效数据宽度
vu8   TX_ADDRESS[TX_ADR_WIDTH] = {0x34,0x43,0x10,0x10,0x01};//0x34,0x43,0x10,0x10,0x01};  // ??????
vu8   RX_ADDRESS1[TX_ADR_WIDTH] = {0x35,0x43,0x10,0x10,0x01};//????????????,??????????
vu8   RX_ADDRESS2[TX_ADR_WIDTH] = {0x36,0x43,0x10,0x10,0x01};
vu8   RX_ADDRESS3[TX_ADR_WIDTH] = {0x37,0x43,0x10,0x10,0x01};
vu8   RX_ADDRESS4[TX_ADR_WIDTH] = {0x38,0x43,0x10,0x10,0x01};
vu8   RX_ADDRESS5[TX_ADR_WIDTH] = {0x39,0x43,0x10,0x10,0x01};


vu8 RX_BUF[TX_PLOAD_WIDTH+1]={0};
vu8 TX_BUF[TX_PLOAD_WIDTH+1]={0,0,0,0,0,0};
vu8 flag=0,flag_xiazai=0,i_uart=0;
vu8 sta;
#define RX_DR	(sta&0x40)
#define TX_DS	    (sta&0x20)
#define MAX_RT    (sta&0x10)
vu8  num=0,k=0;
vu8  daozhan_a=0,daozhan_b=0,num_a=0,num_b=0;
vu8  daozhan[]={"d0100           "};

/**************************************************/

/**************************************************
函数: init_io()

描述:
    初始化IO
**************************************************/
void nrf24l01_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	//按键初始化
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
    PBout(0)=1;
    PBout(10)=1;
	CE  = 0;        // 待机
	CSN = 1;        // SPI禁止
	SCK = 0;        // SPI时钟置低
	IRQ = 1;        // 中断复位

}

/**************************************************/

/**************************************************
函数：SPI_RW()

描述：
    根据SPI协议，写一字节数据到nRF24L01，同时从nRF24L01
	读出一字节
**************************************************/
vu8 SPI_RW(vu8 byte)
{
	vu8 i;

	SCK=0;
	delay_us(1);
   	for(i=0; i<8; i++)          // 循环8次
   	{
		delay_us(1);
   		if((byte & 0x80)==0x80)   // byte最高位输出到MOSI
			MOSI=1;
		else
			MOSI=0;  
   		byte <<= 1;             // 低一位移位到最高位
		delay_us(1);
   		SCK = 1;                // 拉高SCK，nRF24L01从MOSI读入1位数据，同时从MISO输出1位数据

   		delay_us(1);
		if(MISO==0)
			byte&=0xfe;
		else
			byte|=0x01;
		SCK = 0;            	// SCK置低
		delay_us(1);
   	}
    return(byte);           	// 返回读出的一字节
}
/**************************************************/

/**************************************************
函数：SPI_RW_Reg()

描述：
    写数据value到reg寄存器
**************************************************/
vu8 SPI_RW_Reg(vu8 reg, vu8 value)
{
	vu8 status;
  	CSN = 0;                   // CSN置低，开始传输数据
  	status = SPI_RW(reg);      // 选择寄存器，同时返回状态字
  	SPI_RW(value);             // 然后写数据到该寄存器
  	CSN = 1;                   // CSN拉高，结束数据传输
  	return(status);            // 返回状态寄存器
}
/**************************************************/

/**************************************************
函数：SPI_Read()

描述：
    从reg寄存器读一字节
**************************************************/
vu8 SPI_Read(vu8 reg)
{
	vu8 reg_val;
  	CSN = 0;                    // CSN置低，开始传输数据
  	SPI_RW(reg);                // 选择寄存器
  	reg_val = SPI_RW(0);        // 然后从该寄存器读数据
  	CSN = 1;                    // CSN拉高，结束数据传输
  	return(reg_val);            // 返回寄存器数据
}
/**************************************************/

/**************************************************
函数：SPI_Read_Buf()

描述：
    从reg寄存器读出bytes个字节，通常用来读取接收通道
	数据或接收/发送地址
**************************************************/
vu8 SPI_Read_Buf(vu8 reg, vu8 * pBuf, vu8 bytes)
{
	vu8 status, i;
  	CSN = 0;                    // CSN置低，开始传输数据
  	status = SPI_RW(reg);       // 选择寄存器，同时返回状态字
  	for(i=0; i<bytes; i++)
    	pBuf[i] = SPI_RW(0);    // 逐个字节从nRF24L01读出
  	CSN = 1;                    // CSN拉高，结束数据传输
  	return(status);             // 返回状态寄存器
}
/**************************************************/

/**************************************************
函数：SPI_Write_Buf()

描述：
    把pBuf缓存中的数据写入到nRF24L01，通常用来写入发
	射通道数据或接收/发送地址
**************************************************/
vu8 SPI_Write_Buf(vu8 reg, vu8 * pBuf, vu8 bytes)
{
	vu8 status, i;
  	CSN = 0;                    // CSN置低，开始传输数据
  	status = SPI_RW(reg);       // 选择寄存器，同时返回状态字
  	for(i=0; i<bytes; i++)
    	SPI_RW(pBuf[i]);        // 逐个字节写入nRF24L01
  	CSN = 1;                    // CSN拉高，结束数据传输
  	return(status);             // 返回状态寄存器
}
/**************************************************/
vu8 kal;
/**************************************************



函数：RX_Mode()

描述：
    这个函数设置nRF24L01为接收模式，等待接收发送设备的数据包
**************************************************/
void RX_Mode(void)
{
	CE=0;
	SPI_Write_Buf(WRITE_REG + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH);//?????0?????,???????
//	SPI_Write_Buf(WRITE_REG + RX_ADDR_P1, RX_ADDRESS1, TX_ADR_WIDTH); //?????1?????
//	SPI_Write_Buf(WRITE_REG + RX_ADDR_P2, RX_ADDRESS2, TX_ADR_WIDTH);//?????2?????
//	SPI_Write_Buf(WRITE_REG + RX_ADDR_P3, RX_ADDRESS3, TX_ADR_WIDTH);//?????3?????
//	SPI_Write_Buf(WRITE_REG + RX_ADDR_P4, RX_ADDRESS4, TX_ADR_WIDTH);//?????4?????
//	SPI_Write_Buf(WRITE_REG + RX_ADDR_P5, RX_ADDRESS2, TX_ADR_WIDTH);//?????5?????
//	SPI_RW_Reg(WRITE_REG + RX_PW_P0, TX_PLOAD_WIDTH);  // ????0???????????????,32????
//  	SPI_RW_Reg(WRITE_REG + RX_PW_P1, TX_PLOAD_WIDTH);//32????
//  	SPI_RW_Reg(WRITE_REG + RX_PW_P2, TX_PLOAD_WIDTH);//32????
//  	SPI_RW_Reg(WRITE_REG + RX_PW_P3, TX_PLOAD_WIDTH);//32????
//  	SPI_RW_Reg(WRITE_REG + RX_PW_P4, TX_PLOAD_WIDTH);//32????
//  	SPI_RW_Reg(WRITE_REG + RX_PW_P5, TX_PLOAD_WIDTH);//32????
  	SPI_RW_Reg(WRITE_REG + EN_AA, 0x1);       // ????????????
  	SPI_RW_Reg(WRITE_REG + EN_RXADDR, 0x1);   // ????????
	SPI_RW_Reg(WRITE_REG + RF_CH, 40);                 // ??????0x40
    SPI_RW_Reg(WRITE_REG + RX_PW_P0, TX_PLOAD_WIDTH);  // 接收通道0选择和发送通道相同有效数据宽度
	SPI_RW_Reg(WRITE_REG + RF_SETUP, 0x07);            // ?????1Mbps,????0dBm,????????
	SPI_RW_Reg(WRITE_REG + N2401_CONFIG, 0x0F);      // CRC??,16?CRC??,??
//	SPI_RW_Reg(WRITE_REG + SETUP_RETR, 0x0a);  // ????????250us+86us,????10?
	CE=1;                                           // 拉高CE启动接收设备

}
/**************************************************/

void Check_ACK(vu8 clear)
{
	while(IRQ);
	sta = SPI_RW(NOP);                    // 返回状态寄存器
	if(MAX_RT)                    // 是否清除TX FIFO，没有清除在复位MAX_RT中断标志后重发
		if(clear)
			SPI_RW(FLUSH_TX);
	SPI_RW_Reg(WRITE_REG + STATUS, sta);  // 清除TX_DS或MAX_RT中断标志
	IRQ=1;
	if(TX_DS)
	{
//		return(0x00);
	}
//	else
//		return(0xff);
}






vu8 ig=0;


/**************************************************
函数：TX_Mode()

描述：
    这个函数设置nRF24L01为发送模式，（CE=1持续至少10us），
	130us后启动发射，数据发送结束后，发送模块自动转入接收
	模式等待应答信号。
**************************************************/
void TX_Mode(vu8 * BUF)
{
			vu8 FJNum=1;
	CE = 0;

//	if (FJNum==0)
//    {
//        SPI_Write_Buf(WRITE_REG + TX_ADDR, RX_ADDRESS1, TX_ADR_WIDTH);     // 写入发送地址
//        SPI_Write_Buf(WRITE_REG + RX_ADDR_P0, RX_ADDRESS1, TX_ADR_WIDTH);  // 为了应答接收设备，接收通道0地址和发送地址相同
//    }
//    else if (FJNum==1)
//    {
        SPI_Write_Buf(WRITE_REG + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH);     // 写入发送地址
        SPI_Write_Buf(WRITE_REG + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH);  // 为了应答接收设备，接收通道0地址和发送地址相同
//    }
//    else if (FJNum==2)
//    {
//        SPI_Write_Buf(WRITE_REG + TX_ADDR, RX_ADDRESS3, TX_ADR_WIDTH);     // 写入发送地址
//        SPI_Write_Buf(WRITE_REG + RX_ADDR_P0, RX_ADDRESS3, TX_ADR_WIDTH);  // 为了应答接收设备，接收通道0地址和发送地址相同
//    }
//    else if (FJNum==3)
//    {
//        SPI_Write_Buf(WRITE_REG + TX_ADDR, RX_ADDRESS4, TX_ADR_WIDTH);     // 写入发送地址
//        SPI_Write_Buf(WRITE_REG + RX_ADDR_P0, RX_ADDRESS4, TX_ADR_WIDTH);  // 为了应答接收设备，接收通道0地址和发送地址相同
//    }
//    else if (FJNum==4)
//    {
//        SPI_Write_Buf(WRITE_REG + TX_ADDR, RX_ADDRESS5, TX_ADR_WIDTH);     // 写入发送地址
//        SPI_Write_Buf(WRITE_REG + RX_ADDR_P0, RX_ADDRESS5, TX_ADR_WIDTH);  // 为了应答接收设备，接收通道0地址和发送地址相同
//    }
    
        SPI_Write_Buf(WR_TX_PLOAD, BUF, TX_PLOAD_WIDTH);                  // 写数据包到TX FIFO
  	SPI_RW_Reg(WRITE_REG + EN_AA, 0x01);       // 使能接收通道0自动应答
  	SPI_RW_Reg(WRITE_REG + EN_RXADDR, 0x01);   // 使能接收通道0
  	SPI_RW_Reg(WRITE_REG + SETUP_RETR, 0x0a);  // 自动重发延时等待250us+86us，自动重发10次
  	SPI_RW_Reg(WRITE_REG + RF_CH, 40);         // 选择射频通道0x40
  	SPI_RW_Reg(WRITE_REG + RF_SETUP, 0x07);    // 数据传输率1Mbps，发射功率0dBm，低噪声放大器增益
  	SPI_RW_Reg(WRITE_REG + N2401_CONFIG, 0x0e);      // CRC使能，16位CRC校验，上电
	CE = 1;
}

	
	vu8 NRF24L01_RxPacket(vu8 *rx_buf)
{
  vu8 revale=0;
  SPI_RW_Reg(WRITE_REG + N2401_CONFIG,0X0F);
  CE=1;
  delay_ms(10);
  sta=SPI_Read(STATUS);
  if(RX_DR)
  {
  	CE=0;
	SPI_Read_Buf(RD_RX_PLOAD,rx_buf,TX_PLOAD_WIDTH);
    revale=1; 
  }
 SPI_RW_Reg(WRITE_REG + STATUS,sta);
 return revale;
} 


