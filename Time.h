#pragma once
#include<stdio.h>
#include <time.h>
class Time
{
private:
	float start;
	float b_time;
public:
	void updatetime();
	void GetStartTime();
	float getframetime(){ return (b_time - start) / 1000; }

	Time();
	~Time();
};

