/********************************************************************
 * COPYRIGHT -- Pollux
 ********************************************************************
 * Program: Entry
 * File: File.h
 * Author: CEDRIC
 * Created: July 24, 2014
 *******************************************************************/

#include <bur/plctypes.h>
#include <fileio.h>
#include <string.h>

class File
{
	private:
	char m_Device[80];
	char m_Filename[80];
	UDINT m_wId;
	UINT m_uErrorCode;
	UINT m_uStatus;
	BOOL m_bError;
	FileOpen_typ m_fopen;
	FileClose_typ m_fclose;
	FileCreate_typ m_fcreate;
	FileRead_typ m_fread;
	FileWrite_typ m_fwrite;
	FileInfo_typ m_info;
	fiFILE_INFO m_tinfo;
	int m_step;
	
	public:
	File( char *Device, char *Filename );
	~File();
	
	INT close();
	INT create();
	INT get_size( UDINT* size );
	INT open();
	INT read( UDINT buffer, UDINT length, UDINT offset );
	INT read_info();
	INT write( UDINT buffer, UDINT length, UDINT offset );
};
