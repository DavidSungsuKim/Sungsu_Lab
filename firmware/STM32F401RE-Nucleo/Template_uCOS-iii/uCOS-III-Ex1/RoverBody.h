
//***********************************************************
//      STM32F401-Nucleo Personal Application
// File name    : RoverBody.h
// Designer     : Sungsu Kim
// Email        : sheld2@naver.com
// All rights reserved
// 
//***********************************************************

#ifndef _ROVER_BODY_H_
#define _ROVER_BODY_H_          1

#define HS311_NL_DEG             (-45)
#define HS311_PL_DEG             (45)
#define HS311_RANGE_DEG          (90)
#define HS311_NL_PULSE_WIDTH_SEC (0.001) 
#define HS311_PL_PULSE_WIDTH_SEC (0.002) 
#define HS311_PULSE_RANGE_SEC    (0.001) 
#define HS311_RESOLUTION_SEC     (HS311_PULSE_RANGE_SEC / HS311_RANGE_DEG)

typedef struct stRoberConfig
{
  int32_t       configured;
}RoberConfig;

typedef struct stRoberStatus
{
  int32_t       speedPercent;
}RoberStatus;

int     RbInitBody              (void);
int     RbChangeDirection       (int32_t percent);
int     RbChangeSpeed           (int32_t percent);  
int     RbBrakeABS              ();

#endif