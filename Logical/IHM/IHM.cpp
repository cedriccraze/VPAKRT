/********************************************************************
 * COPYRIGHT -- Pollux
 ********************************************************************
 * Program: IHM
 * File: IHM.cpp
 * Author: CEDRIC
 * Created: August 13, 2014
 ********************************************************************
 * Implementation of program IHM
 ********************************************************************/

#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

unsigned long bur_heap_size = 0xFFFF; /* Must be added manually */

#include <string.h>
#include "../CPP/Json.h"
#include "../CPP/SerialCodes.h"

void _INIT IHMInit(void)
{
	step	= 10;
	nActivePage	= 10;
	
	// HTTP Client comunication
	client.enable			= false;
	client.abort			= false;
	client.send				= false;
	client.hostPort			= 0;
	client.method			= httpMETHOD_GET;
	client.option			= httpOPTION_HTTP_11;
	client.pHost			= (long unsigned int)gHost;
	client.pRequestData		= 0;
	client.pRequestHeader	= 0;
	client.pResponseData	= (long unsigned int)response;
	client.pResponseHeader	= 0;
	client.pStatistics		= 0;
	client.pStruct			= 0;
	client.pUri				= (long unsigned int)url_request_serial;
	client.requestDataLen	= 0;
	client.responseDataSize	= sizeof(response);
}

void _CYCLIC IHMCyclic(void)
{
	httpClient(&client);
	
	switch( step )
	{
		case 10:	// Check system UUID
			if( strlen( gSystemUUID ) < 20 )
				step = 20;
			else
				step = 100;
			break;
		
		case 20:	// Send message to server
			memset( response, 0, sizeof(response));
			client.enable	= true;
			client.send		= true;
			step			= 30;
			break;
		
		case 30:	// Wait answer
			switch( client.status )
			{
				case 0:	// Ok
					client.send	= false;
					step		= 40;
					break;
				
				case 65535:	// Busy
					break;
				
				case 65534:	// Enable missing
					break;
				
				default:	// Error
					break;
			}
			break;
		
		case 40:	// Wait response
			if( strlen(response) > 0 )
			{
				bServerCom	= true;
				step		= 50;
			}
			break;
	}
}

void _EXIT IHMExit(void)
{
	/* TODO: Add code here */
}
