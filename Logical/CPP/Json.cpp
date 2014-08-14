/********************************************************************
 * COPYRIGHT -- Pollux
 ********************************************************************
 * Library: CPP
 * File: Json.cpp
 * Author: CEDRIC
 * Created: July 30, 2014
 *******************************************************************/

#include <bur/plctypes.h>
#include "Json.h"

/*
 * Json class
 *
	*/
Json::Json( JsonError* error, JSON_TYPE type)
{
	this->m_type = type;
	this->m_error = error;
}

Json::~Json()
{
	this->free();
}

void Json::free()
{
}

string Json::parse_string( string* s, unsigned int* position, char* chars/*=NULL*/ )
{
	unsigned int	start	= string::npos;
	unsigned int	end		= string::npos;
	string			str;
	
	if( chars == NULL )
	{
		// Find first "
		start	= s->find('"', *position);
		if( start != string::npos )
		{
			start++;
			(*position)++;
			
			// Find second "
			end = s->find( '"', start );
		}
	}
	else
	{
		start	= *position;
		end		= s->find_first_of(chars,*position);
	}
	
	if( start == string::npos || end == string::npos )
		return str;
	
	// Build string
	(*position) = end+1;
	int length	= end-start;
	str = s->substr( start, length );
	return str;
}

Json* Json::parse_value( string* s, unsigned int* position )
{
	Json* obj		= NULL;
	bool bParsing	= true;
	string str;
	do
	{
		switch( s->at((*position)) )
		{			
			case ' ':	// Do not consider spaces
				break;
				
			case '"':	// String
				str = this->parse_string(s, position);
				if( str.empty() )
				{
					this->m_error->set_error_code(kJSON_ERR_STRING, *position);
					return NULL;
				}
				obj = new JsonValue(this->m_error,kJSON_STRING, str );
				bParsing	= false;
				break;
				
			case '{':	// Object
				obj	= new JsonObject(this->m_error);
				(*position)++;
				if( obj->parse(s,position) == NULL )
					return NULL;
				bParsing	= false;
				break;
				
			case '[':	// Array
				obj = new JsonArray(this->m_error);
				(*position)++;
				if( obj->parse(s,position) == NULL )
					return NULL;
				bParsing = false;
				break;
				
			case '-':	// Number
			case '+':
			case '.':
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
			case 'f':	// false
			case 't':	// true
				str = this->parse_string(s,position, ",}");
				if( str.empty() )
				{
					this->m_error->set_error_code(kJSON_ERR_STRING,*position);
					return NULL;
				}
				obj = new JsonValue(this->m_error, kJSON_NULL, str );
				bParsing = false;
				// Decrease index
				(*position)--;
				break;			
				
			case ',':	// Syntax Error
			case ']':
			case '}':
				this->m_error->set_error_code(kJSON_ERR_PARSE_VALUE, *position );
				return NULL;
				break;
		}
	}
	while( bParsing == true && (*position)++ < s->length() );
	
	return obj;
}

/*
* JsonValue class
*
*/
JsonValue::JsonValue( JsonError* error, JSON_TYPE type, string value ) : Json( error, type )
{	
	if( type == kJSON_NULL )
		this->set_string(value);
	else
		this->m_string = value;
}

JsonValue::~JsonValue()
{
	this->free();
}

void JsonValue::free()
{
	this->m_string.clear();
}

void JsonValue::set_string( string str )
{
	this->free();
	this->m_string	= str;

	int pos = 0;	
	
	// Check if TRUE
	if( this->m_string.compare("true") == 0 )
		Json::set_type(kJSON_TRUE);
	else
	{	// Check if FALSE
		if( this->m_string.compare("false") == 0 )
			Json::set_type(kJSON_FALSE);
		else
		{	// Check if REAL
			pos = 0;
			if( this->m_string.find('.') != string::npos )
				Json::set_type(kJSON_REAL);
			else
			{	// Check if REAL (exponent)
				pos = 0;
				if( this->m_string.find('e') != string::npos )
					Json::set_type(kJSON_REAL);
				else
				{	// Check if INTEGER
					pos = 0;
					if( this->m_string.find_first_of("0123456789") != string::npos )
						Json::set_type(kJSON_INTEGER);
				}
			}
		}
	}
}

/*
* JsonObject class
*
*/
JsonObject::JsonObject( JsonError* error ) : Json(error, kJSON_OBJECT)
{
}

JsonObject::~JsonObject()
{
	this->free();
}

void JsonObject::free()
{
	// Delete the pairs
	map<string,Json*>::iterator it;
	for( it=this->m_keys.begin(); it!=this->m_keys.end(); ++it )
	{
		Json* value = it->second;
		delete value;
	}
		
	// Delete the vector
	this->m_keys.clear();

	// Call parent
	Json::free();
}

