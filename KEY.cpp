#include "KEY.h"



KEY::KEY()
{
	for (int i = 0; i < 256; i++){
		Buf[i] = 0;
		cheack[i] = false;
	}
}


KEY::~KEY()
{
}

//キーの更新（毎フレーム通るようにどこかに書く）
void KEY::UpDateKey(){
	for (int i = 0; i < 256; i++){
		Buf[i]>0 ? cheack[i] = true : cheack[i] = false;
	}
	//キーの更新
	GetHitKeyStateAll(Buf);

	/*この先マウス処理*/
	mousesave = mousenow;

	mousenow = GetMouseInput();

	GetMousePoint(&mouseX, &mouseY);//マウスの座表をゲット！
}

//単押し判定
bool KEY::OnePushKey(char KEY){
	if (Buf[KEY] == 1 && !cheack[KEY]){
		cheack[KEY] = true;
		return true;
	}
	return false;
}

//マウスの処理
bool KEY::GetMousePush(int input){
	if ((mousenow & input) != 0){
		return true;
	}
	else {
		return false;
	}
}

bool KEY::GetMouseOnePush(int input){
	if (GetMousePush(input) && (mousesave & input) == 0){
		return true;
	}
	else{
		return false;
	}
}
