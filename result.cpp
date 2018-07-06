#include "result.h"


result::result(arcade_controller *lac, KEY *gkey, flow_game *fm, date *alldate)
{
	ac = lac;
	key = gkey;
	fgm = fm;
	dt = alldate;
	once_flg = false;
	wait = false;
	last = false;
	Evaluation = false;
	wait_time = 0;
	comment_wait_time = 0;
	star_count = 0;
	stamp_ExtRate = 10;
	for (int i = 0; i < 5; i++){
		char buf[256];
		sprintf(buf, "music/SE/voice/last_%d.wav", i);
		voice_last[i] = LoadSoundMem(buf);
	}
	for (int i = 0; i < 250; i++){
		star[i] = new starinf();
		star[i]->star_posX = rand() % WINDOW_MAX_X;
		star[i]->star_posY = rand() % WINDOW_MAX_Y;
		star[i]->alpha = 255;
		star[i]->alpha_flg = false;
		star[i]->num = 0;
	}
	comment_X = COMMENT_POSX;
	g_back = LoadGraph("image/result/result_back.png");
	g_last = LoadGraph("image/think_you.png");
	level = LoadGraph("image/result/kobusi_red.png");
	star_img = LoadGraph("image/result/stamp/star.png");

	result_decision = LoadSoundMem("music/SE/result.mp3");
	rank_up = LoadSoundMem("music/SE/rank_up.mp3");
	count = LoadSoundMem("music/SE/count.mp3");


}


result::~result()
{
}

void result::format(){
	once_flg = false;
	wait = false;
	last = false;
	Evaluation = false;
	wait_time = 0;
	comment_wait_time = 0;
	star_count = 0;
	stamp_ExtRate = 10;
}

int result::action(){
	if (!once_flg){
		once_flg = true;
		fgm->set_score(all_score);
		fgm->set_score(score_motion);
		fgm->format();
	}

	int rank_b = rank;

	//どのランクか判断
	int unko = all_score[SCORE] - score_motion[SCORE];
	int rank_k = dt->Getscore_standard(fgm->get_selectnum()) / 7;
	if (unko > dt->Getscore_standard(fgm->get_selectnum()) - rank_k)rank = 4;
	else if (unko > dt->Getscore_standard(fgm->get_selectnum()) - rank_k*3)rank = 3;
	else if (unko > dt->Getscore_standard(fgm->get_selectnum()) - rank_k*4)rank = 2;
	else if (unko > dt->Getscore_standard(fgm->get_selectnum()) - rank_k*5)rank = 1;
	else rank = 0;

	if (rank != rank_b)PlaySoundMem(rank_up,DX_PLAYTYPE_BACK);

	if (wait){
		wait_time++;
		if (wait_time > 15){
			wait_time = 0;
			wait = false;
		}
		return FLOW_RESULT;
	}

	//スコアをカウントアップさせる
	for (int i = 4; i >= 0; i--){
		if (score_motion[i] != 0){
			int back = score_motion[i] % 7;
			score_motion[i] -= 5;
			if (i == 0){
				back = score_motion[i] % 1000;
				score_motion[i] -= 300;
				if (back<score_motion[i] % 1000)PlaySoundMem(count,DX_PLAYTYPE_BACK);
			}
			else{
				if (back<score_motion[i] % 7)PlaySoundMem(count,DX_PLAYTYPE_BACK);
			}

			if (score_motion[i] <= 0){
				score_motion[i] = 0;
				if (i == 0){
					PlaySoundMem(result_decision, DX_PLAYTYPE_BACK);
					Evaluation = true;
				}
				wait = true;
			}
			break;
		}
	}

	//きらきらの透過と座標を設置
	for (int i = 0; i < 250; i++){
		//透過
		if (star[i]->alpha_flg){
			star[i]->alpha -= 2;
			if (star[i]->alpha < 0){
				star[i]->alpha = 255;
				star[i]->alpha_flg = false;
			}
			continue;
		}

		//座標設置
		int num = rand() % 60;
		if (CheckHitKey(KEY_INPUT_SPACE)){
			if (num == 1){
				star[i]->star_posX = rand() % 164;
				star[i]->star_posY = rand() % 164;
				star[i]->num = 1;
				star[i]->alpha_flg = true;
			}
			if (num == 2 || num == 5 || num == 10){
				star[i]->star_posX = rand() % 308;
				star[i]->star_posY = rand() % 530;
				star[i]->num = 2;
				star[i]->alpha_flg = true;
			}
			if (num == 3){
				star[i]->star_posX = rand() % 450;
				star[i]->star_posY = rand() % 120;
				star[i]->num = 3;
				star[i]->alpha_flg = true;
			}
		}
		else{
			if (num == 1){
				star[i]->star_posX = rand() % 164;
				star[i]->star_posY = rand() % 164;
				star[i]->num = 1;
				star[i]->alpha_flg = true;
			}
		}
		
	}

	//シーン切り替え
	if (fgm->tapcheck() == CENTER_SELECT){
		for (int i = 4; i >= 0; i--){
			if (score_motion[i] == 0)continue;
			score_motion[i] = 0;
			if (i == 0){
				PlaySoundMem(result_decision, DX_PLAYTYPE_BACK);
				Evaluation = true;
			}
			else dt->s_sean_change();
			return FLOW_RESULT;
		}
		if (!last){
			last = true;
			StopSoundMem(voice_last[rank]);
			PlaySoundMem(dt->s_think_voice(),DX_PLAYTYPE_BACK);
			dt->s_sean_change();
			return FLOW_RESULT;
		}
		format();
		dt->s_sean_change();
		return FLOW_TITLE;
	}
	if (Evaluation&&!CheckSoundMem(result_decision)){
		PlaySoundMem(voice_last[rank], DX_PLAYTYPE_BACK);
		Evaluation = false;
	}
	return FLOW_RESULT;
}

