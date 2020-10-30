/*
*********************************************************************************************************
*                                              EXAMPLE CODE
*
*                          (c) Copyright 2003-2013; Micrium, Inc.; Weston, FL
*
*               All rights reserved.  Protected by international copyright laws.
*               Knowledge of the source code may NOT be used to develop a similar product.
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                       uC/OS-III Application Code
*                                                Template
*
* The template application code provided here can be used as a foundation for your own uC/OS-III-based
* projects.  This file, app_cfg.h, is normally where an application's task priorities and stack sizes
* are defined.
*
* Filename      : app_cfg.h
* Version       : V1.00
*********************************************************************************************************
*/

#ifndef  __APP_CFG_H__
#define  __APP_CFG_H__

#include  <lib_cfg.h>

/*
*********************************************************************************************************
*                                            TASK PRIORITIES
*********************************************************************************************************
*/

#define  APP_TASK_START_PRIO                        4u

#define  APP_CFG_TASK1_PRIO                         5u         
#define  APP_CFG_TASK2_PRIO                         6u         
#define  APP_CFG_TASK3_PRIO                         7u         
#define  APP_CFG_TASK4_PRIO                         8u         

/*
*********************************************************************************************************
*                                            TASK STACK SIZES
*                             Size of the task stacks (# of OS_STK entries)
*********************************************************************************************************
*/

#define  APP_TASK_START_STK_SIZE                    256u

#define  APP_CFG_TASK1_STK_SIZE                     512u
#define  APP_CFG_TASK2_STK_SIZE                     512u
#define  APP_CFG_TASK3_STK_SIZE                     512u
#define  APP_CFG_TASK4_STK_SIZE                     512u


#endif
