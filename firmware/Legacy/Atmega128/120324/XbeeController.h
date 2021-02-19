


#define XBEE_DEST_ADDR 		"ATDH0, DL1234\r"
#define XBEE_MY_ADDR		"ATMY5678\r"

#define XBEE_PAN_ID			"ATID1111\r"
#define XBEE_DATA_MODE		"ATCN\r"
#define XBEE_CMD_MODE		"+++"


unsigned short xbSend(char* str);
unsigned short xbRecv(char* str);

