/*
 * Scheduler.h
 *
 *  Created on: Feb 10, 2019
 *      Author: AhmedShaban
 */

#ifndef SCHEDULER_H_
#define SCHEDULER_H_


extern FUNC(void,CODE_FLASH_CLASS)		Scheduler_vidInit(void);
extern FUNC(void,CODE_FLASH_CLASS)		Scheduler_vidStart(void);
extern FUNC(void,CODE_FLASH_CLASS)		Scheduler_vidUpdate(void);
extern FUNC(void,CODE_FLASH_CLASS)		Scheduler_vidDispatcher(void);
extern FUNC(uint8,CODE_FLASH_CLASS)		Scheduler_u8AddTask(void(*pfTaskFunctionPtr)(void),
															 VAR(uint16,AUTOMATIC)	u16Delay,
															 VAR(uint16,AUTMATIC)	u16Period);

extern FUNC(uint8,CODE_FLASH_CLASS)		Scheduler_u8DeleteTask(CONST(uint8,AUTOMATIC) ku8Index);
extern FUNC(void,CODE_FLASH_CLASS)		Scheduler_vidReportStatus(void);


#endif /* 07_SCHEDULER_SCHEDULER_H_ */
