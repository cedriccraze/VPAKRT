/********************************************************************
 * COPYRIGHT -- Pollux
 ********************************************************************
 * Program: Camera1
 * File: Camera1.cpp
 * Author: CEDRIC
 * Created: July 17, 2014
 ********************************************************************
 * Implementation of program Camera1
 ********************************************************************/

#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif
unsigned long bur_heap_size = 0xFFFF; /* Must be added manually */

#include <string.h>
#include "../CPP/Timer.h"
#include "../CPP/Json.h"

#define	URL_TEST			"~cedriccraze/vpackRT/services/index.php?user_UUID=%s&system_UUID=%s&lot_number=%s&quantity=%d"
#define	URL_UPDATE_STATUS	"~cedriccraze/vpackRT/services/update_ium_status.php?user_UUID=%s&system_UUID=%s&serial=%s&status=%d"

#define	STATUS_VALID		2
#define	STATUS_NOT_VALID	3

Timer* timer;
void _INIT Camera1Init(void)
{
	step		= 10;
	step_http	= 0;
	timer		= new Timer(100);
	position	= -1;
	
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
	client.pUri				= (long unsigned int)url;
	client.requestDataLen	= 0;
	client.responseDataSize	= sizeof(response);
}

void _CYCLIC Camera1Cyclic(void)
{
	switch( step )
	{
		case 10:	// Send code to printer
			for( int i=0; i<50; i++ )
				{
				if( Parts[i].bPresent == true && Parts[i].bCamera1 == false )
				{
					if( Parts[i].nPosition == gOffsetCamera )
					{
						position = i;
						step = 20;
					}
				}
			}
			break;
		
		case 20:	// Start trigger
			doCam1TRG = true;
			timer->start();
			step = 30;
			break;
		
		case 30:	// Stop trigger
			timer->cyclic();
			if( timer->getOutput() == true )
			{
				doCam1TRG = false;
				step = 40;
			}
			break;
		
		case 40:	// Wait DV
			if( diCam1DV == true )
			{
				Parts[position].bResult		= diCam1RES;
				Parts[position].bCamera1	= true;
								
				step = 50;
			}
			break;
		
		case 50:	// Wait HTTP
			if( step_http == 0 )
			{
				// Update server
				step_http = 10;
				// Copy data
				memcpy( &part, &Parts[position], sizeof(Part) );
				
				step = 10;
			}
			else	// OVERRUN !!!
			{
				gMoving	= false;
				gMode	= ERROR;
				step	= 999;
			}
			break;
	}
	
	// HTTP Communication
	httpClient(&client);
	switch( step_http )
	{
		case 10:	// Start HTTP
			memset( response, 0, sizeof(response) );
			sprintf( url, URL_UPDATE_STATUS, gUserUUID, gSystemUUID, part.sSerial, part.bResult == true ? STATUS_VALID : STATUS_NOT_VALID  );
			client.enable	= true;
			client.send		= true;
			step_http		= 20;
			break;
		
		case 20:	// Wait HTTP processing
			switch( client.status )
			{
				case 0:		// Ok
					client.send	= false;
					step_http		= 30;
					break;
				
				case 65535:	// BUSY
					break;
				
				default:	// Error
					step_http	= 999;
					break;
			}
			break;
		
		case 30:	// Wait response
			if( strlen(response) > 0 )
				step_http = 40;
			break;
		
		case 40:	// JSON decodification
			{
				JsonError error;
				JsonObject* root = (JsonObject*)json_decode( response, &error );
				if( root == NULL || root->get_type() != kJSON_OBJECT )
				{
					step_http = 999;
					break;
				}
				
				JsonValue* value = (JsonValue*)root->key_value("result");
				if( value == NULL || strcmp( value->string_value(), "NOK" ) == 0 )
				{
					step_http = 998;
					break;
				}
				if( strcmp( value->string_value(), "OK" ) != 0 )
				{
					step_http = 997;
					break;
				}
				
				// End communication
				step_http	= 0;
			}
			break;
	}
}

void _EXIT Camera1Exit(void)
{
	delete timer;
}
