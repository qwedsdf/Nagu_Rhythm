#pragma once
#include"date.h"
#include"KEY.h"
#include"arcade_controller.h"
#include <stdlib.h>

using namespace std;
class Flow
{
private:
	double x;
	double y;
protected:
	date* dt;
	KEY *key;
	arcade_controller *ac;
	int music_now;

public:
	Flow();
	~Flow();
};

