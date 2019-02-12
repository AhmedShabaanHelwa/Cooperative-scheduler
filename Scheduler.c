/*
 * Scheduler.c
 *
 *  Created on: Feb 10, 2019
 *      Author: AhmedShaban
 */
/* 1- Compiler-specific libraries */
/* 2- Generic libraries */
#include "../01_LIB/STD_TYPES.h"
#include "../01_LIB/BIT_MATH.h"
#include "../05_COMMON/02_STDTYPES/Compiler.h"
/* 3- Global modules */
#include "../02_MCAL/01_DIO/DIO.h"   /* !Comment: for test purpose only*/
/* 4- Module inclusion */
#include "Scheduler_private.h"
#include "Scheduler_config.h"
#include "Scheduler.h"
/* 5- Local definitions and macros */
//#define Scheduler_vidSetISR()					//Scheduler_pfGptCallBack##(##Scheduler_vidUpdate##)
/* 6- Global(exported) variables/functions */
VAR(uint8, DATA_RAM_CLASS)	Error_eu8Code;
/* 7- Static (private) variables/definitions */
static VAR(Scheduler_tstrTask,AUTOMATIC)		Scheduler_astrTasks[Scheduler_u8MAX_NUM_TASKS] = {{0}};
static FUNC(void, CODE_FLASH_CLASS)				Scheduler_vidSetISR(void);
/* !Comment: This function is 8051 architecture related,
 * !Comment: to find analogy later */
//static FUNC(void,CODE_FLASH_CLASS)	Scheduler_vidGoToSleep(void);



FUNC(void,CODE_FLASH_CLASS)		Scheduler_vidInit(void)
{
	/* !Comment: Local counter variable */
	VAR(uint8,AUTOMATIC)	u8LocalIndex;

	/* !Pseudo: Deleting tasks */
	for(u8LocalIndex = 0; u8LocalIndex < Scheduler_u8MAX_NUM_TASKS; u8LocalIndex++)
	{
		Scheduler_u8DeleteTask(u8LocalIndex);
	}
	/* !Pseudo: Reset global error variable */
	/* !Comment: the above deletion shall produce an error
	 * !Comment: since the tasks array is initially empty */
	Error_eu8Code = 0;
	/* !Pseudo: Set global interrupt bit variable */
	/* !Pseudo: Set up the General purpose timer for scheduling */
	/* !Comment: the scheduler is operating by one-mSec Tick interval */
	Scheduler_vidGptInitialization();

	Scheduler_vidSetISR();

	Scheduler_vidStart();
}/* !Comment: End of initialization function */


FUNC(void,CODE_FLASH_CLASS)		Scheduler_vidStart(void)
{
	/* !Pseudo: Enable the General purpose timer interrupt */
	Scheduler_vidGptInterruptEnable();
	/* !Pseudo: Enable the Global Interrupt Bit */
	Scheduler_vidEnableInterrupts();
}/* !Comment: End of start function */


FUNC(void,CODE_FLASH_CLASS)		Scheduler_vidUpdate(void)
{
	/* !Comment: Local counter variable */
	VAR(uint8,AUTOMATIC)	u8LocalIndex;

	/* !Pseudo: Check available tasks */
	for(u8LocalIndex = 0; u8LocalIndex < Scheduler_u8MAX_NUM_TASKS; u8LocalIndex++)
	{
		/* !Pseudo: Checking valid task pointer */
		if( Scheduler_astrTasks[u8LocalIndex].pfTask != NULL )
		{
			/* !Pseudo: check the task due by checking the task
			 * !Pseudo: available delay 						*/
			if(Scheduler_astrTasks[u8LocalIndex].u16Delay == 0)
			{
				/* !Comment: The task is due */
				/* !Pseudo: Enable task "Run" flag */
				Scheduler_astrTasks[u8LocalIndex].u8Run = true;
				/* !Comment: The periodic tasks shall run automatically */
				/* !Pseudo: check for periodicity and adjust it */
				if(Scheduler_astrTasks[u8LocalIndex].u16Period)
				{
				    Scheduler_astrTasks[u8LocalIndex].u16Delay = Scheduler_astrTasks[u8LocalIndex].u16Period;
				}
				else
				{
					/* !Comment: The task is one-shot */
					/* !Pseudo: Do nothing */
				}
			}
			else
			{
				/* !Comment: The task is not due */
				/* !Pseudo: Decrement the delay */
				Scheduler_astrTasks[u8LocalIndex].u16Delay--;
			}
		}
		else
		{
			/* !Pseudo: Do nothing */
		}

	}/* !Comment: End of for-loop */
}/* !Comment: End of update function */



