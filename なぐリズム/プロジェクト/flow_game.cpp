#include "flow_game.h"


flow_game::flow_game(date *alldate, KEY *gkey, arcade_controller *lac, Time* tim, flow_move *fmv)
{
	
	key = gkey;
	dt = alldate;
	ac = lac;
	fmove = fmv;

	for (int i = 0; i < NOTES_MAX; i++){
		note[i] = new notes(alldate,tim);
	}

	format();

	//画像
	combo_frame		= LoadGraph("image/font/combo03.png");
	score_frame		= LoadGraph("image/font/score02.png");
	back_fream		= LoadGraph("image/game/back_fream.png");
	bonus_graph		= LoadGraph("image/effects/bonus.png");
	fiver_effect	= LoadGraph("image/effects/confetti.png");
	gradation[GRADITION_BAD]	= LoadGraph("image/game/play_bk_f.png");
	gradation[GRADITION_NOMAL]	= LoadGraph("image/game/play_bk_g.png");
	kcal_fream = LoadGraph("image/kcal_num/kcal_bk.png");
	pepar_ring = LoadGraph("image/game/ring.png");

	for (int i = 0; i < 2; i++){
		char buf[256];
		sprintf(buf, "image/game/hubuki/hubuki_0%d.png", i + 1);
		hubuki[i] = LoadGraph(buf);
	}
	for (int i = 0; i < MAX_BOKO; i++){
		char buf[256];
		sprintf(buf, "image/game/boko_%d-1.png", i+1);
		boko[i] = LoadGraph(buf);
		sprintf(buf, "image/game/boko_%d-2.png", i + 1);
		boko1[i] = LoadGraph(buf);
	}

	//音
	fiver_SE = LoadSoundMem("music/SE/se_maoudamashii_magical25.mp3");
	push_eir = LoadSoundMem("music/SE/push_eir.mp3");
	push_miss = LoadSoundMem("music/SE/miss.mp3");
}


flow_game::~flow_game()
{
}

void flow_game::format(){
	fiver_flg = false;
	combo_cheack = false;
	finish_flg = false;
	testplay = false;
	caloryY = -100;
	combo_image = 0;
	stop_time = -1;
	spd = 5;
	combo = 0;
	notecount = 0;
	starttime = 0;
	rank = 0;
	fiver_score = 100000000;//仮の値
	score = 0;
	combo_max = 0;
	parfect_max = 0;
	good_max = 0;
	bad_max = 0;
	panchcount = 0;
	StopSoundMem(dt->Getmusic(select_num));
	for (int i = 0; i < NOTES_MAX; i++){
		note[i]->format();
	}
}

//ノーツがタイミングよくボタンを押されたら消す処理(判定)
void flow_game::erase_effct(int num){
	int number=-1;//１番ちかいノーツの番号を入れる
	double min = GOOD+1;//1番ちかいノーツの距離をいれる変数
	int max_dif = GOOD + 1;
	for (int i = 0; i < NOTES_MAX; i++){
		if (note[i]->getType() != TYPE_BARRAGE &&(note[i]->getID() != num || !note[i]->getflg()))continue;

		//ローカル変数に差分をまとめて計算
		double difference;
		//連打の場合
		if (note[i]->getType() == TYPE_BARRAGE){
			difference = abs(note[i]->getPosY() - note[i]->getLength()/2 - LINE_POS_UP);
			if (difference < note[i]->getLength() / 2){
				score += 50;
				note[i]->set_animation(num);
				StopSoundMem(push_eir);
				PlaySoundMem(dt->s_pushSE(num), DX_PLAYTYPE_BACK);
				return;
			}
		}
		else{
			//difference = abs(note[i]->getPosY() - LINE_POS_UP);
			int dif = (clock() - (clock_t)starttime) - note[i]->get_time();
			difference = abs((clock() - (clock_t)starttime) - note[i]->get_time());

			//1番近いノーツの距離と番号を検出
			if (!note[i]->getpushflg() && difference < GOOD){
				if (min > difference && max_dif > dif){
					max_dif = dif;
					number = i;
					min = difference;
				}
			}
		}
	}

	//範囲にノーツがいなかったとき
	if (number == -1)return;

	//1番近いノーツを消す
	if (!note[number]->getpushflg()){
		combo++;
		if (combo_max < combo)combo_max = combo;
		int more = 3;
		if (bonus_cheack())more = 7;
		score += 50 + more * combo;
		if (min < PERFECT){
			note[number]->set_perfect(); 
			parfect_max++;
		}
		else if (min < GOOD){
			note[number]->set_good();
			good_max++;
		}
		note[number]->Deathcount();
		StopSoundMem(push_eir);
		PlaySoundMem(dt->s_pushSE(num), DX_PLAYTYPE_BACK);
	}
}

bool flow_game::bonus_cheack(){
	if (score%(fiver_score + 5000) > fiver_score){
		if (!fiver_flg){
			PlaySoundMem(fiver_SE, DX_PLAYTYPE_BACK);
			fiver_flg = true;
		}
		return true;
	}
	fiver_flg = false;
	return false;
}

