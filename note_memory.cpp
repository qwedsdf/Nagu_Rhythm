#include "note_memory.h"


note_memory::note_memory(date *alldate, KEY *gkey, arcade_controller *lac, hand_made *hd)
{
	kasoru = LoadGraph("image/kasoru.png");
	recordingflg = false;
	loadflg = false;
	moveflg = false;
	movebackflg = false;
	stopflg = false;
	backY = SAVE_LINE;
	count = 0;
	buf = 0;
	line_y = 0;
	position = 0;
	move_next = 0;
	page_count = 0;
	save_time = 0;
	notes_line = 0;
	first_note = -1;
	second_note = -1;
	first_pos = -1;
	second_pos = -1;

	key = gkey;
	dt = alldate; 
	ac = lac;
	hand = hd;

	music_now = dt->s_music_now();
}


note_memory::~note_memory()
{
}

void note_memory::format(){
	//ノーツ順位の初期化
	first_note = -1;
	second_note = -1;
	for (int i = 0; i != moveflg_note.size(); i++){
		moveflg_note[i] = false;
	}
}

void note_memory::load(){
	//元からあるノーツデータのロードを開始させるフラグを立てる(時間でロード)
	if (key->OnePushKey(KEY_INPUT_RSHIFT)){
		move_next = 0;
		page_count = 0;
		//ノーツデータをベクターにいれる
		ifstream ifs("noteschip/note.txt");
		string str;
		while (getline(ifs, str, '/'))
		{
			istringstream stream(str);
			string stri;
			int countl = 0;
			while (getline(stream, stri, ','))
			{
				int a = atoi(stri.c_str());
				if (countl == 0){ time.push_back(a); y.push_back(775 - (250*a / 1000)); }
				else if (countl == 1){ id.push_back(a); moveflg_note.push_back(false); }
				else length.push_back(a);
				countl++;
			}
		}
		x.clear();
		//ノーツデータをロードする
		for (int i = 0; i != id.size(); i++){
			switch (id[i])
			{
			case 0:
				x.push_back(LEFT);
				break;
			case 1:
				x.push_back(LEFT_CENTER);
				break;
			case 2:
				x.push_back(RIGHT_CENTER);
				break;
			case 3:
				x.push_back(RIGHT);
				break;
			default:
				break;
			}
		}		
	}
	///////////////座標でロード//////////
	/*if (key->OnePushKey(KEY_INPUT_LSHIFT)){
		ifstream ifs("noteschip/chip1_pos.txt");
		string str;
		while (getline(ifs, str, '/'))
		{
			istringstream stream(str);
			string stri;
			int countl = 0;
			while (getline(stream, stri, ','))
			{
				int a = atoi(stri.c_str());
				if (countl == 0)x.push_back(a);
				else if (countl == 1)y.push_back(a);
				else { id.push_back(a); moveflg_note.push_back(false); }
				countl++;
			}
		}
	}*/
}

//クイックソート(改造済み)
void note_memory::QSort(int left, int right)
{
	int pivot, l_hold, r_hold, l_id, l_length;

	l_hold = left;
	r_hold = right;
	pivot = ltime[left];
	l_id = lid[left];
	l_length = llength[left];
	while (left < right)
	{
		while ((ltime[right] >= pivot) && (left < right))
			right--;
		if (left != right)
		{
			ltime[left] = ltime[right];
			lid[left] = lid[right];
			llength[left] = llength[right];
			left++;
		}
		while ((ltime[left] <= pivot) && (left < right))
			left++;
		if (left != right)
		{
			ltime[right] = ltime[left];
			lid[right] = lid[left];
			llength[right] = llength[left];
			right--;
		}
	}
	ltime[left] = pivot;
	lid[left] = l_id;
	llength[left] = l_length;
	pivot = left;
	left = l_hold;
	right = r_hold;
	if (left < pivot)
		QSort(left, pivot - 1);
	if (right > pivot)
		QSort(pivot + 1, right);
}


