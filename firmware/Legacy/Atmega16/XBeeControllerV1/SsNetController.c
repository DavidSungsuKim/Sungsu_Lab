#include "SsNetController.h"
#include "Cpu.h"
#include "XbeeController.h"

unsigned char	g_ucSsNetTaskState 	= TASK_IDLE;
unsigned char	g_ucProtocolErr		= 0x00;
unsigned char	g_ucMsgLength		= 0x00;
unsigned char	g_ucRxTimeout		= RESET;

unsigned short	g_usPacketTick		= 0;
unsigned short	g_usPacketTimeLimit	= TIMEOUT_BYTE;
unsigned short	g_usMsgTick			= 0;
unsigned short	g_usMsgTimeLimit	= 0;

unsigned short	g_usTest			= 0;

extern RingBuff 	g_SendBuff;

void SsNetInitTask(void)
{

}

void SsNetMainTask(void)
{
	unsigned char*	pNumBuf;

	switch(g_ucSsNetTaskState)
	{
		case TASK_IDLE:
			ToggleLED(LED1);
			
			if(g_SendBuff.num)
				SsNetSetNextTask(TASK_RX_WAIT);

			break;

		case TASK_RX_PREP:
			InitRingBuff(&g_SendBuff);

			SsNetSetNextTask(TASK_IDLE);
			break;

		case TASK_RX_WAIT:

			g_ucMsgLength = g_SendBuff.buffer[0];

			if(g_ucMsgLength > 0)
			{
				g_usMsgTimeLimit 	= TIMEOUT_BYTE * g_SendBuff.buffer[0];
				g_usMsgTick			= 0x0000;

				SsNetSetNextTask(TASK_MSG_ORGANIZE);
			}
			else
			{
				g_ucProtocolErr		= ERR_SSNET_NO_DATA;
				SsNetSetNextTask(TASK_RESP_MSG);
			}
				
			break;

		case TASK_MSG_ORGANIZE:

			pNumBuf = (unsigned char*)&(g_SendBuff.num);
	
			while(g_ucMsgLength > *pNumBuf)
			{
				//****test***********
				SendUART("C");
				//*******************
				if(g_usMsgTick > g_usMsgTimeLimit)
				{
					g_ucProtocolErr		= ERR_SSNET_TIMEOUT_MSG;
					SsNetSetNextTask(TASK_RESP_MSG);
					
					return;
				}
			}

			if(!SsNetIsCheckSumValid())
			{
				g_ucProtocolErr		= ERR_SSNET_CHECKSUM_INVALID;
				SsNetSetNextTask(TASK_RESP_MSG);
					
				return;
			}
			else
			{
				SsNetDispMsg();
				SsNetCtrlRxTimeout(SET);
			}
		
			SsNetSetNextTask(TASK_RESP_MSG);
			
			break;

		case TASK_RESP_MSG:
			
			g_usTest++;

			if(g_ucProtocolErr)
				SendUART("A");//xbSend("\rMsgErr\r");
			else
				SendUART("B");//xbSend("\rMsgOk\r");	
			
			SsNetSetNextTask(TASK_RX_PREP);

			break;
			
		default:

			break;
	}
}

unsigned char SsNetIsCheckSumValid(void)
{	
	unsigned char	ucCheckSum	= 0x00;
	unsigned char	i;	
	unsigned char	ucLastData	= 0x00;

	RingBuff*	ptr	= &g_SendBuff;

	for(i = 0; i < (ptr->num - 1);	i++)
	{
		ucCheckSum += ptr->buffer[i];
	}
	
	ucLastData = ptr->buffer[(ptr->num)-1];

	if(ucCheckSum == ucLastData)
		return CHECKSUM_OK;
	else
		return CHECKSUM_ERR;
}

void SsNetDispMsg(void)
{
	return;
}

void SsNetCtrlRxTimeout	(unsigned char	ucCtrl)
{
	g_ucRxTimeout = ucCtrl;
}

unsigned char SsNetGetRxTimeout(void)
{
	return (g_ucRxTimeout);
}

void SsNetSetNextTask(unsigned char	ucState)
{
	g_ucSsNetTaskState = ucState;
}
