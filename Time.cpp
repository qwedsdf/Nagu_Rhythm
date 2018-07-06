#include "Time.h"


Time::Time()
{
	start = 0;
	b_time = 0;
}


Time::~Time()
{
}

void Time::updatetime(){
	b_time = (float)clock();
}

void Time::GetStartTime(){
	start = (float)clock();
}
