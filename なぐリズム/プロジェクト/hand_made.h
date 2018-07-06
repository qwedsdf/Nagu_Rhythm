#pragma once
#include"Flow.h"
#include"notes.h"
#include<vector>
#include <stdio.h>
using namespace std;
//ŽÀŽžŠÔ‚ð‘ª‚é‚Ì‚ÉŽg‚¤
#include<windows.h>
#include<time.h>


class hand_made :public Flow
{
private:
	int count;
	bool recordingflg;
	vector<int> time;
	vector<int> id;
	clock_t start;
public:
	hand_made(date *alldate, KEY *gkey, arcade_controller *ac);
	~hand_made();
	bool action();
	void Draw();
	void tapcheck(bool tap[3]);

	void set_recordingflg(bool flg){ recordingflg = flg; }

	clock_t get_start(){ return start; }
	bool get_recordingflg(){ return recordingflg; }
};



