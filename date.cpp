#include "date.h"
#define PUSH_EFF_X 1200
#define PUSH_EFF_Y 500

date::date()
{
	SetTransColor(0, 0, 1);
	notes			  = LoadGraph("image/notes/notes_normal.png");
	note_double		  = LoadGraph("image/notes/notes_double.png");
	note_barrage	  = LoadGraph("image/notes/barrage.png");
	note_barrage_face = LoadGraph("image/notes/barrage_face.png");
	note_barrage_tail = LoadGraph("image/notes/barrage_tail.png");
	note_barrage_boko = LoadGraph("image/notes/barrage_1.png");
	notes1			  = LoadGraph("image/notes/notes_shine.png");
	back			  = LoadGraph("image/game/back.png");
	music_score		  = LoadGraph("image/music_score.png");
	b_next			  = LoadGraph("image/b_next.png");
	b_back			  = LoadGraph("image/b_back.png");
	waku			  = LoadGraph("image/waku.png");
	titleback		  = LoadGraph("image/title/titleback.png");
	selectback		  = LoadGraph("image/selectback.png");
	perfect	 		  = LoadGraph("image/effects/perfect.png");
	nice			  = LoadGraph("image/effects/nice.png");
	select_waku		  = LoadGraph("image/select_waku.png");
	dot				  = LoadGraph("image/kcal_num/kcal_dot.png");
	
	music_now	 = LoadSoundMem("music/music_now/music.mp3");
	select_start = LoadSoundMem("music/SE/voice/select_cal.wav");
	think_voice = LoadSoundMem("music/SE/voice/think_you.wav");

	for (int i = 0; i < 10; i++){
		char buf[256];
		sprintf(buf,"image/font/number_%d.png",i);
		numbers[i] = LoadGraph(buf);
		sprintf(buf, "image/score_number/score_%d.png", i);
		score_numbers[i] = LoadGraph(buf);
		sprintf(buf, "image/combo_number/num_%d.png", i);
		combo_numbers[i] = LoadGraph(buf);
		sprintf(buf, "image/play_combo/playcombo_%d.png", i);
		play_combo_numbers[i] = LoadGraph(buf);
		sprintf(buf, "image/kcal_num/kcal_%d.png", i);
		kcal_numbers[i] = LoadGraph(buf);
	}

	for (int i = 0; i < 5; i++){
		char buf[256];
		sprintf(buf, "image/result/comment/comment_%d.png", i);
		comment[i] = LoadGraph(buf);
		sprintf(buf, "image/result/illustration/boko_%d.png", i);
		illustration[i] = LoadGraph(buf);
		sprintf(buf, "image/result/stamp/stamp_%d.png", i);
		stamp[i] = LoadGraph(buf);
	}


	WIN32_FIND_DATA lp;
	HANDLE h = FindFirstFile("music_note/*", &lp);
	int i = 0;
	do{
		if (i < 2){
			i++;
		}
		else{
			string directory;
			notedates note;
			if (lp.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY){
				directory = lp.cFileName;
				directory = "music_note/" + directory + "/note.txt";
				//ノーツをロードさせる
				ifstream ifs(directory);
				string str_;
				while (getline(ifs, str_))
				{
					string str;
					istringstream stream_(str_);
					while (getline(stream_, str, '/'))
					{
						istringstream stream(str);
						string stri;
						int count = 0;
						while (getline(stream, stri, ','))
						{
							int a = atoi(stri.c_str());
							if (count == 0 && a != 0)note.time.push_back(a);
							else if (count == 1){
								note.id.push_back(a);
								note.type.push_back(TYPE_NOMAL);
							}
							else note.length.push_back(a);
							count++;
						}
					}
				}
				//曲を入れる
				directory = lp.cFileName;
				directory = "music_note/" + directory + "/music.mp3";
				note.music = LoadSoundMem(directory.c_str());
				ChangeVolumeSoundMem(180, note.music);

				//タイトルを入れる
				directory = lp.cFileName;
				directory = "music_note/" + directory + "/title.png";
				note.title = LoadGraph(directory.c_str());

				//リザルト用のタイトルを入れる
				directory = lp.cFileName;
				directory = "music_note/" + directory + "/result_title.png";
				note.result_title = LoadGraph(directory.c_str());

				//イラストを入れる
				directory = lp.cFileName;
				directory = "music_note/" + directory + "/back.png";
				note.illustration = LoadGraph(directory.c_str());

				//ランク判定
				note.score_standard = note.time.size() * 50;

				for (int i = 0; i != note.time.size();i++){
					note.score_standard += 3 * (i + 1);
				}
			}

			//タイプ判別
			for (int i = 1; i < (int)note.type.size(); i++){
				if (note.time[i - 1] == note.time[i]){
					note.type[i - 1] = TYPE_DOUBLE;
					note.type[i] = TYPE_DOUBLE;
				}
				if (note.length[i] != 0)note.type[i] = TYPE_BARRAGE;
			}
			directory = lp.cFileName;
			directory = "music_note/" + directory + "/information.txt";

			ifstream ifs(directory);
			string str_;
			while (getline(ifs, str_))
			{
				char cstr[256];
				char *command, *code;

				strcpy(cstr, str_.c_str());

				if (strstr(cstr, "//") != NULL) continue;

				command = strtok(cstr, "=");
				code = strtok(NULL, "=");

				load_inf(command, code, &note);
			}
			notedate.push_back(note);
		}
	} while (FindNextFile(h, &lp));

	sort_date(&notedate);

	////////////////////////////////////////
	//作成したノーツをロードさせる
	ifstream ifs("noteschip/note.txt");
	string str;
	notedates test;
	while (getline(ifs, str, '/'))
	{
		istringstream stream(str);
		string stri;
		int count = 0;
		while (getline(stream, stri, ','))
		{
			int a = atoi(stri.c_str());
			if (count == 0)test.time.push_back(a);
			else if (count == 1){
				test.id.push_back(a);
				test.type.push_back(TYPE_NOMAL);
			}
			else test.length.push_back(a);
			count++;
		}

		//タイプ判別
		for (int i = 1; i < (int)test.id.size(); i++){
			if (test.time[i - 1] == test.time[i]){
				test.type[i - 1] = TYPE_DOUBLE;
				test.type[i] = TYPE_DOUBLE;
			}
			if (test.length[i] != 0)test.type[i] = TYPE_BARRAGE;
		}
	}
	testl.push_back(test);

	//SE
	push[LEFT_SELECT] = LoadSoundMem("music/SE/push_L.mp3");
	push[CENTER_SELECT] = LoadSoundMem("music/SE/push.mp3");
	push[RIGHT_SELECT] = LoadSoundMem("music/SE/push_R.mp3");
	noum = LoadSoundMem("music/SE/noum.mp3");
	sean_change = LoadSoundMem("music/SE/sean_change.mp3");

	SetTransColor(0,0,0);
	LoadDivGraph("image/push_eff.bmp", 5, 5, 1, PUSH_EFF_X / 5, PUSH_EFF_Y, pusheffect);
}

//ソートするやつ
void date::sort_date(vector<notedates> *notedate_copy){
	vector<notedates> copy = *notedate_copy;
	for (int i = 0; i != copy.size(); i++)
	{
		for (int y = i; y != copy.size(); y++)
		{
			if (copy[i].level > copy[y].level){
				notedates level = copy[y];
				copy[y] = copy[i];
				copy[i] = level;
			}
		}
	}
	*notedate_copy = copy;
}

void date::load_inf(char *command, char *code, notedates *note){
	if (strcmp(command, COMMAND_LEVEL) == 0) {
		note->level = atoi(code);
		return;
	}

	else if (strcmp(command, COMMAND_TIME) == 0) {
		note->time_long = atoi(code);
		return;
	}

	else if (strcmp(command, COMMAND_MUSIC_START_TIME) == 0) {
		note->music_start_time = atoi(code);
		return;
	}

	else if (strcmp(command, COMMAND_MUSIC_FINISH_TIME) == 0) {
		note->width_time = atoi(code) - note->music_start_time;
		return;
	}
}


date::~date()
{
}
