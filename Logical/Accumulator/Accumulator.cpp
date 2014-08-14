/********************************************************************
 * COPYRIGHT -- Pollux
 ********************************************************************
 * Program: Accumulator
 * File: Accumulator.cpp
 * Author: CEDRIC
 * Created: July 22, 2014
 ********************************************************************
 * Implementation of program Accumulator
 ********************************************************************/

#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

unsigned long bur_heap_size = 0xFFFF; /* Must be added manually */

#include "../CPP/Timer.h"

Timer* timer;
void _INIT AccumulatorInit(void)
{
	step = 10;
	timer = new Timer(1000);
}

void _CYCLIC AccumulatorCyclic(void)
{
	switch( step )
	{
		case 10:	// WAIT accumulator == TRUE
			if( diAccumulator == true )
			{
				timer->start();
				step = 20;
			}
			else
			{
				if( gMode != AUTOMATIC )
				{
					doAccumulator = true;
					step = 40;
				}
			}
			break;
		
		case 20:	// WAIT timer
			timer->cyclic();
			if( timer->getOutput() == true )
			{
				gMoving = false;
				doAccumulator = true;
				step = 30;
			}
			else 	// WAIT reset accumulator?
			{
				if( diAccumulator == false )
				{
					timer->stop();
					step = 10;
				}
			}
			break;

		case 30:	// WAIT accumulator = FALSE
			if( diAccumulator == false )
			{
				gMoving = true;
				doAccumulator = false;
				step = 10;
			}
			break;
		
		case 40:	// Not in automatic
			if( gMode == AUTOMATIC )
			{
				doAccumulator = false;
				step = 10;
			}
			break;
	}
}

void _EXIT AccumulatorExit(void)
{
	delete timer;
}
