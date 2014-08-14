/********************************************************************
 * COPYRIGHT -- Pollux
 ********************************************************************
 * Program: Conveyor
 * File: Conveyor.cpp
 * Author: CEDRIC
 * Created: July 16, 2014
 ********************************************************************
 * Implementation of program Conveyor
 ********************************************************************/

#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

unsigned long bur_heap_size = 0xFFFF; /* Must be added manually */

#include "../CPP/Timer.h"

void shift_parts();

Timer* pTimer;
void _INIT ConveyorInit(void)
{
	step = 10;
	pTimer = new Timer(1);
}

void _CYCLIC ConveyorCyclic(void)
{
	switch( step )
	{
		case 10:	// Check if movement allowed
			if( gMoving == true )
				step = 20;
			break;
		
		case 20:	// Set output to 1
			doConveyor = true;
			pTimer->start();
			step = 30;
			break;
		
		case 30:	// Wait timer
			pTimer->cyclic();
			if( pTimer->getOutput() == true )
				step = 40;
			break;
		
		case 40:	// Set output to 0
			doConveyor = false;
			pTimer->stop();
			step = 10;
			// Shift register
			shift_parts();
			break;
	}
}

void _EXIT ConveyorExit(void)
{
	delete pTimer;
}

void shift_parts()
{
	for( int i=0; i<50; i++)
		{
		if( Parts[i].bPresent == true )
		{
			Parts[i].nPosition++;
		}
	}
}
