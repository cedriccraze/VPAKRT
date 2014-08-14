/********************************************************************
 * COPYRIGHT -- Pollux
 ********************************************************************
 * Program: main
 * File: main.cpp
 * Author: CEDRIC
 * Created: July 17, 2014
 ********************************************************************
 * Implementation of program main
 ********************************************************************/

#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif
#include <standard.h>

unsigned long bur_heap_size = 0xFFFF; /* Must be added manually */
#include "../CPP/SerialCodes.h"

void _INIT mainInit(void)
{
	gMode	= MANUAL;
	gSerialQueue = (UDINT)createSerialQueue();
}

void _CYCLIC mainCyclic(void)
{
	switch( gMode )
	{
		case MANUAL: // Manual
			gMoving = false;
			doAlarmTower.green	= false;
			doAlarmTower.yellow	= true;
			doAlarmTower.red	= false;
			gMode = STANDBY;
			break;
		
		case AUTOMATIC:	// Automatic
			gMoving = true;
			doAlarmTower.green	= true;
			doAlarmTower.yellow	= false;
			doAlarmTower.red	= false;
			gMode = STANDBY;
			break;
		
		case ERROR:	// error
			gMoving = false;
			doAlarmTower.green	= false;
			doAlarmTower.yellow	= false;
			doAlarmTower.red	= true;
			gMode = STANDBY;
			break;
		
		case STANDBY:	// standby
			break;
	}
}

void _EXIT mainExit(void)
{
	// Release the serial queue
	freeSerialQueue((queue_serial*)gSerialQueue);
}
