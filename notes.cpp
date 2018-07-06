#include "notes.h"


notes::notes(date* ldt, Time* time)
{
	x = 0;
	y = -MARGIN;
	ani_count = 0;
	size = 1;
	id = -1;//�A�^������Ŏg������-1
	spd = SPD;
	flg = false;
	pushflg = false;
	alpha = 255;
	dt = ldt;
	ti = time;
}


notes::~notes()
{
}

bool notes::action(float spd){
	if (!flg || pushflg)return false;
	bool a = false;
	a = outcheck();
	if (!pushflg)y += ti->getframetime() * spd;
	return a;
}

//�m�[�c���^�C�~���O�悭�����ꂽ��true�ɂ���
void notes::Deathcount(){
	pushflg = true;
}

void notes :: setflg(bool lflg){
	flg = lflg;
}

//�A�j���[�V�������I��������m�F
void notes::reset_cheack(){
	//�m�[�c��������
	if (ani_count / 2 == 5){
		ani_count = 0;
		format();
		setflg(false);
	}
	alpha = 255 * (10 - ani_count) / 10;
	size = 1 + (float)ani_count/5;
}

void notes::setType(int ty){
	type = ty;
}

void notes::setPosX(){
	switch (id)
	{
	case 0:
		x = ID0;
		break;
	case 1:
		x = ID1;
		break;
	case 2:
		x = ID2;
		break;
	default:
		break;
	}
}

void notes::setLength(int leng){
	length = leng;
}

//��ʊO�Ƀm�[�c���o�����`�F�b�N
bool notes::outcheck(){
	if (type == TYPE_BARRAGE){
		if (y - length - 200 > WINDOW_MAX_Y){
			setflg(false);
			ani_frame.clear();
			lx.clear();
			ly.clear();
			alpha_l.clear();
			ani_frame.shrink_to_fit();
			lx.shrink_to_fit();
			ly.shrink_to_fit();
			alpha_l.shrink_to_fit();
			format();
		}
		return false;
	}
	if (y - length > WINDOW_MAX_Y){
		setflg(false);
		format();
		return true;
	}
	return false;
}

void notes::format(){
	x = 0;
	y = -MARGIN;
	ani_count = 0;
	alpha = 255;
	size = 1;
	id = -1;
	spd = SPD;
	flg = false;
	pushflg = false;
}

void notes::set_animation(int lnum){
	ani_frame.push_back(0); 
	alpha_l.push_back(255);
	int randamX;
	int randamY;

	if (rand() % 2 == 0){
		randamX = rand() % 30;
		randamY = rand() % 30;
	}
	else{
		randamX = -rand() % 30;
		randamY = -rand() % 30;
	}

	switch (lnum)
	{
	case LEFT_SELECT:
		lx.push_back(ID0 + randamX);
		break;
	case CENTER_SELECT:
		lx.push_back(ID1 + randamX);
		break;
	case RIGHT_SELECT:
		lx.push_back(ID2 + randamX);
		break;
	default:
		break;
	}

	ly.push_back(LINE_POS_UP + randamY);

}


void notes::Draw(){
	if (!flg)return;
	if (!pushflg){
		int gr;

		//�ʏ�m�[�c
		if (type == TYPE_NOMAL)gr = dt->g_Getnotes();

		//�A�Ńm�[�c
		else if (type == TYPE_BARRAGE){
			gr = dt->g_Getnote_barrage();
			DrawRotaGraph(ID1, (int)(y - length), 1.0, 0.0, dt->g_Getnote_barrage_tail(), true);
			DrawExtendGraph(ID0 - 55, (int)(y - length + 125), ID2 + 55, (int)y-175, gr, false);
			DrawRotaGraph(ID1, (int)y - 175, 1.0, 0.0, dt->g_Getnote_barrage_face(), true);
		}

		//���������m�[�c
		else gr = dt->g_Getnote_double();
		if (type != TYPE_BARRAGE)DrawRotaGraph((int)x, (int)y, 1.0, 0, gr, true);
	}
	else{ 
		//�G�t�F�N�g
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
		if (g_judg == dt->g_Getperfect()) y = LINE_POS_UP;
		DrawRotaGraph((int)x, (int)y, size, 0.0, dt->g_Getnotes1(), true);
		DrawRotaGraph((int)x, (int)y - 70, 2.0, 0.0, g_judg, true);
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
		ani_count++;
		reset_cheack();
	}

	//�A�ŗp�̃G�t�F�N�g
	for (int i = 0; i != ani_frame.size(); i++){
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha_l[i]);
		DrawRotaGraph(lx[i], ly[i], 1.0, 0.0, dt->g_Getnotes1(), true);
		alpha_l[i] -= 10;
		ani_frame[i]++;
	}
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
	
}


