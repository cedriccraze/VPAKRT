/********************************************************************
 * COPYRIGHT -- Pollux
 ********************************************************************
 * Program: Entry
 * File: Entry.cpp
 * Author: CEDRIC
 * Created: July 17, 2014
 ********************************************************************
 * Implementation of program Entry
 ********************************************************************/

#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

unsigned long bur_heap_size = 0xFFFF; /* Must be added manually */

//#include "File.h"
#include <string.h>
#include "../CPP/Json.h"
#include "../CPP/SerialCodes.h"

int add_part();

void _INIT EntryInit(void)
{
	step	= 0;
}

void _CYCLIC EntryCyclic(void)
{
	switch( step )
	{
		case 10:	// Check size (low)
			if( gSerialQueue == 0 )
				break;
			size = getSerialQueueSize( (queue_serial*)gSerialQueue );
			if( size == 0  )
				gMoving		= false;
			else
			{
				gMoving	= true;
				step	= 20;
			}
			break;
		
		case 20:	// Read one code
			strcpy( serial_code, popSerialCodeFromQueue( (queue_serial*)gSerialQueue ) );
			step = 30;
			break;
		
		case 30:	// Wait sensor
			if( diEntry == true )
			{
				position = add_part();
				if( position >= 0 )
					step = 40;
			}
			break;
		
		case 40:
			if( diEntry == false )
			{
				Parts[position].nWidth = Parts[position].nPosition;
				strcpy( Parts[position].sSerial, serial_code );
				step = 10;
			}
			break;
	}
	
}

void _EXIT EntryExit(void)
{
}

int part_index( int position )
{
	for( int i=0; i<50; i++ )
		{
		if( Parts[i].bPresent == true )
		{
			if( Parts[i].nPosition == position )
				return i;
		}
	}
	
	// No position matches
	return -1;
}

int add_part()
{
	for( int i=0; i<50; i++ )
		{
		if( Parts[i].bPresent == false )
		{
			Parts[i].bPresent = true;
			Parts[i].nPosition	= 0;
			Parts[i].nWidth		= 0;
			Parts[i].bPrinted	= false;
			Parts[i].bCamera1	= false;
			Parts[i].bRejected	= false;
			Parts[i].bResult	= false;
			return i;
		}
	}
	
	// No position found!!
	return -1;
}
