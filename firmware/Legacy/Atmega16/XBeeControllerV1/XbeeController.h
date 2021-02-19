


#define XBEE_DEST_ADDR 			"ATDH0, DL1234\r"
#define XBEE_MY_ADDR			"ATMY5678\r"

#define XBEE_PAN_ID				"ATID1111\r"
#define XBEE_DATA_MODE			"ATCN\r"
#define XBEE_CMD_MODE_BEGIN		"+++"
#define XBEE_CMD_MODE_END		"AT"

unsigned short xbSend(char* str);
unsigned short xbRecv(char* str);
unsigned short xbSendData(char cData);

unsigned short xbDecodeCmd(void);
unsigned short xbRepondCmd(void);

