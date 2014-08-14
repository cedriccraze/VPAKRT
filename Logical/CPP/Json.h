/********************************************************************
 * COPYRIGHT -- Pollux
 ********************************************************************
 * Library: CPP
 * File: Json.h
 * Author: CEDRIC
 * Created: July 30, 2014
 *******************************************************************/

#ifndef __JSON__
#define __JSON__

#include <bur/plctypes.h>
#include <vector>
#include <string>
#include <map>

using namespace std;

typedef enum
{
	kJSON_ERR_NO_ERROR,
	kJSON_ERR_STRING,		// Missing "
	kJSON_ERR_COLON,		// Missing :
	kJSON_ERR_OBJECT,		// Missing , or }
	kJSON_ERR_ARRAY,		// Missing , or ]
	kJSON_ERR_PARSE_VALUE,	// Syntax error while parsing value
	kJSON_ERR_NO_DATA,		// No data found
}JSON_ERROR;

class JsonError
{
	private:
	string m_sMsg;
	JSON_ERROR m_nCode;
	int m_nPosition;
	
	public:
	JsonError();
	
	const char *get_error_string() { return this->m_sMsg.c_str(); }
	JSON_ERROR get_error_code() { return this->m_nCode; }
	int get_position() { return this->m_nPosition; }
	void set_error_code( JSON_ERROR nCode, int position );
};

/* types */
typedef enum {
	kJSON_OBJECT,
	kJSON_ARRAY,
	kJSON_STRING,
	kJSON_INTEGER,
	kJSON_REAL,
	kJSON_TRUE,
	kJSON_FALSE,
	kJSON_NULL
} JSON_TYPE;

class Json
{
	private:
	JSON_TYPE m_type;
	protected:
	JsonError* m_error;
	
	protected:
	void set_type( JSON_TYPE type ) { m_type = type; };
	string parse_string( string* s, unsigned int* position, char* chars=NULL );
	Json* parse_value( string* s, unsigned int* position );
	public:
	Json( JsonError* error, JSON_TYPE type );
	virtual ~Json();
	
	virtual void free();
	JSON_TYPE get_type() { return m_type; };
	virtual Json* parse( string* s, unsigned int* position ) { return NULL; };
};

class JsonValue : public Json
{
	private:
	string m_string;
	
	protected:
	void set_string( string value );
	public:
	JsonValue( JsonError* error, JSON_TYPE type, string value );
	virtual ~JsonValue();
	
	void free();
	const char* string_value() { return this->m_string.c_str(); }
	int int_value() { return atoi(this->m_string.c_str()); };
	double float_value() { return atof(this->m_string.c_str()); }
	bool bool_value() { if( Json::get_type() == kJSON_TRUE ) return true; else return false; }
};

class JsonObject : public Json
{
	private:
	map<string,Json*> m_keys;
	
	public:
	JsonObject( JsonError* error );
	virtual ~JsonObject();
	
	void free();
	Json* parse( string* s, unsigned int* position );
	Json* key_value( string key );
};

class JsonArray : public Json
{
	private:
	vector<Json*> m_values;
	
	public:
	JsonArray( JsonError* error );
	virtual ~JsonArray();
	
	void free();
	Json* parse( string* s, unsigned int* position );
	Json* get_at( int index );
	int getsize() { return this->m_values.size(); }
};

Json* json_decode( char* string, JsonError* error );

#endif

