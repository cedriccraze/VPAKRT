/********************************************************************
 * COPYRIGHT -- Pollux
 ********************************************************************
 * Library: CPP
 * File: SerialCodes.cpp
 * Author: CEDRIC
 * Created: August 01, 2014
 *******************************************************************/

#include <bur/plctypes.h>
#include <queue>
#include <string>
#include "SerialCodes.h"

queue_serial* createSerialQueue()
{
	queue_serial* q = new queue_serial;
	return q;
}

int getSerialQueueSize( queue_serial* q )
{
	int size	= q->size();
	return size;
}

void pushSerialCodeToQueue( queue_serial* q, const char* serial )
{
	string s = serial;
	q->push(s);
}

const char* popSerialCodeFromQueue( queue_serial* q )
{
	string s = q->front();
	q->pop();
	return s.c_str();
}

void freeSerialQueue( queue_serial* q )
{
	delete q;
	q = NULL;
}

