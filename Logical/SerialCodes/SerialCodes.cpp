/********************************************************************
 * COPYRIGHT -- Pollux
 ********************************************************************
 * Program: SerialCodes
 * File: SerialCodes.cpp
 * Author: CEDRIC
 * Created: August 01, 2014
 ********************************************************************
 * Implementation of program SerialCodes
 ********************************************************************/

#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

unsigned long bur_heap_size = 0xFFFF; /* Must be added manually */

#include <string.h>
#include "../CPP/Json.h"
#include "../CPP/SerialCodes.h"

#define	URL_TEST			"~cedriccraze/vpackRT/services/index.php?user_UUID=%s&system_UUID=%s&lot_number=%s&quantity=%d"
#define	URL_REQUEST_SERIAL	"~cedriccraze/vpackRT/services/request_iums.php?user_UUID=%s&system_UUID=%s&lot_number=%s&quantity=%d"

void _INIT SerialCodesInit(void)
{
	step	= 0;
	
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

void _CYCLIC SerialCodesCyclic(void)
{
	// HTTP Communication
	httpClient(&client);
	switch( step )
	{
		case 10:	// Send request
			if( gSerialQueue == 0 )
				break;
			size	= getSerialQueueSize((queue_serial*)gSerialQueue);
			if( size < 50 )
			{
				memset( response, 0, sizeof(response) );
				sprintf( url_request_serial, URL_REQUEST_SERIAL, gUserUUID, gSystemUUID, gLotNumber, 400-getSerialQueueSize((queue_serial*)gSerialQueue) );
				client.enable	= true;
				client.send		= true;
				step			= 20;
			}
			break;
		
		case 20:	// Wait communication
			switch( client.status )
			{
				case 0:		// Ok
					client.send = false;
					step		= 30;
					break;
				
				case 65535:	// Busy
					break;
				
				case 65534:	// Enable
					break;
				
				default:	// Error
					client.send	= false;
					step		= 999;
					break;
			}
			break;
		
		case 30:	// Wait response
			if( strlen(response) > 0 )
				step = 40;
			break;
		
		case 40:	// JSON
			{
				JsonError error;
				JsonObject* root = (JsonObject*)json_decode( response, &error );
				if( root == NULL || root->get_type() != kJSON_OBJECT )
				{
					step = 999;
					break;
				}
				
				JsonValue* value = (JsonValue*)root->key_value("result");
				if( value == NULL || strcmp( value->string_value(), "NOK" ) == 0 )
				{
					step = 998;
					break;
				}
				if( strcmp( value->string_value(), "OK" ) != 0 )
				{
					step = 997;
					break;
				}
				
				JsonArray* codes = (JsonArray*)root->key_value("codes");
				if( codes == NULL || codes->get_type() != kJSON_ARRAY )
				{
					step = 996;
					break;
				}
				for( int i=0; i<codes->getsize(); i++ )
					{
					value = (JsonValue*)codes->get_at(i);
					if( value == NULL || value->get_type() != kJSON_STRING)
					{
						step = 995;
						break;
					}

					pushSerialCodeToQueue((queue_serial*)gSerialQueue, value->string_value() );
				}
				
				if( step < 900 )
					step = 10;
				
				// Free memory
				delete root;
			}
			break;
		
		default:
			gMode	= ERROR;
			gMoving	= false;
			break;
	}
}

void _EXIT SerialCodesExit(void)
{
	client.enable	= false;
	client.send		= false;
}
