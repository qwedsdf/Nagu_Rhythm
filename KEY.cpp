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

//�L�[�̍X�V�i���t���[���ʂ�悤�ɂǂ����ɏ����j
void KEY::UpDateKey(){
	for (int i = 0; i < 256; i++){
		Buf[i]>0 ? cheack[i] = true : cheack[i] = false;
	}
	//�L�[�̍X�V
	GetHitKeyStateAll(Buf);

	/*���̐�}�E�X����*/
	mousesave = mousenow;

	mousenow = GetMouseInput();

	GetMousePoint(&mouseX, &mouseY);//�}�E�X�̍��\���Q�b�g�I
}

//�P��������
bool KEY::OnePushKey(char KEY){
	if (Buf[KEY] == 1 && !cheack[KEY]){
		cheack[KEY] = true;
		return true;
	}
	return false;
}

//�}�E�X�̏���
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
