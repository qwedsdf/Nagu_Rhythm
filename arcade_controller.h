#pragma once
#include <string>
#include <sstream>
#include"Dxlib.h"
#include"date.h"
class arcade_controller
{
private:
	int Buf[14];
	bool cheack[14];
public:
	arcade_controller();
	~arcade_controller();

	void GetHitBottonStateAll();
	void UpDateBotton();
	bool OnePushBotton(int num);
};

