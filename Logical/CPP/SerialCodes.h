/********************************************************************
 * COPYRIGHT -- Pollux
 ********************************************************************
 * Library: CPP
 * File: SerialCodes.h
 * Author: CEDRIC
 * Created: August 01, 2014
 *******************************************************************/

#include <bur/plctypes.h>
#include <queue>

using namespace std;
typedef queue<string> queue_serial;

// Return the number of serial codes
queue_serial* createSerialQueue();
int getSerialQueueSize( queue_serial* q );
void pushSerialCodeToQueue( queue_serial* q, const char* serial );
const char* popSerialCodeFromQueue(queue_serial* q);
void freeSerialQueue( queue_serial* q);

