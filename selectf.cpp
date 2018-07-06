#include "selectf.h"


selectf::selectf(KEY *gkey, date *alldate, flow_game *gm, flow_move *fmv)
{
	count = 0; 
	spd_all = 0;
	arrow_move = 0;
	volume = 255;
	center_num = 2;
	start_time = -10000;
	format();
	move_flg = NONE_MOVE;
	for (int i = 0; i < 5; i++){
		x_waku[i] = WINDOW_MAX_X / 2 + (WAKU_SIZE + 50) * (i-2);
		waku_bai[i] = 0.8;
	}
	key = gkey;
	dt = alldate;
	fgm = gm;
	fmove = fmv;

	back = LoadGraph("image/select_bk.png");
	rignt_arrow = LoadGraph("image/arrows/right_arrow.png");
	left_arrow  = LoadGraph("image/arrows/left_arrow.png");

	boko_L = LoadGraph("image/selectboko/boko_L.png");
	boko_R = LoadGraph("image/selectboko/boko_R.png");

	mask_black = LoadGraph("image/mask.png");

	BeginCal = LoadSoundMem("music/SE/voice/BeginCal.wav");

	left_boko_posY = FIRST_POSY;
	right_boko_posY = FIRST_POSY;

	jump_spd_L = FIRST_JUMP;
	jump_spd_R = FIRST_JUMP;
	load();
}

selectf ::~selectf()
{
}

void selectf::format(){
	left_jump_flg = false;
	right_jump_flg = false;
	first_flg = false;
}

void selectf::load(){
	//WIN32_FIND_DATA lp;
	//HANDLE h = FindFirstFile("music_note/*", &lp);
	//int i = 0;
	//do{
	//	if (i < 2){
	//		i++;
	//	}
	//	else{
	//		string directory;
	//		notedates note;
	//		if (lp.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY){
	//			directory = lp.cFileName;
	//			directory = "music_note/" + directory + "/note.txt";
	//			//ノーツをロードさせる
	//			ifstream ifs(directory);
	//			string str;
	//			while (getline(ifs, str, '/'))
	//			{
	//				istringstream stream(str);
	//				string stri;
	//				int count = 0;
	//				while (getline(stream, stri, ','))
	//				{
	//					int a = atoi(stri.c_str());
	//					if (count == 0)note.time.push_back(a);
	//					else if (count == 1){
	//						note.id.push_back(a);
	//						note.type.push_back(TYPE_NOMAL);
	//					}
	//					else note.length.push_back(a);
	//					count++;
	//				}
	//			}
	//			//曲を入れる
	//			directory = lp.cFileName;
	//			directory = "music_note/" + directory + "/music.mp3";
	//			note.music = LoadSoundMem(directory.c_str());

	//			//タイトルを入れる
	//			directory = lp.cFileName;
	//			directory = "music_note/" + directory + "/title.png";
	//			note.title = LoadGraph(directory.c_str());
	//		}
	//		notedate.push_back(note);
	//	}

	//} while (FindNextFile(h, &lp));
}

void selectf::count_set(){
	stop_sound();
	volume = 255;
	start_time = clock();
	count = count % dt->Getsize();
	if (count < 0)count = dt->Getsize() + count;
	set_music_inf();
	ChangeVolumeSoundMem(255, dt->Getmusic(count));
	if (CheckSoundMem(dt->s_select_start()))return;
	PlaySoundMem(dt->Getmusic(count), DX_PLAYTYPE_BACK,FALSE);
}

void selectf::stop_sound(){
	for (int i = 0; i < (int)dt->Getsize(); i++){
		StopSoundMem(dt->Getmusic(i));
	}
}

