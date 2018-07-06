#include "hand_made.h"



hand_made::hand_made(date *alldate, KEY *gkey, arcade_controller *lac)
{
	recordingflg = false;
	count = 0;
	key = gkey;
	dt = alldate;
	ac = lac;
	music_now = dt->s_music_now();
	key->OnePushKey(KEY_INPUT_SPACE);
	int a=0;
}


hand_made::~hand_made()
{
}

void hand_made::tapcheck(bool tap[3]){
	for (int i = 0; i < 3; i++){
		tap[i] = false;
	}
	if (ac->OnePushBotton(7) || key->OnePushKey(KEY_INPUT_Q) || key->OnePushKey(KEY_INPUT_LSHIFT)){
		tap[LEFT_SELECT] = true;
	}
	if (ac->OnePushBotton(3) || key->OnePushKey(KEY_INPUT_W) || key->OnePushKey(KEY_INPUT_SPACE)){
		tap[CENTER_SELECT] = true;
	}
	if (ac->OnePushBotton(8) || key->OnePushKey(KEY_INPUT_E) || key->OnePushKey(KEY_INPUT_RSHIFT)){
		tap[RIGHT_SELECT] = true;
	}
}

bool hand_made::action(){
	//ノーツをテキストに記憶させる
	if (recordingflg && (ac->OnePushBotton(10) || key->OnePushKey(KEY_INPUT_SPACE))){
	/*	char *filename = "noteschip/note.txt";
		FILE *fp;
		fopen_s(&fp, filename, "w");
		for (int i = 0; i != id.size(); i++){
			fprintf(fp, "%d,", time[i]);
			if (i == id.size() - 1)fprintf(fp, "%d,%d/", id[i], 0);
			else {
				fprintf(fp, "%d,%d/", id[i], 0);
			}
			if (i % 10 == 9)fprintf(fp, "\n");
		}*/
		recordingflg = false;
		StopSoundMem(music_now);
		//fclose(fp);
	}

	if (!recordingflg&&(ac->OnePushBotton(10)||key->OnePushKey(KEY_INPUT_SPACE))){
		PlaySoundMem(music_now, DX_PLAYTYPE_BACK);
		start = clock();
		recordingflg = true;
		return true;
	}
	return false;
}
void hand_made::Draw(){
	DrawFormatString(0, 0, GetColor(255, 0, 0), "%d", clock());
	for (int i = 0; i < (int)time.size(); i++){
		DrawFormatString(0, (i / 30) * 10, GetColor(255, 0, 0), "%d", id[i]);
		DrawFormatString(30, i * 30, GetColor(255, 0, 0), "%d", time[i]);
	}
}