//////セーブシステムを改良する。※時間で図らず、1ピクセルにつき4（1000で1s）で計算する
void note_memory::save(){
	/*　＊問題点...0.004s誤差が出ることがある　*/
	//時間に変更してセーブ
	if (key->OnePushKey(KEY_INPUT_G)){
		format();
		if (y.size() == 0){
			move_next = 0;
			page_count = 0;
		}
		else{
			//同じ場所にノーツがないか確認
			for (int i = 0; i != y.size(); i++)
			{
				if (y[i] == 1)continue;
				for (int k = i + 1; k != y.size(); k++)
				{
					if (y[k] == 1)continue;
					if (y[i] == y[k] && x[i] == x[k]){
						x[k] = 1;
						length[k] = 1;
					}
				}
			}
			
			for (int i = 0; i != y.size(); i++){
				y[i] -= move_next;
				if (x[i] == 1)continue;
				ltime.push_back((SAVE_LINE - y[i]) * 1000 / 250);
			}
			move_next = 0;
			page_count = 0;
			for (unsigned int i = 0; i < x.size(); i++){
				int magnification = x[i] / BACK_X;
				switch (x[i])
				{
				case LEFT:
					lid.push_back(0);
					break;
				case LEFT_CENTER:
					lid.push_back(1);
					break;
				case RIGHT_CENTER:
					lid.push_back(2);
					break;
					/*case RIGHT:
					lid.push_back(3);
					break;*/
				default:
					break;
				}
				ltime.push_back(3000 * magnification + (SAVE_LINE - y[i]) * 4);
			}

			for (int i = 0; i < (int)length.size(); i++){
				if (length[i] == 1)continue;
				llength.push_back(length[i]);
			}

			QSort(0, lid.size() - 1);
			char *filename = "noteschip/note.txt";
			FILE *fp;
			fopen_s(&fp, filename, "w");
			for (int i = 0; i != lid.size(); i++){
				fprintf(fp, "%d,", ltime[i]);
				fprintf(fp, "%d,", lid[i]);
				fprintf(fp, "%d/", llength[i]);
				if (i % 10 == 9)fprintf(fp, "\n");
			}
			fclose(fp);
			ltime.clear();
			lid.clear();
			llength.clear();
		}
		
	}

	/////////////////////////////////ポジションでセーブする////////////////////////////////
	//座標をかき出し
	if (key->OnePushKey(KEY_INPUT_A)){
		char *filename = "noteschip/chip1_pos.txt";
		FILE *fp;
		fopen_s(&fp, filename, "w");
		for (int i = 0; i != x.size(); i++){
			fprintf(fp, "%d,", x[i]);
			fprintf(fp, "%d,", y[i]);
			fprintf(fp, "%d/", id[i]);
			if (i % 10 == 9)fprintf(fp, "\n");
		}
		fclose(fp);
	}
	
	//////////////////////////////////時間でセーブするやつ(産廃行きになりそう)/////////////////////////////////////
	/*if (!recordingflg && (ac->OnePushBotton(10) || key->OnePushKey(KEY_INPUT_SPACE) || key->OnePushKey(KEY_INPUT_C))){
		stopflg=false;
		PlaySoundMem(music_now, DX_PLAYTYPE_BACK);
		start = clock();
		if (key->OnePushKey(KEY_INPUT_C)){
			start += NOTES_DOWN_TIME;
		}
		recordingflg = true;
	}

	//ノーツをベクターの中に入れる(セーブ)
	if (recordingflg){
		for (int i = 0; i < (int)y.size(); i++){
			if (x[i] >= LEFT + notes_line && x[i] <= RIGHT + notes_line){
				if (y[i] < backY)continue;
			}
			else continue;

			ltime.push_back(clock() - start);
			lid.push_back(id[i]);
			x.erase(x.begin() + i);
			y.erase(y.begin() + i);
			id.erase(id.begin() + i);
		}
	}

	//ノーツをテキストに記憶させる
	if (recordingflg){
		if ((ac->OnePushBotton(10) || key->OnePushKey(KEY_INPUT_SPACE))){
			char *filename = "noteschip/chip1.txt";
			FILE *fp;
			fopen_s(&fp, filename, "w");
			for (int i = 0; i != lid.size(); i++){
				fprintf(fp, "%d,", ltime[i]);
				fprintf(fp, "%d/ ", lid[i]);
				if (i % 10 == 9)fprintf(fp, "\n");
			}
			StopSoundMem(music_now);
			fclose(fp);
		}
		if (key->OnePushKey(KEY_INPUT_X)){
			char *filename = "noteschip/chip1_s.txt";
			FILE *fp;
			fopen_s(&fp, filename, "w");
			for (int i = 0; i != lid.size(); i++){
				fprintf(fp, "%d,", ltime[i]);
				fprintf(fp, "%d/ ", lid[i]);
				if (i % 10 == 9)fprintf(fp, "\n");
			}
			StopSoundMem(music_now);
			fclose(fp);
		}
	}*/
}