int flow_game::tapcheck(){
	//あたり判定を入れて,,,上3つ

	if (ac->OnePushBotton(7) || key->OnePushKey(KEY_INPUT_Q)){
		return LEFT_SELECT;
	}
	if (ac->OnePushBotton(3) || key->OnePushKey(KEY_INPUT_W)){
		return CENTER_SELECT;
	}
	if (ac->OnePushBotton(8) || key->OnePushKey(KEY_INPUT_E)){
		return RIGHT_SELECT;
	}
	return NONE_SELECT;
}

//空うちセット
void flow_game::set_hit(int id){
	if (!finish_flg){
		panchcount++;
		PlaySoundMem(push_eir, DX_PLAYTYPE_BACK);
	}
}

void flow_game::tapheck_2(bool tap[3]){
	for (int i = 0; i < 3; i++){
		tap[i] = false;
	}
	if (ac->OnePushBotton(7) || key->OnePushKey(KEY_INPUT_Q) || key->OnePushKey(KEY_INPUT_LSHIFT)){
		tap[LEFT_SELECT] = true;
		set_hit(LEFT_SELECT);
	}
	if (ac->OnePushBotton(3) || key->OnePushKey(KEY_INPUT_W) || key->OnePushKey(KEY_INPUT_SPACE)){
		tap[CENTER_SELECT] = true;
		set_hit(CENTER_SELECT);
	}
	if (ac->OnePushBotton(8) || key->OnePushKey(KEY_INPUT_E) || key->OnePushKey(KEY_INPUT_RSHIFT)){
		tap[RIGHT_SELECT] = true;
		set_hit(RIGHT_SELECT);
	}

}

void flow_game::set_score(int all_score[5]){
	all_score[SCORE] = score;
	all_score[MAX_COMBO] = combo_max;
	all_score[MAX_PARFECT] = parfect_max;
	all_score[MAX_GOOD] = good_max;
	all_score[MAX_BAD] = bad_max;
}

int flow_game :: action(){
	for (int i = 0; i < NOTES_MAX; i++){
		if (note[i]->action(spd * SPD_BAI)){
			bad_max++;
			//PlaySoundMem(push_miss,DX_PLAYTYPE_BACK);
			combo = 0;
			combo_cheack = true;
		}
	}
	if (testplay){
		/*if (notecount != dt->t_Getsize()){
			if (dt->t_Gettime(notecount)  < clock() - starttime + (LINE_POS_UP / (spd * SPD_BAI) * 1000)){
				for (int i = 0; i < NOTES_MAX; i++){
					if (note[i]->getflg())continue;
					note[i]->setflg(true);
					note[i]->setID(dt->t_Getid(notecount));
					note[i]->setPosX();
					note[i]->setType(dt->t_Gettype(select_num));
					note[i]->setLength((int)(dt->t_Getlength(notecount) * (spd * SPD_BAI) / 1000));
					notecount++;
					break;
				}
			}
		}*/
	}
	else{
		//ノーツを生成してる
		if (notecount != dt->Gettimesize(select_num)){
			if (dt->Gettime(select_num, notecount)  < clock() - starttime + ((LINE_POS_UP + MARGIN) / (spd * SPD_BAI) * 1000)){
				for (int i = 0; i < NOTES_MAX; i++){
					if (note[i]->getflg())continue;
					note[i]->setflg(true);
					note[i]->setID(dt->Getid(select_num, notecount));
					note[i]->setPosX();
					note[i]->setType(dt->Gettype(select_num, notecount));
					note[i]->setLength((int)(dt->Getlength(select_num, notecount) * (spd * SPD_BAI) / 1000));
					note[i]->set_time(dt->Gettime(select_num, notecount));
					notecount++;
					if (notecount != dt->Gettimesize(select_num)){
						if (dt->Gettime(select_num, notecount) == dt->Gettime(select_num, notecount - 1))continue;
					}
					break;
				}
			}
		}

	}

	bool tap[3];

	tapheck_2(tap);
	//あたり判定
	for (int i = 0; i < 3; i++){
		if (tap[i])erase_effct(i);
	}

	//リザルドに移行
	if ((clock() - starttime > GetSoundTotalTime(dt->Getmusic(select_num)) || key->OnePushKey(KEY_INPUT_R))&&!finish_flg){
		finish_flg = true;
		stop_time = clock();
	}
	if (fmove->getExtRate() > MAX_CHANGE&&!fmove->getchangeflg()){
		fmove->setchangeflg(true);
		combo_save = 0;
		return FLOW_RESULT;
	}

	//カロリー表示の枠を上から下の降ろす
	if (finish_flg&&caloryY<300)caloryY += 10;

	if (caloryY >= 300 && !fmove->getExflg() && stop_time != -1 && clock() - stop_time > 2000){
		fmove->setExflg(true);
		dt->s_sean_change();
	}
	//１５コンボごとにイメージを入れる
	if (!combo_cheack && combo % COMBO_EFFECT < combo_save % COMBO_EFFECT){
		combo_cheack = true;
		combo_image = 255;
	}
	combo_save = combo;

	if (combo % COMBO_EFFECT != 0)combo_cheack = false;

	return FLOW_GAME;
	
}

