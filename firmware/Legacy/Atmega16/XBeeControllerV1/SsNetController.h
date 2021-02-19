
//*****************************************
// Definitions for Main Task
//*****************************************
#define TASK_IDLE				0
#define TASK_RX_PREP			1
#define TASK_RX_WAIT			2
#define TASK_MSG_ORGANIZE		3
#define	TASK_RESP_MSG			4

//*****************************************
// Definitions for Protocol errors
//*****************************************
#define	ERR_SSNET_NO_DATA			1
#define	ERR_SSNET_TIMEOUT_MSG		2
#define	ERR_SSNET_CHECKSUM_INVALID	3

#define	SET							1
#define	RESET						0

#define	CHECKSUM_OK					1
#define	CHECKSUM_ERR				0

#define	TIMEOUT_BYTE				100

void			SsNetInitTask			(void);
void			SsNetMainTask			(void);
unsigned char	SsNetIsCheckSumValid	(void);
void			SsNetDispMsg			(void);
void			SsNetCtrlRxTimeout		(unsigned char	ucCtrl);
unsigned char	SsNetGetRxTimeout		(void);

void			SsNetSetNextTask		(unsigned char	ucState);