void note_memory::note_on(){
	if (!CheckHitKey(KEY_INPUT_LCONTROL)){ first_pos = -1; second_pos = -1; }
	/////////////////////////ノーツを置く関係//////////////////////////////////
	//マウスを左クリックしたらノーツを表示
	if (key->GetMouseOnePush(MOUSE_INPUT_LEFT)){
		if (CheckHitKey(KEY_INPUT_LCONTROL)){
			if (first_pos == -1)first_pos = key->getMousePointY();
			else if (second_pos == -1){
				second_pos = key->getMousePointY();
				float length_time = (float)(abs(second_pos - first_pos));
				length_time = length_time / 250 * 1000;
				if (abs(LEFT - key->getMousePointX()) < 35) { x.push_back(LEFT); y.push_back(first_pos); id.push_back(0); moveflg_note.push_back(false); length.push_back((int)length_time); }
			}
		}
		else{
			if (abs(LEFT - key->getMousePointX()) < 35) { x.push_back(LEFT); y.push_back(key->getMousePointY()); id.push_back(0); moveflg_note.push_back(false); length.push_back(0); }

			if (abs(LEFT_CENTER - key->getMousePointX()) < 35) { x.push_back(LEFT_CENTER); y.push_back(key->getMousePointY()); id.push_back(1); moveflg_note.push_back(false);  length.push_back(0); }

			if (abs(RIGHT_CENTER - key->getMousePointX()) < 35) { x.push_back(RIGHT_CENTER); y.push_back(key->getMousePointY()); id.push_back(2); moveflg_note.push_back(false); length.push_back(0); }
		}
		
	}

	//右クリックで選択
	if (key->GetMouseOnePush(MOUSE_INPUT_RIGHT)){
		for (int i = 0; i != moveflg_note.size(); i++){
			unsigned int hitX = abs(key->getMousePointX() - x[i]);
			unsigned int hitY = abs(key->getMousePointY() - y[i]);
			if (hitX < NOTES_SIZEX / 2 && hitY < NOTES_SIZEY / 2){
				if (!moveflg_note[i]){
					moveflg_note[i] = true;
					if(first_note==-1)first_note = i;
					else if (second_note == -1)second_note = i;
				}
				else {
					moveflg_note[i] = false;
					if (first_note == i || second_note == i){
						first_note = -1;
						second_note = -1;
					}
				}
			}
		}
	}

	if (key->OnePushKey(KEY_INPUT_F))format();

	//全削除
	if (key->OnePushKey(KEY_INPUT_L)){
		x.clear();
		y.clear();
		time.clear();
		length.clear();
		id.clear();
		moveflg_note.clear();

		x.shrink_to_fit();
		y.shrink_to_fit();
		time.shrink_to_fit();
		length.shrink_to_fit();
		id.shrink_to_fit();
		moveflg_note.shrink_to_fit();
	}

	//ノーツが選択されているときのみ可能なノーツ処理
	for (int i = 0; i != moveflg_note.size(); i++){
		if (moveflg_note[i]){
			if (CheckHitKey(KEY_INPUT_UP))y[i] -= NOTE_MOVE_SPD;
			if (CheckHitKey(KEY_INPUT_DOWN))y[i] += NOTE_MOVE_SPD;
			if (key->OnePushKey(KEY_INPUT_D)){
				x.erase(x.begin() + i);
				y.erase(y.begin() + i);
				id.erase(id.begin() + i);
				moveflg_note.erase(moveflg_note.begin() + i);
				length.erase(length.begin() + i);
				break;
			}
			if (key->OnePushKey(KEY_INPUT_S) && first_note != -1 && second_note != -1){
				y[second_note] = y[first_note];
				moveflg_note[first_note] = false;
				moveflg_note[second_note] = false;
				format();
			}
		}
	}


	//ボタンを押したら次のページへ
	if (key->GetMouseOnePush(MOUSE_INPUT_LEFT)){
		if (abs(BOTAN_X - key->getMousePointX()) < 25 && abs(BOTAN_Y - key->getMousePointY()) < 24){
			moveflg = true;
			page_count++;
		}
	}

	//ボタンを押したら前のページへ
	if (key->GetMouseOnePush(MOUSE_INPUT_LEFT)){
		if (abs(BOTAN_BACK_X - key->getMousePointX()) < 25 && abs(BOTAN_BACK_Y - key->getMousePointY()) < 24){
			movebackflg = true;
			page_count--;
		}
	}

}
void note_memory::move_page(){
	///////////////////////////////ページ移動/////////////////////////////
	//次のページへ
	if (moveflg){
		move_next += 60;
		if (move_next >= PAGE_LONG * page_count){
			move_next = PAGE_LONG * page_count;
			for (unsigned int i = 0; i < x.size(); i++){ x[i] -= PAGE_LONG; }
			moveflg = false;
		}
	}
	//前のページへ
	if (movebackflg){
		move_next -= 60;
		if (move_next <= PAGE_LONG * page_count){
			move_next = PAGE_LONG * page_count;
			for (unsigned int i = 0; i < x.size(); i++){ x[i] += PAGE_LONG; }
			movebackflg = false;
		}
	}
}

