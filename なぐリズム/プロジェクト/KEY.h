#pragma once
#include "DxLib.h"
class KEY
{
private:

public:
	//変数
	char Buf[256];
	bool cheack[256];
	//マウス関係
	int mousenow;
	int mousesave;
	int mouseX = 0;
	int mouseY = 0;
	//関数
	//キー
	void UpDateKey();
	bool OnePushKey(char Key);

	//マウス
	bool GetMousePush(int input);
	bool GetMouseOnePush(int input);
	
	int getMousePointX(){ return mouseX; }
	int getMousePointY(){ return mouseY; }
	KEY();
	~KEY();
};