Json* JsonObject::parse( string* s, unsigned int* position )
{
	// Free any previous data
	this->free();
	
	while( true )
	{
		// Find key
		string key	= this->parse_string(s, position);
		if( key.empty() )
		{
			this->m_error->set_error_code(kJSON_ERR_STRING,*position);
			return NULL;
		}
		
		// Find :
		unsigned int pos3 = s->find( ':', *position );
		if( pos3 == string::npos )
		{
			this->m_error->set_error_code(kJSON_ERR_COLON, *position);
			return NULL;
		}
		(*position) = pos3+1;
		
		// Find value
		Json* value = this->parse_value( s, position );
		if( value == NULL )
			return NULL;
		
		// Create new key
		m_keys[key] = value;

		// Check for end of object or new key/value item
		unsigned int pos = s->find_first_of(",}", *position );
		if( pos == string::npos )
		{
			this->m_error->set_error_code(kJSON_ERR_OBJECT,*position);
			return NULL;
		}
		(*position) = pos+1;
		
		// End of object?
		if( s->at(pos) == '}' )
			break;
	}
	
	return this;
}

Json* JsonObject::key_value( string key )
{
	map<string,Json*>::iterator it = this->m_keys.find(key);
	if( it == this->m_keys.end() )
		return NULL;
	else
		return it->second;
}

/*
* JsonArray class
*
*/
JsonArray::JsonArray( JsonError* error ) : Json(error,kJSON_ARRAY)
{
}

JsonArray::~JsonArray()
{
	this->free();
}

void JsonArray::free()
{
	for( unsigned int i=0; i<this->m_values.size(); i++ )
		delete this->m_values.at(i);
	
	this->m_values.clear();
}

Json* JsonArray::get_at( int index )
{
	if( index < 0 )
		return NULL;
	
	if( (unsigned int)index >= this->m_values.size() )
		return NULL;
	
	return this->m_values.at(index);
}

Json* JsonArray::parse( string* s, unsigned int* position )
{
	while( true )
	{
		Json* value = this->parse_value( s, position );
		if( value == NULL )
			return NULL;
		
		this->m_values.push_back( value );
		
		int pos = s->find_first_of(",]",*position);
		if( pos == -1 )
		{
			this->m_error->set_error_code( kJSON_ERR_ARRAY, *position );
			return NULL;
		}
		(*position) = pos+1;
		
		// End of array?
		if( s->at(pos) == ']' )
			break;
	}
	
	return this;
}

/*
* class JsonError
*
* Class to manage type of errors
*
*/
JsonError::JsonError()
{
	this->m_nCode		= kJSON_ERR_NO_ERROR;
	this->m_nPosition	= 0;
}

void JsonError::set_error_code( JSON_ERROR nCode, int nPosition )
{
	this->m_nCode	= nCode;
	this->m_nPosition	= nPosition;
	switch( this->m_nCode )
	{
		case kJSON_ERR_NO_ERROR		: this->m_sMsg = "No error"; break;
		case kJSON_ERR_STRING		: this->m_sMsg = "String error"; break;
		case kJSON_ERR_COLON		: this->m_sMsg = "Syntax error: missing colon (:)"; break;
		case kJSON_ERR_OBJECT		: this->m_sMsg = "Syntax error: mssing , or } in object"; break;
		case kJSON_ERR_ARRAY		: this->m_sMsg = "Syntax error: missing , or ] in array"; break;
		case kJSON_ERR_PARSE_VALUE	: this->m_sMsg = "Syntax error while parsing value"; break;
		case kJSON_ERR_NO_DATA		: this->m_sMsg = "No data found in string"; break;
		default						:
			{
				char msg[250];
				sprintf( msg, "Error code not found : %02d", this->m_nCode );
				this->m_sMsg = msg;
				break;
			}
	}
}

/*
* json_decode
*
* String to be decodificated
*
*/
Json* json_decode( char* str, JsonError* json_error )
{
	Json* 			root		= NULL;
	unsigned int	position	= 0;
	string			s;
	
	// Set the string
	s.assign( str );

	while( position < s.length() )
	{
		switch( s[position++] )
		{
			case '{': root = new JsonObject(json_error); break;
			case '[': root = new JsonArray(json_error); break;
		}
		if( root != NULL )
			break;
	}

	if( root != NULL )
	{
		if( root->parse( &s, &position ) == NULL )
		{
			delete root;
			root = NULL;
		}
	}
	else
	{
		json_error->set_error_code(kJSON_ERR_NO_DATA, position);
	}

	return root;
}
