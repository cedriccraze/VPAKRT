/********************************************************************
 * COPYRIGHT -- Pollux
 ********************************************************************
 * Program: Entry
 * File: File.cpp
 * Author: CEDRIC
 * Created: July 24, 2014
 *******************************************************************/

#include <bur/plctypes.h>
#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

#include "File.h"

File::File( char *Device, char *Filename )
{
	m_bError		= false;
	m_uErrorCode	= 0;
	m_wId			= 0;
	m_uStatus		= 0;
	m_step			= 10;
	memset( &m_fopen, 0, sizeof(FileOpen_typ) );
	memset( &m_fcreate, 0, sizeof(FileCreate_typ) );
	memset( &m_fread, 0, sizeof(FileRead_typ) );
	memset( &m_fwrite, 0, sizeof(FileWrite_typ) );
	memset( &m_fclose, 0, sizeof(FileClose_typ) );
	strncpy( m_Device, Device, 80 );
	strncpy( m_Filename, Filename, 80 );
}

File::~File()
{
	this->close();
}

INT File::close()
{
	m_fclose.enable	= 1;
	m_fclose.ident	= m_wId;
	FileClose( &m_fclose );
	
	m_uStatus	= m_fclose.status;
	
	switch( m_uStatus )
	{
		case 0:		// Ok
			m_bError	= false;
			return 1;
			break;
		
		case 65535:	// Busy
			return 0;
			break;
		
		case 20799:	// 
			m_bError	= true;
			m_uErrorCode	= FileIoGetSysError();
			return -2;
			break;
		
		default:	// Other errors
			m_bError	= true;
			return -3;
			break;
	}
}

INT File::create()
{
	m_fcreate.enable    = 1;
	m_fcreate.pDevice = (UDINT)m_Device;
	m_fcreate.pFile   = (UDINT)m_Filename;
	/* Call FUB */
	FileCreate(&m_fcreate);
	
	/* Get output information of FBK */
	m_wId		= m_fcreate.ident;
	m_uStatus	= m_fcreate.status;
	
	switch( m_uStatus )
	{
		case 0:		// Ok
			return 1;
			break;
		
		case 65535:	// Busy
			return 0;
			break;
		
		case 20799:	// File IO error
			m_bError		= true;
			m_uErrorCode	= FileIoGetSysError();
			return -1;
			break;
		
		default:	// Other errors
			m_bError	= true;
			return -3;
			break;
	}
}

INT File::get_size( UDINT *size )
{
	int return_code = 0;
	
	switch( m_step )
	{
		case 10:	// Get file information
			switch( this->read_info() )
			{
				case 0:	// Busy
					break;
				
				case 1:	// Ok
					m_step = 20;
					break;
				
				default:
					return_code = -3;
					break;
			}
			break;
		
		case 20:	// Get file size
			*size = m_tinfo.size;
			return_code = 1;
			m_step = 10;
			break;
	}
	
	return return_code;
}

INT File::open()
{
	m_fopen.enable    = 1;
	m_fopen.pDevice = (UDINT)m_Device;
	m_fopen.pFile   = (UDINT)m_Filename;
	m_fopen.mode	= FILE_RW;
	/* Call FUB */
	FileOpen(&m_fopen);
	
	/* Get output information of FBK */
	m_wId		= m_fopen.ident;
	m_uStatus	= m_fopen.status;
	
	/* Status OK? */
	switch( m_uStatus )
	{
		case 0:	// OK
			return 1;
			break;
		
		case 65535:	// Busy
			return 0;
			break;
		
		case 20708:	// Not exist
			m_bError	= true;
			return -1;
			break;
		
		case 20799:	// File IO error
			m_bError		= true;
			m_uErrorCode	= FileIoGetSysError();
			return -2;
			break;
		
		default:	// Other errors
			m_bError	= true;
			return -3;
			break;
	}
}

INT File::read( UDINT buffer, UDINT length, UDINT offset )
{
	m_fread.enable      = 1;
	m_fread.ident     = m_wId;
	m_fread.offset    = offset;
	m_fread.pDest     = buffer;
	m_fread.len       = length;
	/* Call FBK */
	FileRead(&m_fread);
	/* Get status */
	m_uStatus = m_fread.status;
	
	/* Verify status */
	switch( m_uStatus )
	{
		case 0:		// Ok
			return 1;
			break;
		
		case 65535:	// Busy
			return 0;
			break;
		
		case 20799:	// File IO error
			m_bError		= true;
			m_uErrorCode	= FileIoGetSysError();
			return -1;
			break;
		
		default:	// Other cases
			m_bError	= true;
			return -3;
			break;
	}
}

INT File::read_info()
{
	m_info.enable	= 1;
	m_info.pDevice	= (UDINT)m_Device;
	m_info.pName	= (UDINT)m_Filename;
	m_info.pInfo	= (UDINT)&m_tinfo;
	
	FileInfo( &m_info );
	
	switch( m_info.status )
	{
		case 0:		// Ok
			return 1;
			break;
		
		case 65535:	// Busy
			return 0;
			break;
		
		default:	// All errors
			return -3;
			break;
	}
}

INT File::write( UDINT buffer, UDINT length, UDINT offset )
{
	m_fwrite.enable     = 1;
	m_fwrite.ident    = m_wId;
	m_fwrite.offset   = offset;
	m_fwrite.pSrc     = buffer;
	m_fwrite.len      = length;

	/* Call FBK */
	FileWrite(&m_fwrite);
	/* Get status */
	m_uStatus = m_fwrite.status;
	
	switch( m_uStatus )
	{
		case 0:		// Ok
			return 1;
			break;
		
		case 65535:	// Busy
			return 0;
			break;
		
		case 20799:	// File IO error
			m_bError		= true;
			m_uErrorCode	= FileIoGetSysError();
			return -2;
			break;
		
		default:	// Other errors
			m_bError	= true;
			return -3;
			break;
	}
}
