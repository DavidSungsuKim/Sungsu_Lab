
#ifndef _ERROR_CODE_H_
#define _ERROR_CODE_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
	eOK	= 0,
	eERR_STM32_DRIVER,

	eERR_PWM_INVALID_CHAN

}eStatus;

#ifdef __cplusplus
}
#endif

#endif /* _ERROR_CODE_H_ */