void flow_game::change_spd(){
	if (key->OnePushKey(KEY_INPUT_M))spd += 0.5f;
	if (key->OnePushKey(KEY_INPUT_N))spd -= 0.5f;
}

void flow_game::DrawNumber(int x, int lx/*ずらす分の大きさ*/, int y, float lnum, int id){
	int c = 0;
	int num = (int)lnum;
	if (id == 0){
		if (num == 0)DrawGraph(x - lx * c, y, dt->g_Getnumbers(0), true);
		while (num >= 1)
		{
			DrawGraph(x - lx * c, y, dt->g_Getnumbers(num % 10), true);
			num = num / 10;
			c++;
		}
	}
	if (id == 1){
		if (num == 0)DrawGraph(x - lx * c, y, dt->g_Getscore_numbers(0), true);
		while (num >= 1)
		{
			DrawGraph(x - lx * c, y, dt->g_Getscore_numbers(num % 10), true);
			num = num / 10;
			c++;
		}
	}
	if (id == 2){
		if (num == 0)DrawGraph(x - lx * c, y, dt->g_Getcombo_numbers(0), true);
		while (num >= 1)
		{
			DrawGraph(x - lx * c, y, dt->g_Getcombo_numbers(num % 10), true);
			num = num / 10;
			c++;
		}
	}
	if (id == 3){
		if (num == 0)DrawGraph(x - lx * c, y, dt->g_Getplay_combo_numbers(0), true);
		while (num >= 1)
		{
			DrawGraph(x - lx * c, y, dt->g_Getplay_combo_numbers(num % 10), true);
			num = num / 10;
			c++;
		}
	}

	if (id == 4){
		if (num == 0)DrawGraph(x - lx * c, y, dt->g_Getkcal_numbers(0), true);
		DrawGraph(x + lx, y, dt->g_Getdot(), true);
		DrawGraph(x + lx*2, y, dt->g_Getkcal_numbers((int)((lnum - num) * 10) % 10), true);
		while (num >= 1)
		{
			DrawGraph(x - lx * c, y, dt->g_Getkcal_numbers(num % 10), true);
			num = num / 10;
			c++;
		}
	}
}

void flow_game::DrawGauge(){
	/*公式...　	    倍率　＝　長さ/元長さ
	  矢印の場所	場所　＝　yポジション　＋　元長さ　×　倍率*/
}
 
void flow_game::Draw(){
	DrawGraph(0, 0, dt->g_Getback(), true);

	combo > 0 ? DrawGraph(0, 0, gradation[GRADITION_NOMAL], true) : DrawGraph(0, 0, gradation[GRADITION_BAD], true);

	//タイトルを表示
	DrawGraph(12, 627, dt->Gettitle(select_num), false);

	DrawGraph(0, 0, back_fream, true);

	for (int i = 0; i < NOTES_MAX; i++){
		note[i]->Draw();
	}

	//DrawFormatString(0, 0, GetColor(255, 0, 0), "%f", spd);
	int rank0 = rank;

	rank = combo / 15;
	if (rank > rank0)PlaySoundMem(fiver_SE, DX_PLAYTYPE_BACK);

	int rank_adjustment = 0;

	if (rank > 2)rank_adjustment = rank - 2;

	int rank_image = rank - rank_adjustment;

	//DrawGraph(680, 10, score_frame, true);

	//DrawGraph(700, 100, combo_frame, true);

	DrawNumber(808, 48, 590, (float)combo,3);
	DrawNumber(904, 30, 653, (float)score, 0);


	//ぼこちゃん表示
	DrawGraph(38, 28, boko[rank_image], false);
	DrawGraph(711, 28, boko[rank_image], false);

	if (clock() % 1200<600){
		DrawGraph(38, 28, boko1[rank_image], false);
		DrawGraph(711, 28, boko1[rank_image], false);
	}

	if (bonus_cheack()){
		DrawGraph(800, -20, bonus_graph, true);
		DrawGraph(700, 60, fiver_effect, true);
	}

	if (combo_image>0){
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, combo_image);
		DrawRotaGraph(500, 300, 1.0, 0.0, hubuki[combo_image / 15 % 2], true);
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
		combo_image -= 3;
		if (combo_image <= 0)combo_image = 0;
	}

	if (finish_flg){
		DrawGraph(272, caloryY-91, kcal_fream, true);
		DrawNumber(383, 44, caloryY, (float)(CALORY*panchcount), 4);
	}

	DrawGraph(0,0,pepar_ring,true);


	/*for (int i = 0; i != time.size(); i++){
		//DrawFormatString(100, i * 20+30, GetColor(255, 0, 0), "time %d", time[i]);
	}
	for (int i = 0; i != id.size(); i++){
		//DrawFormatString(0, i * 20 + 30, GetColor(255, 0, 0), "id %d", id[i]);
	}*/
	//DrawFormatString(0, 30, GetColor(255, 0, 0), "%d", dt->Getmusic(select_num));
}
