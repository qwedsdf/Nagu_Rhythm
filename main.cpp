#include "DxLib.h"
#include"date.h"
#include"flow_game.h"
#include"note_memory.h"
#include"hand_made.h"
#include"KEY.h"
#include"arcade_controller.h"
#include"Time.h"
#include"title.h"
#include"selectf.h"
#include"result.h"
#include"flow_move.h"
#include"flow_load.h"

date* alldate;
flow_game* game;
note_memory* n_m;
KEY* gkey;
arcade_controller* ac;
hand_made* hd;
Time* tm;
title* titleflow;
selectf* selectflow;
result* resultFlow;
flow_move* moveFlow;
flow_load* loadFlow;

int momNo;
int window_X;
int window_Y;


/////////////////////FPS�}����///////////////////////
class Fps{
	int mStartTime;         //����J�n����
	int mCount;             //�J�E���^
	float mFps;             //fps
	int a[60];
	int get[60];
	static const int N = 60;//���ς����T���v����
	static const int FPS = 60;	//�ݒ肵��FPS

public:
	Fps(){
		mStartTime = GetNowCount();
		mCount = 0;
		mFps = 0;
		for (int i = 0; i < N; i++){
			a[i] = 1;
		}
	}

	bool Update(){
		a[mCount] = GetNowCount() - mStartTime;
		mStartTime = GetNowCount();
		mCount++;
		mCount %= N;
		int ave = 0;
		for (int i = 0; i < N; i++){
			ave += a[i];
		}
		ave /= N;
		mFps = 1000 / (float)ave;
		return true;
	}

	void Draw(){
		DrawFormatString(700, 0, GetColor(255, 0, 0), "%.1f", mFps);
		
	}

	void Wait(){
		int tookTime=0;
		for (int i = 0; i < mCount+1; i++){
			tookTime += a[i];
		}
		int waitTime = (mCount+1) * 1000 / FPS - tookTime;	//�҂ׂ�����
		if (waitTime > 0){
			Sleep(waitTime);	//�ҋ@
		}
		get[mCount] = waitTime;
	}
};

Fps fps;

void SetWindowSize(){
	momNo = FLOW_LOAD;//�g�����������炱���̔ԍ���ς���

	if (momNo == FLOW_MEMRY_ON){
		window_X = 400;
		window_Y = 800;
	}
	else{
		window_X = WINDOW_MAX_X;
		window_Y = WINDOW_MAX_Y;
	}
}


void init(){
	loadFlow = new flow_load();
	// �񓯊��ǂݍ��ݐݒ�ɕύX
	SetUseASyncLoadFlag(TRUE);
	alldate = new date();
	gkey = new KEY();
	ac = new arcade_controller();
	tm = new Time();
	moveFlow = new flow_move();
	game = new flow_game(alldate, gkey, ac, tm, moveFlow);
	selectflow = new selectf(gkey, alldate, game, moveFlow);
	titleflow = new title(gkey, alldate, game, selectflow, moveFlow);
	resultFlow = new result(ac, gkey, game, alldate);


	//�^���p��FLOW�B�g�����������烋�[�v�Ɍ�œ����
	hd = new hand_made(alldate, gkey, ac);
	n_m = new note_memory(alldate, gkey, ac, hd);
	SetUseASyncLoadFlag(FALSE);
}

void action(){
	int a = 0;
	switch (momNo)
	{
	case FLOW_LOAD:
		if (GetASyncLoadNum()==0)momNo = FLOW_TITLE;
		break;
	case FLOW_TITLE:
		momNo = titleflow->action();
		break;

	case FLOW_SELECT:
		momNo = selectflow->action();
		break;

	case FLOW_GAME:
		momNo = game->action();
		break;

	case FLOW_RESULT:
		momNo = resultFlow->action();
		break;

	case FLOW_MEMRY_PUSH:
		hd->action();
		break;

	case FLOW_MEMRY_ON:
		n_m->action();
		break;
	}
	moveFlow->Action();

	/*fps.Update();
	fps.Wait();*/
}

void draw(){
	switch (momNo)
	{
	case FLOW_LOAD:
		loadFlow->draw();
		break;
	case FLOW_TITLE:
		titleflow->draw();
		break;

	case FLOW_SELECT:
		selectflow->draw();
		break;

	case FLOW_GAME:
		game->Draw();
		break;

	case FLOW_RESULT:
		resultFlow->Draw();
		break;

	case FLOW_MEMRY_PUSH:
		hd->Draw();
		break;

	case FLOW_MEMRY_ON:
		n_m->Draw();
		break;
	}
	moveFlow->Draw();

	/*fps.Draw();*/
}

// �v���O������ WinMain ����n�܂�܂�
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	SetWindowSize();
	SetGraphMode(/*��*/window_X,/*�c*/ window_Y, /*�J���[�r�b�g���[�g*/16,/*�t���[����*/ 60);
	ChangeWindowMode(true);
	if (DxLib_Init() == -1)		// �c�w���C�u��������������
	{
		return -1;			// �G���[���N�����璼���ɏI��
	}
	SetDrawScreen(DX_SCREEN_BACK);
	init();
	while (ProcessMessage() != -1){
		
		// ��ʂ�����������
		ClearDrawScreen();

		//���Ԃ̍X�V
		tm->updatetime();
		//�`��
		draw();

		//����
		action();

		//�L�[�̍X�V
		gkey->UpDateKey();

		ac->UpDateBotton();

		//���Ԃ̍X�V
		tm->GetStartTime();
		
		// ����ʂ̓��e��\��ʂɔ��f������
		ScreenFlip();


		if (CheckHitKey(KEY_INPUT_ESCAPE) == 1){
			break;
		}

	}

	DxLib_End();				// �c�w���C�u�����g�p�̏I������

	return 0;				// �\�t�g�̏I�� 
}
