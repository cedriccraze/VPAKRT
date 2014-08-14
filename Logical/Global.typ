(********************************************************************
 * COPYRIGHT -- Pollux
 ********************************************************************
 * File: Global.typ
 * Author: CEDRIC
 * Created: July 16, 2014
 ********************************************************************
 * Global data types of project VPakRT
 ********************************************************************)

TYPE
	Mark : 	STRUCT 
		FabricationDate : STRING[20];
		ValidityDate : STRING[20];
		Drug : STRING[20];
		Lot : STRING[20];
		GTIN : STRING[20];
	END_STRUCT;
	status : 
		(
		STANDBY,
		ERROR,
		AUTOMATIC,
		MANUAL
		);
	Serial :STRING[20]; 
	Part : 	STRUCT 
		nWidth : UINT;
		nPosition : UINT;
		bRejected : BOOL;
		bResult : BOOL;
		bCamera1 : BOOL;
		bPrinted : BOOL;
		bPresent : BOOL;
		sSerial : Serial;
	END_STRUCT;
	alarm_tower : 	STRUCT 
		yellow : BOOL;
		green : BOOL;
		red : BOOL;
	END_STRUCT;
END_TYPE
