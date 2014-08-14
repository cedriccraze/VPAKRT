/********************************************************************
 * COPYRIGHT -- Pollux
 ********************************************************************
 * Program: CheckReject
 * File: CheckReject.cpp
 * Author: CEDRIC
 * Created: July 17, 2014
 ********************************************************************
 * Implementation of program CheckReject
 ********************************************************************/

#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif
unsigned long bur_heap_size = 0xFFFF; /* Must be added manually */

#include "../CPP/Timer.h"

Timer* timer;
void _INIT CheckRejectInit(void)
{
	step		= 10;
	position	= -1;
}

void _CYCLIC CheckRejectCyclic(void)
{
	switch( step )
	{
		case 10:
			for( int i=0; i<50; i++ )
			{
				if( Parts[i].bPresent == true && Parts[i].bResult == false )
				{
					if( Parts[i].nPosition == gOffsetCheckReject )
					{
						position = i;
						step = 20;
					}
				}
			}
			break;
		
		case 20:
			if( diCheckReject == true )
			{
				gMode = ERROR;
			}
			else
				step = 10;
			break;
	}
}

void _EXIT CheckRejectExit(void)
{
	/* TODO: Add code here */
}
