

#define F_CPU 		16000000UL

//*******************************
// General Definition
//*******************************
#define TRUE				1
#define FALSE				0
#define OK					0
#define ERROR				1

//*******************************
// UART Control
//*******************************
#define BAUD_RATE				9600
#define ERR_UART_SEND_TIME_OUT	10000
#define ERR_UART_RECV_TIME_OUT	10000
#define TX_COMPLETE				0x40

//*******************************
// PortA LED Control
//*******************************
#define PORTA_ASSIGNED_LED	0x0F
#define ON					0x01
#define OFF					0x02
#define LED1				0x01
#define LED2				0x02
#define LED3				0x04
#define LED4				0x08

//*******************************
// Ring Buffer Control
//*******************************
#define	SIZE_RING_BUFF		64
#define REMAINED			0x0001
#define FULL				0x0000

/*
// UCSRA bit
#define RXC					0x80
#define TXC					0x40
#define UDRE				0x20
#define FE					0x10
#define DOR					0x08
#define UPE					0x04
#define U2X					0x02
#define MPC					0x01

// UCSRB
#define RXCIE				0x80
#define TXCIE				0x40
#define UDRIE				0x20
#define RXEN				0x10
#define TXEN				0x08
#define UCSZ2				0x04
#define RXB8				0x02
#define TXB8				0x01

// UCSRC
#define RESERVED			0x80
#define UMSEL				0x40
#define UPM1				0x20
#define UPM0				0x10
#define USBS				0x08
#define UCSZ1				0x04
#define UCSZ0				0x02
#define UCPOL				0x01
*/

typedef struct RingBuffer
{
	unsigned char	head;
	unsigned char	tail;
	unsigned char	num;
	unsigned char	full;
	unsigned char	buffer[SIZE_RING_BUFF];
}RingBuff;

void 			InitCPU			();
void 			InitCMT0		();
void 			InitCMT2		();
void 			InitUART0		();
unsigned short 	SendUART		(char* cData);
unsigned short 	RecvUART		(char* cData);
unsigned short 	CtrlPortALED	(unsigned char state, unsigned char numLed);
void			ToggleLED		(unsigned char numLed);

void 			InitRingBuff	(RingBuff* buff);
void 			PushRingBuff	(RingBuff* buff, unsigned char data);
unsigned short 	PopRingBuff		(RingBuff* buff, unsigned char* data);
