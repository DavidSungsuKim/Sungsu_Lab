

#define PORT_DOTMATRIX_ROW	PORTE
#define PORT_DOTMATRIX_COL	PORTB

//*******************************
// UART Part Definition
//*******************************
#define BAUD_RATE			9600
#define ERR_UART_TIME_OUT	100

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

void InitCPU();

void InitCMT0();
void InitCMT2();
void InitUART0();
unsigned short SendUART0(char cData);
unsigned short RecvUART0(char* cData);
