#pragma once
#include"flow_move.h"
#include"Flow.h"
#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include"flow_game.h"
#include"selectf.h"
class title :public Flow
{
private:
	flow_game *fgm;
	selectf *sel;
	flow_move* fmove;
	bool once;
	int pushtime;
	int g_push_start;
	int pushSE;
public:
	title(KEY *gkey, date *alldate, flow_game *gm, selectf *sel, flow_move *fmv);
	~title();

	int action();
	void draw();
};

