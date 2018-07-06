#include "flow_move.h"

flow_move::flow_move()
{
	fist = LoadGraph("image/move_scean/Kobusi.png");
	
	format();
}


flow_move::~flow_move()
{
}

void flow_move::format(){
	alpha = 0;
	ExtRate = 0.2;
	Exflg = false;
	changeflg = false;
	spd = 0.01;
	anglecount = 0;
}

void flow_move::Action(){
	//‘å‚«‚³‚Ì•Ï‰»
	if (Exflg){
		if (alpha < 255 && ExtRate <= MAX_CHANGE){
			alpha += 15;
			spd = 127 - alpha;
			ExtRate += spd/30000;
			if (alpha >= 255){
				spd = 0.01;
				start_time = clock();
			}
			return;
		}
		if (clock() - start_time < 100)return;
		spd += AXELL;
		if (ExtRate < MAX_FIST_SIZE)ExtRate += spd;
		if (ExtRate > MAX_FIST_SIZE){
			ExtRate = MAX_FIST_SIZE;
			Exflg = false;
		}
		if (ExtRate > MAX_CHANGE){
			if (alpha > 0)alpha -= 30;
			if (alpha < 0)alpha = 0;
		}
	}
	else{
		format();
	}

}

void flow_move::Draw(){
	if (Exflg){
		double angle = cos(anglecount * PI / 180) * (30 * PI / 180);
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
		DrawRotaGraph(WINDOW_MAX_X / 2, WINDOW_MAX_Y / 2, ExtRate, 0, fist, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
	
}
