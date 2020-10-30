
#include <includes.h>
#include "RoverBody.h"
#include "UartController.h"

uint32_t GetDuty        (int32_t deg);

RoberConfig             g_roberConfig;
RoberStatus             g_roberStatus;

////////////////////////////////////////////////////////////////////////////////
// Global Functions
int RbInitBody (void)
{
  g_roberConfig.configured = 0;
  g_roberStatus.speedPercent = 0;
  return 0;
}

int RbChangeDirection (int32_t percent)
{
  double speedPercent = (double)g_roberStatus.speedPercent;

  if ( speedPercent <= 0. )
    return -1;
 
  double speedPercentLeft;
  double speedPercentRight; 
  
  if ( percent == 0 )
  {
    speedPercentLeft    = speedPercent;
    speedPercentRight   = speedPercent;
  }
  else if ( percent < 0 )
  {
    speedPercentLeft    = ( 100. + (double)percent ) / speedPercent * 100.;
    if ( speedPercentLeft < 1. ) 
      speedPercentLeft = 1.;
    speedPercentRight   = speedPercent;
  }
  else
  {
    speedPercentRight   = ( 100. - (double)percent ) / speedPercent * 100.; 
    if ( speedPercentRight < 1 ) 
      speedPercentRight = 1;
    speedPercentLeft    = speedPercent;
  }

  printf("Rb: left percent=%.f\r\n", speedPercentLeft); 
  printf("Rb: right percent=%.f\r\n", speedPercentRight);   
  
  int32_t percentLeft   = (uint32_t)speedPercentLeft;
  int32_t percentRight  = (uint32_t)speedPercentRight;

  BSP_PWM1( (uint32_t)PWM1_CHAN2, (uint32_t)percentLeft ); 
  BSP_PWM1( (uint32_t)PWM1_CHAN3, (uint32_t)percentRight ); 
  
  return 0;
}

int RbChangeSpeed (int32_t  percent )
{
  if ( percent < 1 )    percent = 1;
  if ( percent > 99 )   percent = 99;
  
  BSP_PWM1( (uint32_t)PWM1_CHAN2, (uint32_t)percent ); 
  BSP_PWM1( (uint32_t)PWM1_CHAN3, (uint32_t)percent ); 
  
  g_roberStatus.speedPercent = percent;
  return 0;
}

int RbBrakeABS ()
{
  
  return 0;
}

////////////////////////////////////////////////////////////////////////////////
// Local functions
uint32_t GetDuty (int32_t deg)
{
  if ( deg < HS311_NL_DEG )      deg = HS311_NL_DEG;
  if ( deg > HS311_PL_DEG)       deg = HS311_PL_DEG;

  int32_t degDiff       = deg - HS311_NL_DEG;
   
  double pulseWidth     = degDiff * HS311_RESOLUTION_SEC + HS311_NL_PULSE_WIDTH_SEC;
  double periodSec      = BSP_PWM1GetPeriodSec();  
    
  uint32_t pulseDuty    = (uint32_t)(pulseWidth / periodSec * 100);
  printf("GetDuty:pulseWidth=%.5f, periodSec=%.5f\r\n", pulseWidth, periodSec);
  printf("GetDuty:deg=%d, duty=%d\r\n", deg, pulseDuty);
   
  return pulseDuty;
}
               