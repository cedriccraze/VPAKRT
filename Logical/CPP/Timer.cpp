/********************************************************************
 * COPYRIGHT -- Pollux
 ********************************************************************
 * Library: CPP
 * File: Timer.cpp
 * Author: CEDRIC
 * Created: July 17, 2014
 *******************************************************************/

#include <bur/plctypes.h>

#include <string.h>
#include "Timer.h"

Timer::Timer()
{
	memset( &this->fubTON, 0, sizeof(this->fubTON) );
	this->started = false;
}

Timer::Timer( signed long time )
{
	memset( &this->fubTON, 0, sizeof(this->fubTON) );
	this->started = false;
	this->setTime( time );
}

Timer::~Timer()
{
}

void Timer::cyclic()
{
	TON(&this->fubTON);
}

signed long Timer::getElapsedTime()
{
	return this->fubTON.ET;
}

bool Timer::getOutput()
{
	return this->fubTON.Q;
}

void Timer::setTime( signed long time )
{
	this->fubTON.PT = time;
}

void Timer::start()
{
	this->fubTON.IN = true;
	this->started = true;
}

void Timer::stop()
{
	this->fubTON.IN = false;
	this->started = false;
}
