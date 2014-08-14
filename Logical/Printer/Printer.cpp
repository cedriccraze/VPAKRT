/********************************************************************
 * COPYRIGHT -- Pollux
 ********************************************************************
 * Program: Printer
 * File: Printer.cpp
 * Author: CEDRIC
 * Created: July 17, 2014
 ********************************************************************
 * Implementation of program Printer
 ********************************************************************/

#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif
unsigned long bur_heap_size = 0xFFFF; /* Must be added manually */

#include "../CPP/Timer.h"

Timer *timerPrinter;
void _INIT PrinterInit(void)
{
	step			= 10;
	timerPrinter	= new Timer(100);
	position		= -1;
}

void _CYCLIC PrinterCyclic(void)
{
	switch( step )
	{
		case 10:	// Wait ACK to print
			for( int i=0; i<50; i++ )
			{
				if( Parts[i].bPresent == true && Parts[i].bPrinted == false )
				{
					if( Parts[i].nPosition == gOffsetPrinter )
					{
						position = i;
						step = 20;
					}
				}
			}
			break;
		
		case 20:	// Start printing
			doPrinter = true;
			timerPrinter->start();
			step = 30;
			break;
		
		case 30:
			timerPrinter->cyclic();
			if( timerPrinter->getOutput() == true )
			{
				doPrinter = false;
				step = 10;
				Parts[position].bPrinted = true;
			}
			break;
	}
}

void _EXIT PrinterExit(void)
{
	delete timerPrinter;
}
