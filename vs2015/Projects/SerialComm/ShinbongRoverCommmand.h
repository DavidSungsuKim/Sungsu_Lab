#pragma once

#define _ASCII(x)		((char)(x + 48))
#define _NUM10TH(x)		( x / 10 )
#define _NUM1TH(x)		( x % 10 )

enum eDataGram
{
	eSize		= 16
};

enum eDataGramIndex
{
	eStart		= 0,
	eCmd		= 1,
	eData1		= 2,
	eData2		= 3,
	eData3		= 4,
	eData4		= 5,
	eData5		= 6,
	eData6		= 7,
	eData7		= 8,
	eData8		= 9,
	eData9		= 10,
	eData10		= 11,
	eData11		= 12,
	eData12		= 13,
	eData13		= 14,
	eData14		= 15,
	eData15		= 16,
	eEnd		= 16
};

enum eDataChar
{
	eCharStart		= 'X'
};

enum eCmdList
{
	eCmdTest		= 'A',
	eCmdLED2		= '1',
	eCmdLED3		= '2',
	eCmdPWM1		= '3',
	eCmdChangeDir	= '4',
	eCmdChangeSpeed	= '5'
};

enum ePWM1Channel
{
	eChanPWM1 = 1,
	eChanPWM2 = 2,
	eChanPWM3 = 3
};

typedef struct stMsg
{
	char data[eSize];

	stMsg()
	{
		memset(data, _ASCII(0), sizeof(data));
		data[eStart] = eCharStart;
	}

}StMsg;

class CRoverCmd
{
public:
	CRoverCmd(int x);
	~CRoverCmd();
};