void note_memory::set_note_flg(){
	moveflg_note.push_back(false);
}

//キー入力
void note_memory::save_hand(){
	if (hand->action()){
		start = (int)hand->get_start();
		save_time = clock() - start;
		backY = 775 - 250 * save_time / 1000 + move_next;
	}
	if (hand->get_recordingflg()){
		bool tap[3];
		hand->tapcheck(tap);
		if (tap[LEFT_SELECT]){
			x.push_back(LEFT);
			y.push_back(backY);
			id.push_back(0);
			set_note_flg();
			length.push_back(0);
		}
		if (tap[CENTER_SELECT]){
			x.push_back(LEFT_CENTER);
			y.push_back(backY);
			id.push_back(1);
			set_note_flg();
			length.push_back(0);
		}
		if (tap[RIGHT_SELECT]){
			x.push_back(RIGHT_CENTER);
			y.push_back(backY);
			id.push_back(2);
			set_note_flg();
			length.push_back(0);
		}
		
		if (backY < 400){
			move_next += 400 - backY;
			for (int i = 0; i != y.size(); i++){
				y[i] += 400 - backY;
			}
		}
	}
}

void note_memory::action(){
	save_time = clock() - start;
	//notes_line = BACK_X * (save_time / 3000);

	load();
	
	save();

	note_on();

	//キー打ち入力
	save_hand();
	
	//move_page();


	//〇分〇秒をクリックしたらそこから音楽スタート
	if (key->GetMouseOnePush(MOUSE_INPUT_LEFT)){
		int posX, posY;
		for (int i = 0; i < 300; i++){
			posX = 35;
			posY = 775 - i * 250 + move_next;
			if (abs(key->getMousePointX() - posX) < 32 && abs(key->getMousePointY() - posY) < 8){
				stopflg = false;
				SetCurrentPositionSoundMem(44100 * i, music_now); //音楽の再生タイミングを決める

				start = clock()-1000*i;
				hand->set_recordingflg(true);
				StopSoundMem(music_now);
				PlaySoundMem(music_now, DX_PLAYTYPE_BACK, FALSE);
			}
		}
	}

	//曲を一時停止
	if (key->OnePushKey(KEY_INPUT_Z)){
		if (!stopflg){
			StopSoundMem(music_now);
			stop_time = save_time;
			stopflg = true;
		}
		else {
			start = clock() - stop_time;
			SetCurrentPositionSoundMem(44100 * stop_time / 1000, music_now); //音楽の再生タイミングを決める
			PlaySoundMem(music_now, DX_PLAYTYPE_BACK, FALSE);
			stopflg = false;
		}
	}
	if (key->OnePushKey(KEY_INPUT_SPACE))StopSoundMem(music_now);

	int unko = GetMouseWheelRotVol() * WHEEL_SPD;
	move_next += unko;
	//ノーツも一緒にすくろーる
	for (int i = 0; i != y.size(); i++){
		y[i] += unko;
	}
}
void note_memory::Draw(){
	//背景とタイムを表示
	for (int i = 0; i < 300; i++){
		int posX, posY;
		posX = 35;
		posY = 525 - (i * 250) + move_next;
		DrawGraph(posX, posY, dt->g_Getmusic_score(), false);
		SetFontSize(16);
		DrawFormatString(posX - 30, posY, GetColor(0, 255, 0), "%d分%d秒", (i + 1) / 60, (i + 1) % 60);
	}

	DrawFormatString(0, 0, GetColor(255, 0, 0), "%d", moveflg_note.size());
	DrawFormatString(0, 30, GetColor(255, 0, 0), "%d", x.size());
	DrawFormatString(0, 60, GetColor(255, 0, 0), "%d", y.size());
	DrawFormatString(0, 90, GetColor(255, 0, 0), "%d", length.size());
	DrawFormatString(0, 120, GetColor(255, 0, 0), "%d", id.size());

	DrawFormatString(300, 0, GetColor(255, 0, 0), "%d", key->getMousePointX());
	
	//ノーツの表示
	for (int i = 0; i !=moveflg_note.size(); i++){
		if (length[i] != 0){
			DrawExtendGraph(x[i], y[i] - length[i] * 250 / 1000, x[i]+180, y[i], dt->g_Getnote_barrage(), false);
		}
		else DrawRotaGraph(x[i], y[i], 1.0, 0, dt->g_Getnotes(), false);
		if (moveflg_note[i])DrawRotaGraph(x[i], y[i], 1.0, 0, dt->g_Getwaku(), true);
	}

	if (recordingflg){
		if (line_y < buf)PlaySoundMem(dt->s_noum(), DX_PLAYTYPE_BACK);
		buf = line_y;
	}

	//UI表示
	DrawRotaGraph(BOTAN_X, BOTAN_Y,1.0,0.0, dt->g_Getb_next(), false);
	DrawRotaGraph(BOTAN_BACK_X, BOTAN_BACK_Y, 1.0, 0.0, dt->g_Getb_back(), false);

	//ラインを表示
	save_time = clock() - start;

	backY = 775 - 250 * save_time / 1000 + move_next;

	if (stopflg){
		backY = 775 - 250 * stop_time / 1000 + move_next;
	}
	DrawLine(LEFT - 35, backY, RIGHT + 35, backY, GetColor(255, 0, 0), false);
	int MouseX, MouseY;
	GetMousePoint(&MouseX, &MouseY);
	DrawGraph(MouseX, MouseY, kasoru, true);

	//数値確認
	DrawFormatString(100, 0, GetColor(255, 0, 0), "%d", first_pos);
	DrawFormatString(130, 0, GetColor(255, 0, 0), "%d", second_pos);
}
