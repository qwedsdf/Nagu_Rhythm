#pragma once
#include"Dxlib.h"
#include"date.h"
#include"Time.h"
#include"date.h"


#define ID0 360
#define ID1 480
#define ID2 600

#define LINE_POS_UP_DEF LINE_POS_UP-250
#define LINE_POS_DWON_DEF 400-250

class notes
{
	double x;
	double y;
	int length;
	double spd;
	bool flg;
	bool pushflg;//ノーツが押されて後の演出中にノーツの当たり判定が出るのを防ぐ
	int id;
	int ani_count;
	int type;
	int g_judg;
	int time;
	//エフェクト用
	int alpha;
	float size;
	//連打用
	vector<int> ani_frame;
	vector<int> lx;
	vector<int> ly;
	vector<int> alpha_l;

	date* dt;
	Time* ti;
public:
	notes(date* dt, Time* ti);
	~notes();

	void format();
	bool action(float spd);
	void setflg(bool flg);
	void setID(int num){	id = num;	}
	void setType(int ty);
	void setPosX();
	void setLength(int leng);
	void Draw();
	bool outcheck();
	void Deathcount();
	void reset_cheack();
	void set_perfect(){ g_judg = dt->g_Getperfect(); }
	void set_good(){ g_judg = dt->g_Getnice(); }
	void set_time(int ltime){ time = ltime; }
	void set_animation(int lnum);//連打用アニメーションで使う値をセット

	//数値検出関数
	bool getflg(){ return flg; }
	bool getpushflg(){ return pushflg; }
	double getPosY(){ return y; }
	double getPosX(){ return x; }
	int getLength(){ return length; }
	int getID(){ return id; }
	int getType(){ return type; }
	int get_time(){ return time; }
};