/***************************************************************************************
 * 描述    ：W5500的端口0工作在客户端模式,主动与《TCP&UDP测试工具》上创建的服务端连接,
 *			 并且定时给服务端发送字符串"\r\nWelcome To YiXinElec!\r\n",同时将接
 *			 收到服务端发来的数据回发给服务端。
 * 实验平台：用户51单片机开发板 + YIXIN_W5500以太网(TCP/IP)模块
 * 硬件连接：  P3^2 -> W5500_RST   
 *             P3^3 -> W5500_INT(本例程没有用到中断,此引脚可以不接)     
 *             P3^6 -> W5500_SCS      
 *             P3^7 -> W5500_SCK    
 *             P3^4 -> W5500_MISO    
 *             P3^5 -> W5500_MOSI    

 * 淘宝    ：http://yixindianzikeji.taobao.com/
***************************************************************************************/

/*例程网络参数*/
//网关：192.168.1.1
//掩码:	255.255.255.0
//物理地址：0C 29 AB 7C 00 01
//本机IP地址:192.168.1.199
//端口0的端口号：5000
//端口0的目的IP地址：192.168.1.190
//端口0的目的端口号：6000

#include <reg51.h>
#include "W5500.h"				
#include <string.h>

void Delay(unsigned int d);			//延时函数(ms)

/*******************************************************************************
* 函数名  : W5500_Initialization
* 描述    : W5500初始货配置
* 输入    : 无
* 输出    : 无
* 返回值  : 无
* 说明    : 无
*******************************************************************************/
void W5500_Initialization(void)
{
	W5500_Init();		//初始化W5500寄存器函数
	Detect_Gateway();	//检查网关服务器 
	Socket_Init(0);		//指定Socket(0~7)初始化,初始化端口0
}

/*******************************************************************************
* 函数名  : Load_Net_Parameters
* 描述    : 装载网络参数
* 输入    : 无
* 输出    : 无
* 返回值  : 无
* 说明    : 网关、掩码、物理地址、本机IP地址、端口号、目的IP地址、目的端口号、端口工作模式
*******************************************************************************/
void Load_Net_Parameters(void)
{
	Gateway_IP[0] = 192;//加载网关参数
	Gateway_IP[1] = 168;
	Gateway_IP[2] = 1;
	Gateway_IP[3] = 1;

	Sub_Mask[0]=255;//加载子网掩码
	Sub_Mask[1]=255;
	Sub_Mask[2]=255;
	Sub_Mask[3]=0;

	Phy_Addr[0]=0x0c;//加载物理地址
	Phy_Addr[1]=0x29;
	Phy_Addr[2]=0xab;
	Phy_Addr[3]=0x7c;
	Phy_Addr[4]=0x00;
	Phy_Addr[5]=0x01;

	IP_Addr[0]=192;//加载本机IP地址
	IP_Addr[1]=168;
	IP_Addr[2]=1;
	IP_Addr[3]=199;

	S0_Port[0] = 0x13;//加载端口0的端口号5000 
	S0_Port[1] = 0x88;

	S0_DIP[0]=192;//加载端口0的目的IP地址
	S0_DIP[1]=168;
	S0_DIP[2]=1;
	S0_DIP[3]=190;
	
	S0_DPort[0] = 0x17;//加载端口0的目的端口号6000
	S0_DPort[1] = 0x70;

	S0_Mode=TCP_CLIENT;//加载端口0的工作模式,TCP客户端模式
}

/*******************************************************************************
* 函数名  : W5500_Socket_Set
* 描述    : W5500端口初始化配置
* 输入    : 无
* 输出    : 无
* 返回值  : 无
* 说明    : 分别设置4个端口,根据端口工作模式,将端口置于TCP服务器、TCP客户端或UDP模式.
*			从端口状态字节Socket_State可以判断端口的工作情况
*******************************************************************************/
void W5500_Socket_Set(void)
{
	if(S0_State==0)//端口0初始化配置
	{
		if(S0_Mode==TCP_SERVER)//TCP服务器模式 
		{
			if(Socket_Listen(0)==TRUE)
				S0_State=S_INIT;
			else
				S0_State=0;
		}
		else if(S0_Mode==TCP_CLIENT)//TCP客户端模式 
		{
			if(Socket_Connect(0)==TRUE)
				S0_State=S_INIT;
			else
				S0_State=0;
		}
		else//UDP模式 
		{
			if(Socket_UDP(0)==TRUE)
				S0_State=S_INIT|S_CONN;
			else
				S0_State=0;
		}
	}
}

/*******************************************************************************
* 函数名  : Process_Socket_Data
* 描述    : W5500接收并发送接收到的数据
* 输入    : s:端口号
* 输出    : 无
* 返回值  : 无
* 说明    : 本过程先调用S_rx_process()从W5500的端口接收数据缓冲区读取数据,
*			然后将读取的数据从Rx_Buffer拷贝到Temp_Buffer缓冲区进行处理。
*			处理完毕，将数据从Temp_Buffer拷贝到Tx_Buffer缓冲区。调用S_tx_process()
*			发送数据。
*******************************************************************************/
void Process_Socket_Data(SOCKET s)
{
	unsigned short size;
	size=Read_SOCK_Data_Buffer(s, Rx_Buffer);
	memcpy(Tx_Buffer, Rx_Buffer, size);			
	Write_SOCK_Data_Buffer(s, Tx_Buffer, size);
}

/*******************************************************************************
* 函数名  : main
* 描述    : 主函数，用户程序从main函数开始运行
* 输入    : 无
* 输出    : 无
* 返回值  : int:返回值为一个16位整形数
* 说明    : 无
*******************************************************************************/
int main(void)
{
	unsigned int W5500_Send_Delay_Counter =0;

	Load_Net_Parameters();		//装载网络参数	
	W5500_Hardware_Reset();		//硬件复位W5500
	W5500_Initialization();		//W5500初始货配置
	while (1)
	{
		W5500_Socket_Set();//W5500端口初始化配置
		
		W5500_Interrupt_Process();//W5500中断处理程序框架

		if((S0_Data & S_RECEIVE) == S_RECEIVE)//如果Socket0接收到数据
		{
			S0_Data&=~S_RECEIVE;
			Process_Socket_Data(0);//W5500接收并发送接收到的数据
		}
		else if(W5500_Send_Delay_Counter >= 3000)//定时发送字符串
		{
			if(S0_State == (S_INIT|S_CONN))
			{
				S0_Data&=~S_TRANSMITOK;
				memcpy(Tx_Buffer, "\r\nWelcome To YiXinElec!\r\n", 23);	
				Write_SOCK_Data_Buffer(0, Tx_Buffer, 23);//指定Socket(0~7)发送数据处理,端口0发送23字节数据
			}
			W5500_Send_Delay_Counter=0;
		}
		W5500_Send_Delay_Counter++;
	}
}

/*******************************************************************************
* 函数名  : Delay
* 描述    : 延时函数(ms)
* 输入    : d:延时系数，单位为毫秒
* 输出    : 无
* 返回    : 无 
* 说明    : 延时是利用Timer2定时器产生的1毫秒的计数来实现的
*******************************************************************************/
void Delay(unsigned int  x)
{
    unsigned int i,j;

    for(j=0;j<5;j++)
		for(i=0;i<x;i++);
}
