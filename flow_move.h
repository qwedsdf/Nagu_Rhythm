#pragma once
#include"Dxlib.h"
#include"Flow.h"

#define MAX_FIST_SIZE 1
#define MAX_CHANGE 0.7
#define AXELL 0.002
class flow_move
{
private:
	int fist;
	bool Exflg;
	int alpha;
	int start_time;
	double spd;
	double ExtRate;
	int anglecount;
	bool changeflg;
public:
	flow_move();
	~flow_move();

	void format();

	void Action();
	void Draw();

	void setExflg(bool lExflg){ Exflg = lExflg; }
	void setchangeflg(bool lchangeflg){ changeflg = lchangeflg; }

	double getExtRate(){ return ExtRate; }
	bool getchangeflg(){ return changeflg; }
	bool getExflg(){ return Exflg; }
};

