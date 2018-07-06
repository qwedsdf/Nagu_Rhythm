#pragma once
#include"Flow.h"
#include"notes.h"
#include"hand_made.h"
#include <stdio.h>
#include<vector>
using namespace std;
//実時間を測るのに使う
#include<windows.h>
#include<time.h>
#include <iostream>
#include <fstream>

//////////////ノーツ配置関係//////////////////
#define ADJUST_X 35

#define LEFT 35 + ADJUST_X+ ADJUST_X
#define LEFT_CENTER 123 + ADJUST_X+ ADJUST_X
#define RIGHT_CENTER 208 + ADJUST_X+ ADJUST_X
#define RIGHT 297 + ADJUST_X+ ADJUST_X

#define BACK_X 450


#define INTERVAL 645  //拍の間隔  *1秒で645p進む*

#define BOTAN_X 1350
#define BOTAN_Y 750

#define BOTAN_BACK_X 30
#define BOTAN_BACK_Y 750

#define PAGE_LONG 1350

#define SAVE_LINE 775

#define SAVE_NOTES_SPD 250 / 40

#define NOTE_MOVE_SPD 5

#define WHEEL_SPD 30


class note_memory :public Flow
{
private:
	int kasoru;
	int backY;
	int count;
	int buf;
	int line_y;
	int position;
	int move_next;
	int page_count;
	int save_time;
	int notes_line;
	int stop_time;
	hand_made *hand;
	//同時押しノーツを作るとに使う
	int first_note;
	int second_note;
	//連打ノーツを作るときに使う
	int first_pos;
	int second_pos;

	bool recordingflg;
	bool loadflg;
	bool moveflg;
	bool movebackflg;
	bool stopflg;

	vector<int> x;
	vector<int> y;
	vector<int> id;
	vector<bool> moveflg_note;
	vector<int> length;

	vector<int> time;
	vector<int> ltime;
	vector<int> lid;
	vector<int> llength;
	clock_t start;
public:
	note_memory(date *alldate, KEY *gkey, arcade_controller *ac, hand_made *hd);
	~note_memory();
	void action();
	void Draw();
	void format();

	void load();
	void save();
	void note_on();
	void move_page();
	void save_hand();
	void set_note_flg();//moveflg_noteとlenghを入れる
	void QSort(int left, int right);
};



