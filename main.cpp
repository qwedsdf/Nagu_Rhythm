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


/////////////////////FPS図るやつ///////////////////////
class Fps{
	int mStartTime;         //測定開始時刻
	int mCount;             //カウンタ
	float mFps;             //fps
	int a[60];
	int get[60];
	static const int N = 60;//平均を取るサンプル数
	static const int FPS = 60;	//設定したFPS

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
		int waitTime = (mCount+1) * 1000 / FPS - tookTime;	//待つべき時間
		if (waitTime > 0){
			Sleep(waitTime);	//待機
		}
		get[mCount] = waitTime;
	}
};

Fps fps;

void SetWindowSize(){
	momNo = FLOW_LOAD;//使いたかったらここの番号を変えて

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
	// 非同期読み込み設定に変更
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


	//録音用のFLOW。使いたかったらループに後で入れて
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

// プログラムは WinMain から始まります
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	SetWindowSize();
	SetGraphMode(/*横*/window_X,/*縦*/ window_Y, /*カラービットレート*/16,/*フレーム数*/ 60);
	ChangeWindowMode(true);
	if (DxLib_Init() == -1)		// ＤＸライブラリ初期化処理
	{
		return -1;			// エラーが起きたら直ちに終了
	}
	SetDrawScreen(DX_SCREEN_BACK);
	init();
	while (ProcessMessage() != -1){
		
		// 画面を初期化する
		ClearDrawScreen();

		//時間の更新
		tm->updatetime();
		//描画
		draw();

		//動き
		action();

		//キーの更新
		gkey->UpDateKey();

		ac->UpDateBotton();

		//時間の更新
		tm->GetStartTime();
		
		// 裏画面の内容を表画面に反映させる
		ScreenFlip();


		if (CheckHitKey(KEY_INPUT_ESCAPE) == 1){
			break;
		}

	}

	DxLib_End();				// ＤＸライブラリ使用の終了処理

	return 0;				// ソフトの終了 
}
