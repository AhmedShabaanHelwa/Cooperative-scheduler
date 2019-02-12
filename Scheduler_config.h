/*
 * Scheduler_config.h
 *
 *  Created on: Feb 10, 2019
 *      Author: AhmedShaban
 */

#ifndef SCHEDULER_CONFIG_H_
#define SCHEDULER_CONFIG_H_
/* !Pseudo: Include the General Purpose Timer header here */
#include "../02_MCAL/05_SYSTICK/SysTick.h"
/* !Pseudo: Include the Global interrupt header here */
#include "../02_MCAL/02_GI/GI.h"

/* !Pseudo: Configure the maximum number of tasks */
#define Scheduler_u8MAX_NUM_TASKS							1
/* !Pseudo: Configure General Purpose Timer initialization function  */
#define Scheduler_vidGptInitialization()				SysTick_vidInit()
/* !Pseudo: Configure the General Purpose Timer callback setting function */
#define Scheduler_pfGptCallBack					        SysTick_vidSetCallBack

#define Scheduler_vidGptInterruptEnable()				SysTick_vidInterruptEnable()
#define Scheduler_vidGptInterruptDisable()				SysTick_vidInterruptDisable()
/* !Pseudo: Configure the Global interrupt enable function */
#define Scheduler_vidEnableInterrupts()					//GI_vidEnable()
#define Scheduler_vidDisableInterrupts()				//GI_vidEnable()

#endif /* 07_SCHEDULER_SCHEDULER_CONFIG_H_ */
