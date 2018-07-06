#pragma once
#include"math.h"
#include"Flow.h"
#include"flow_game.h"
#include"flow_move.h"

#define MOVE_SELECT_SPD 50

#define NONE_MOVE 0
#define LEFT_MOVE 1
#define RIGHT_MOVE 2
#define CENTER_MOVE 3


#define WAKU_SIZE 500
#define WAKU_Y WINDOW_MAX_Y / 2

//AJM == AdJustMent (調整)
#define WAKU_TITLE_AJM_X 100

//AJM == AdJustMent (調整)
#define WAKU_TITLE_AJM_Y 90

#define BOKO_LEFT_POSX 150
#define BOKO_RIGHT_POSX 810

//左右のぼこちゃん
/*潜っている深さ*/
#define BOKO_DEPTH 60
/*最初のY座標*/
#define FIRST_POSY WINDOW_MAX_Y - 100 + BOKO_DEPTH
/*最初のjumpの値*/
#define FIRST_JUMP 0

#define JUMP_SPD 1
#define JUMP_FIRST_MOSTION 10


class selectf :public Flow
{
private:
	bool left_jump_flg;
	bool right_jump_flg;
	bool first_flg;
	flow_game *fgm;
	//vector<notedates> notedate;
	int x_waku[5];
	double waku_bai[5];
	int count;
	int move_flg;
	int spd_all;
	int center_num;
	int move_now;
	int start_time;
	int volume;
	int arrow_move;
	double jump_spd_L;
	double jump_spd_R;

	//左右のぼこちゃんのY座標
	double left_boko_posY;
	double right_boko_posY;

	flow_move* fmove;

	//画像
	int back;
	int rignt_arrow;
	int left_arrow;
	int boko_L;
	int boko_R;
	int mask_black;

	int BeginCal;
	
public:
	selectf(KEY *gkey, date *alldate, flow_game *gm, flow_move *fmv);
	~selectf();

	void format();
	void jump_boko();
	void move_frame();
	int action();
	void draw();
	void load();
	void count_set();
	void stop_sound();
	void draw_title();
	void set_music_inf();
	//int first_music(){ return notedate[count].music; }
};


