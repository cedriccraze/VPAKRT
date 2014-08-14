/********************************************************************
 * COPYRIGHT -- Pollux
 ********************************************************************
 * Program: Reject
 * File: Reject.cpp
 * Author: CEDRIC
 * Created: July 17, 2014
 ********************************************************************
 * Implementation of program Reject
 ********************************************************************/

#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif
unsigned long bur_heap_size = 0xFFFF; /* Must be added manually */

#include "../CPP/Timer.h"

Timer* timer;
void _INIT RejectInit(void)
{
	step		= 10;
	timer		= new Timer(50);
	position	= -1;
}

void _CYCLIC RejectCyclic(void)
{
	switch( step )
	{
		case 10:
			for( int i=0; i<50; i++ )
				{
				if( Parts[i].bPresent == true && Parts[i].bResult == false )
				{
					if( Parts[i].nPosition == gOffsetReject )
					{
						position = i;
						step = 20;
					}
				}
			}
			break;
		
		case 20:	// Start trigger
			doReject = true;
			consecutive_rejects++;
			timer->start();
			step = 30;
			break;
		
		case 30:	// Stop trigger
			timer->cyclic();
			if( timer->getOutput() == true )
			{
				doReject = false;
				if( consecutive_rejects == gConsecutiveRejects )
					step = 40;
				else
					step = 10;
			}
			break;
		
		case 40:	// Consecutive rejects
			gMoving	= false;
			gMode	= ERROR;
			// TODO
			// - Go to specific screen
			// - Allow user to stop/pause/continue
			break;
	}
}

void _EXIT RejectExit(void)
{
	delete timer;
}
