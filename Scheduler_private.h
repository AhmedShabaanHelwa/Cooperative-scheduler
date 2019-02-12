/*
 * Scheduler_private.h
 *
 *  Created on: Feb 10, 2019
 *      Author: AhmedShaban
 */

#ifndef SCHEDULER_PRIVATE_H_
#define SCHEDULER_PRIVATE_H_

typedef struct{
	/* 1- Task pointer */
	void (*pfTask)(void);
	/* 2- Task delay */
	uint16	u16Delay;
	/* 3- Task periodicity */
	uint16 u16Period;
	/* 4- Run Flag */
	uint8	u8Run;
}Scheduler_tstrTask;

#endif /* 07_SCHEDULER_SCHEDULER_PRIVATE_H_ */