FUNC(void,CODE_FLASH_CLASS)		Scheduler_vidDispatcher(void)
{
	/* !Comment: Local counter variable */
	VAR(uint8,AUTOMATIC)	u8LocalIndex;


	/* !Pseudo: Continuiuily check for task due */
	while(true)
	{
		/* !Pseudo: Looping on all available tasks */
		for(u8LocalIndex = 0; u8LocalIndex < Scheduler_u8MAX_NUM_TASKS; u8LocalIndex++)
		{
			/* !Pseudo: check for task due */
			if(Scheduler_astrTasks[u8LocalIndex].u8Run == true)
			{
				/* !Pseudo: Run the task */
				Scheduler_astrTasks[u8LocalIndex].pfTask();
				/* !Bug: missed reset of run flag */
				Scheduler_astrTasks[u8LocalIndex].u8Run = false;
				/* !Comment: the periodic tasks shall run automatically */
				/* !Comment: delete the task if it is one-shot */
				/* !Pseudo: check for periodicity */
				if(Scheduler_astrTasks[u8LocalIndex].u16Period == 0)
				{
					Scheduler_u8DeleteTask(u8LocalIndex);
				}
				else
				{
					/* !Comment: The task is periodic and shall run automatically
					 * !Comment: No need for deletion 								*/
					/* !Pseudo: Do nothing 											*/
				}
			}
			else
			{
				/* !Comment: The task is not due 									*/
				/* !Pseudo: Do nothing												 */
			}
		}

		Scheduler_vidReportStatus();

		/* !Comment: This function is 8051 architecture related,
		 * !Comment: to find analogy later */
		//static FUNC(void,CODE_FLASH_CLASS)	Scheduler_vidGoToSleep(void);

	}/* !Comment: End of while-one loop of dispatcher */


}/* !Comment: End of dispatcher function */


FUNC(uint8,CODE_FLASH_CLASS)		Scheduler_u8AddTask(void(*pfTaskFunctionPtr)(void),
															 VAR(uint16,AUTOMATIC)	u16Delay,
															 VAR(uint16,AUTMATIC)	u16Period)
{
	/* !Comment: Local counter variable */
	VAR(uint8,AUTOMATIC)	u8LocalIndex = 0;

	/* !Pseudo: Find a gap in the tasks array by checking the  */
	/* !Pseudo: valid task pointer and boundary of array */
	while((Scheduler_astrTasks[u8LocalIndex].pfTask != NULL) && (u8LocalIndex < Scheduler_u8MAX_NUM_TASKS))
	{
		u8LocalIndex++;
	}

	/* !Pseudo: Check for array boundary */
	if(u8LocalIndex == Scheduler_u8MAX_NUM_TASKS)
	{
		/* !Pseudo: set the global error variable */
		Error_eu8Code = 6; /* any value until handling a error handling module  */
		return Scheduler_u8MAX_NUM_TASKS;
	}
	else
	{
		/* !Comment: Available space for new task found */
		/* !Pseudo: Add the new task parameters */
		Scheduler_astrTasks[u8LocalIndex].pfTask = pfTaskFunctionPtr;
		Scheduler_astrTasks[u8LocalIndex].u16Delay = u16Delay;
		Scheduler_astrTasks[u8LocalIndex].u16Period = u16Period;
		Scheduler_astrTasks[u8LocalIndex].u8Run = false;

		/* !Pseudo: returning the index of added task */
		return u8LocalIndex;
	}
} /* !Comment: End of add task function */


FUNC(uint8,CODE_FLASH_CLASS)	Scheduler_u8DeleteTask( CONST(uint8,AUTOMATIC) ku8Index)
{
	VAR(uint8, AUTOMATIC) u8LocalReturnCode;

	/* !Pseudo: Check for task pointer validity */
	if(Scheduler_astrTasks[ku8Index].pfTask == NULL)
	{
		/* !Comment: This is not valid task */
		/* !Pseudo: Set the global error code to proper code */
		Error_eu8Code = 8; /* any value until handling a error handling module  */

		/* !Pseudo: Setting the return value */
		u8LocalReturnCode = E_OK;
	}
	else
	{
		/* !Comment: valid task pointer */
		/* !Comment: Deletion means clear task parameters not deleting code in memory */

		/* !Pseudo: Invalidate (reset) the task parameters */
		Scheduler_astrTasks[ku8Index].pfTask = NULL;
		Scheduler_astrTasks[ku8Index].u16Delay = 0;
		Scheduler_astrTasks[ku8Index].u16Period = 0;
		Scheduler_astrTasks[ku8Index].u8Run = false;
	}

	return u8LocalReturnCode;
} /* !Comment: End of delete task function */


FUNC(void,CODE_FLASH_CLASS)		Scheduler_vidReportStatus(void)
{
   /* TODO: impelentation of such function */
}



FUNC(void, CODE_FLASH_CLASS)				Scheduler_vidSetISR(void)
{

	Scheduler_pfGptCallBack(Scheduler_vidUpdate);

}
