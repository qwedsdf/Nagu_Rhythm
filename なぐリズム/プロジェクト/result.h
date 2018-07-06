#pragma once
#include"Flow.h"
#include"flow_game.h"

#define COMMENT_POSX 121


struct starinf
{
	bool alpha_flg;
	int star_posX;
	int star_posY;
	int alpha;
	int num;
};

class result:public Flow
{
private:
	//画像
	int g_back;
	int g_last;
	int level;
	int comment_X;
	int star_img;
	int all_score[5];
	int score_motion[5];
	int rank;
	int wait_time;
	int comment_wait_time;
	int star_count;
	float stamp_ExtRate;
	bool once_flg;//このフローに入って1F目かどうか
	bool wait;
	bool last;
	bool Evaluation;
	flow_game *fgm; 
	starinf *star[250];

	//SE
	int result_decision;
	int rank_up;
	int count;

	//ボイス
	int voice_last[5];
	
public:
	void format();
	void set_score();
	int action();
	void Draw();

	result(arcade_controller *ac, KEY *gkey, flow_game *fm, date *alldate);
	~result();
};