void selectf::move_frame(){
	//左右どちらのキーを押したのか検出
	move_now = fgm->tapcheck();

	//選択曲を移動させるふらフラグを立たす
	if (move_flg == NONE_MOVE){
		if (move_now == LEFT_SELECT){
			if (!left_jump_flg){
				left_jump_flg = true;
				jump_spd_L = JUMP_FIRST_MOSTION;
			}
			center_num--;
			center_num = center_num % 5;
			if (center_num < 0) center_num = 5 + center_num;
			move_flg = LEFT_MOVE;
			count--;
			count_set();
		}
		else if (move_now == RIGHT_SELECT){
			if (!right_jump_flg){
				right_jump_flg = true;
				jump_spd_R = JUMP_FIRST_MOSTION;
			}
			center_num++;
			center_num = center_num % 5;
			if (center_num < 0) center_num = 5 + center_num;
			move_flg = RIGHT_MOVE;
			count++;
			count_set();
		}
	}
	//曲枠と曲枠の間隔
	int interval = WAKU_SIZE + 50;

	//選択曲を移動
	//ひだりいいいいいいいいい
	if (move_flg == LEFT_MOVE){
		for (int i = 0; i < 5; i++){
			x_waku[i] += MOVE_SELECT_SPD;
			if (x_waku[i] == WINDOW_MAX_X / 2 + (WAKU_SIZE + 50) * (5 - 2)) x_waku[i] = WINDOW_MAX_X / 2 + (WAKU_SIZE + 50) * - 2;
		}
		spd_all += MOVE_SELECT_SPD;
		if (spd_all >= interval){
			move_flg = NONE_MOVE;
			for (int i = 0; i < 5; i++){
				x_waku[i] -= spd_all - interval;
			}
			spd_all = 0;
		}
	}
	//みぎいいいいいいいいい
	else if (move_flg == RIGHT_MOVE){
		for (int i = 0; i < 5; i++){
			x_waku[i] -= MOVE_SELECT_SPD;
			if (x_waku[i] == WINDOW_MAX_X / 2 + (WAKU_SIZE + 50) * -3) x_waku[i] = WINDOW_MAX_X / 2 + (WAKU_SIZE + 50) * 2;
		}
		spd_all += MOVE_SELECT_SPD;
		if (spd_all >= interval){
			move_flg = NONE_MOVE;
			for (int i = 0; i < 5; i++){
				x_waku[i] += spd_all - interval;
			}
			spd_all = 0;
		}
	}
	//大きさの倍率をセット
	for (int i = 0; i < 5; i++){
		int unko = abs(x_waku[i] - WINDOW_MAX_X / 2);
		if (unko>200)unko = 200;
		waku_bai[i] = (1000 - unko)*0.001;
	}
}

//左右のボコちゃんのジャンプ
void selectf::jump_boko(){
	if (left_jump_flg){
		jump_spd_L -= JUMP_SPD;
		left_boko_posY -= jump_spd_L;
		if (left_boko_posY > FIRST_POSY){
			jump_spd_L = 0;
			left_boko_posY = FIRST_POSY;
			left_jump_flg = false;
		}
	}

	if (right_jump_flg){
		jump_spd_R -= JUMP_SPD;
		right_boko_posY -= jump_spd_R;
		if (right_boko_posY > FIRST_POSY){
			jump_spd_R = 0;
			right_boko_posY = FIRST_POSY;
			right_jump_flg = false;
		}
	}
}

int selectf::action(){
	//枠を移動
	move_frame();

	//ノーツが下りてくるスピード変更(使わないかも)
	fgm->change_spd();

	jump_boko();

	if (CheckSoundMem(dt->s_select_start()) == 0 && !first_flg){
		count_set();
		first_flg = true;
	}

	//シーン変更前の演出
	if (move_flg == NONE_MOVE && move_now == CENTER_SELECT&&!fmove->getExflg()){
		fmove->setExflg(true);
		move_flg = CENTER_MOVE;
		//dt->s_sean_change();
		StopSoundMem(dt->s_select_start());
		PlaySoundMem(BeginCal,DX_PLAYTYPE_BACK);
	}

	//シーン変更を実施
	if (fmove->getExtRate() > MAX_CHANGE&&!fmove->getchangeflg()){
		stop_sound();
		ChangeVolumeSoundMem(255, dt->Getmusic(count));
		PlaySoundMem(dt->Getmusic(count), DX_PLAYTYPE_BACK);
		fgm->set_selectnum(count);
		fgm->in_starttime();
		fmove->setchangeflg(true);
		count = 0;
		move_flg = NONE_MOVE;
		format();
		return FLOW_GAME;
	}

	//曲が終了の時か調べる
	int unko = clock() - start_time;
	if (unko > dt->Getwidth_time(count) * 1000){
		count_set();
	}
	//時が近ければボリュームを下げる
	else if (unko > (dt->Getwidth_time(count) - 2) * 1000){
		volume -= 3;
		ChangeVolumeSoundMem(volume, dt->Getmusic(count));
	}

	//裏動作。今はほぼシカトする
	if (key->OnePushKey(KEY_INPUT_SPACE)){
		stop_sound();
		PlaySoundMem(dt->s_music_now(), DX_PLAYTYPE_BACK);
		fgm->in_starttime();
		fgm->set_test_play(true);
		return FLOW_GAME;
	}

	return FLOW_SELECT;
}

