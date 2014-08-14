/********************************************************************
 * COPYRIGHT -- Pollux
 ********************************************************************
 * Library: CPP
 * File: Timer.h
 * Author: CEDRIC
 * Created: July 17, 2014
 *******************************************************************/

#include <bur/plctypes.h>
#include <standard.h>
class Timer
{
	private:
	TON_typ fubTON;
	bool started;
	
	public:
	Timer();
	Timer( signed long time );
	~Timer();
	
	void cyclic();
	signed long getElapsedTime();
	bool getOutput();
	bool isStarted() const { return this->started; }
	void setTime( signed long time );
	void start();
	void stop();
};
