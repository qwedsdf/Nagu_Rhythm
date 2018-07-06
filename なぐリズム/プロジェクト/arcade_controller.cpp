#include "arcade_controller.h"


arcade_controller::arcade_controller()
{
	for (int i = 0; i < 14; i++){
		Buf[i] = 0;
		cheack[i] = false;
	}
}


arcade_controller::~arcade_controller()
{
}

void arcade_controller::GetHitBottonStateAll(){
	
	int Pad1 = GetJoypadInputState(DX_INPUT_PAD1);
	Buf[0] = Pad1&PAD_INPUT_1;
	Buf[1] = Pad1&PAD_INPUT_2;
	Buf[2] = Pad1&PAD_INPUT_3;
	Buf[3] = Pad1&PAD_INPUT_4;
	Buf[4] = Pad1&PAD_INPUT_5;
	Buf[5] = Pad1&PAD_INPUT_6;
	Buf[6] = Pad1&PAD_INPUT_7;
	Buf[7] = Pad1&PAD_INPUT_8;
	Buf[8] = Pad1&PAD_INPUT_9;
	Buf[9] = Pad1&PAD_INPUT_10;
	Buf[10] = Pad1&PAD_INPUT_11;
	Buf[11] = Pad1&PAD_INPUT_12;
	Buf[12] = Pad1&PAD_INPUT_13;
	Buf[13] = Pad1&PAD_INPUT_14;

	
}

void arcade_controller::UpDateBotton(){
	for (int i = 0; i < 14; i++){
		Buf[i]!=0 ? cheack[i] = true : cheack[i] = false;
	}
	
	GetHitBottonStateAll();
	
}

bool arcade_controller::OnePushBotton(int num){
	int connum = num - 1;
	if (Buf[connum] != 0 && !cheack[connum]){
		cheack[connum] = true;
		return true;
	}
	return false;
}