void result::Draw(){
	DrawGraph(0, 0, g_back, true);

	int comment_X_save = comment_X % WINDOW_MAX_X;
	DrawGraph(comment_X_save, 12, dt->g_Getcomment(rank), false);
	DrawGraph(comment_X_save + WINDOW_MAX_X, 12, dt->g_Getcomment(rank), false);

	if (comment_wait_time != 0){
		comment_wait_time++;
		comment_wait_time %= 60;
	}
	else comment_X -= 10;

	if (comment_X % WINDOW_MAX_X > comment_X_save){
		comment_wait_time = 1;
	}

	DrawGraph(87, 185, dt->Getresult_title(fgm->get_selectnum()), false);

	//スコアを表示
	fgm->DrawNumber(425, 50, 341, (float)(all_score[SCORE] - score_motion[SCORE]), 1);
	fgm->DrawNumber(470, 18, 469, (float)(all_score[MAX_COMBO] - score_motion[MAX_COMBO]), 2);
	fgm->DrawNumber(470, 18, 503, (float)(all_score[MAX_PARFECT] - score_motion[MAX_PARFECT]), 2);
	fgm->DrawNumber(470, 18, 537, (float)(all_score[MAX_GOOD] - score_motion[MAX_GOOD]), 2);
	fgm->DrawNumber(470, 18, 571, (float)(all_score[MAX_BAD] - score_motion[MAX_BAD]), 2);

	//スコアに応じて表示の画像
	DrawGraph(593, 112, dt->g_Getillustration(rank), false);
	if (score_motion[SCORE] == 0){
		if (stamp_ExtRate > 1){
			stamp_ExtRate --;
			if (stamp_ExtRate <= 1){
				stamp_ExtRate = 1;
				dt->s_sean_change();
			}

		}
		DrawRotaGraph(80 + 82, 445 + 82, stamp_ExtRate, 0.0, dt->g_Getstamp(rank), true);
		//きらきらの描写
		for (int i = 0; i < rank * 5; i++){
			if (!star[i]->alpha_flg)continue;
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, star[i]->alpha);
			if (CheckHitKey(KEY_INPUT_SPACE)){
				switch (star[i]->num)
				{
				case 1:
					DrawGraph(star[i]->star_posX + 78, star[i]->star_posY + 445, star_img, true);
					break;
				case 2:
					DrawGraph(star[i]->star_posX + 593, star[i]->star_posY + 112, star_img, true);
					break;
				case 3:
					DrawGraph(star[i]->star_posX + 87, star[i]->star_posY + 145, star_img, true);
					break;
				default:
					break;
				}
			}
			else DrawGraph(star[i]->star_posX + 78 - 20, star[i]->star_posY + 445 - 20, star_img, true);
		}
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
		star_count++;
	}

	for (int i = 0; i < dt->Getlevel(fgm->get_selectnum()); i++){
		DrawGraph(87 + i * 34, 135, level, true);
	}

	if (last)DrawGraph(0, 0, g_last,false);
}