//曲を流す前に各情報を入れる
void selectf::set_music_inf(){
	SetCurrentPositionSoundMem(44100 * dt->Getstart_time(count), dt->Getmusic(count)); //音楽の再生タイミングを決める
	start_time = clock();
}

void selectf::draw_title(){
	//枠を5つ出力
	int num[5];
	num[0] = center_num - 2;
	num[1] = center_num - 1;
	num[2] = center_num;
	num[3] = (center_num + 1)%5;
	num[4] = (center_num + 2)%5;

	if (num[0] < 0)num[0] = 5 + num[0];
	if (num[1] < 0)num[1] = 5 + num[1];
	
	int size = dt->Getsize();
	int num_count[5];
	num_count[0] = (count - 2) % size;
	num_count[1] = (count - 1) % size;
	num_count[2] = count% size;
	num_count[3] = (count + 1) % size;
	num_count[4] = (count + 2) % size;

	for (int i = 0; i < 5; i++){
		if (num_count[i] < 0)num_count[i] = size + num_count[i];
		//DrawRotaGraph(x_waku[num[i]] - WAKU_TITLE_AJM_X, WAKU_Y - WAKU_TITLE_AJM_Y, waku_bai[num[i]], 0.0, dt->Gettitle(i), false);
		//DrawRotaGraph(x_waku[num[i]] - WAKU_TITLE_AJM_X, WAKU_Y - WAKU_TITLE_AJM_Y, waku_bai[num[i]], 0.0, dt->Getillustration(i), false);
	}
	for (int i = 0; i < 5; i++){
		//DrawFormatString(i * 30, 0, GetColor(255, 0, 0), "%d", num[i]);
		//DrawFormatString(0, i * 30 + 30, GetColor(255, 0, 0), "%d", x_waku[i]);
	}
	//DrawFormatString(100, 100, GetColor(255, 0, 0), "count %d", count);
	//DrawFormatString(100, 130, GetColor(255, 0, 0), "center_num %d", center_num);
}

void selectf::draw(){
	DrawGraph(0, 0, back, false);

	//曲の描写
	for (int i = 0; i < 5; i++){
		int tmp;
		int num;
		if (i < 3){
			tmp = (center_num + i) % 5;
			num = (count + i) % dt->Getsize();
			//DrawFormatString(i * 40 + 200, 60, GetColor(255, 0, 0), "%d", num);
		}
		else {
			int unko = 5-i;
			tmp = (center_num - unko);
			if (tmp < 0)tmp = 5 + tmp;

			num = count - unko;
			if (num < 0)num = dt->Getsize() + num;

			//DrawFormatString(-unko * 40 + 200, 60, GetColor(255, 0, 0), "%d", num);
		}
		DrawRotaGraph(x_waku[tmp], WAKU_Y, waku_bai[tmp], 0.0, dt->Getillustration(num), true);

		//マスクをかける
		int alpha_blend = (int)(255 - waku_bai[tmp] * 255 + (1 - waku_bai[tmp]) * 200);
		if (alpha_blend <= 0)continue;
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha_blend);
		DrawRotaGraph(x_waku[tmp], WAKU_Y, waku_bai[tmp], 0.0, mask_black, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

	//左右のぼこちゃん
	DrawRotaGraph(BOKO_LEFT_POSX, (int)left_boko_posY, 1.0, 0, boko_L, true);
	DrawRotaGraph(BOKO_RIGHT_POSX, (int)right_boko_posY, 1.0, 0, boko_R, true);
	
	draw_title();
}
