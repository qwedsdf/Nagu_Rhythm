#pragma once
#include "DxLib.h"
class KEY
{
private:

public:
	//�ϐ�
	char Buf[256];
	bool cheack[256];
	//�}�E�X�֌W
	int mousenow;
	int mousesave;
	int mouseX = 0;
	int mouseY = 0;
	//�֐�
	//�L�[
	void UpDateKey();
	bool OnePushKey(char Key);

	//�}�E�X
	bool GetMousePush(int input);
	bool GetMouseOnePush(int input);
	
	int getMousePointX(){ return mouseX; }
	int getMousePointY(){ return mouseY; }
	KEY();
	~KEY();
};
