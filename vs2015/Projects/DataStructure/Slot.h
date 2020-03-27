#pragma once

#ifndef _SLOT_
#define _SLOT_			1

//#define USE_CHAINNING	1

#include "Person.h"

typedef int Key;
typedef Person	*Value;

typedef enum SlotStatus
{
	EMPTY,
	DELETED,
	INUSE

}eSlotStatus;

typedef struct _slot
{
	Key			key;
	Value		val;
#ifndef USE_CHAINNING
	eSlotStatus	status;		// if Slot is to be used in channing, status isn't required.
#endif
}Slot;

#endif
