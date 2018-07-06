#pragma once
#include"Flow.h"
#include"notes.h"
#include"flow_move.h"
#include<vector>
#include <stdio.h>
#include <fstream>
#include <iostream>


//�{���B������ς���ƃX�s�[�h�ύX���ɑ������������Ȃ�
#define SPD_BAI 100

#define MAX_BOKO 3

//�R���{�����������̃G�t�F�N�g�̃g���K�[�^�C�~���O
#define COMBO_EFFECT 15

#define CALORY 0.03

//�O���f�[�V�����̃i���o�[
#define GRADITION_BAD 0
#define GRADITION_NOMAL 1



using namespace std;


class flow_game : public Flow
{
private:
	bool fiver_flg;
	bool combo_cheack;
	bool finish_flg;
	bool testplay;

	int caloryY;//��ذ��Y���W
	int stop_time;//�J�����[�\����Ɏg��
	int combo_image;
	int combo;
	int combo_save;//�O�̃t���[���ł̃R���{���L��������
	int combo_max;
	int parfect_max;
	int good_max;
	int bad_max;
	float spd;//�m�[�c���~��Ă���X�s�[�h
	int notecount;
	int starttime;
	int score;
	int fiver_score;
	int select_num;
	int rank;
	int panchcount;
	int kcal_fream;
	notes *note[NOTES_MAX];
	/*vector<int> time;
	vector<int> id;
	vector<int> type;
	vector<int> length;*/
	Time* ti;
	//�摜
	int combo_frame;
	int score_frame;
	int back_fream;
	int bonus_graph;
	int fiver_effect;
	int pepar_ring;

	int gradation[2];
	int hubuki[2];
	int boko[MAX_BOKO];
	int boko1[MAX_BOKO];

	flow_move* fmove;

	//��
	int fiver_SE;
	int push_eir;
	int push_miss;

public:
	flow_game(date *alldate, KEY *gkey, arcade_controller *ac, Time* time, flow_move *fmv);
	~flow_game();
	void format();
	int action();
	void Draw();
	void DrawNumber(int x, int lx, int y, float num, int id);
	void DrawGauge();
	int tapcheck();//1�݂̂����o
	void tapheck_2(bool tap[3]);//�Q�ȏ�̓������������o	
	void erase_effct(int num);
	void in_starttime(){ starttime = clock(); }
	void change_spd();
	bool bonus_cheack();
	void set_score(int all_score[5]);
	void set_test_play(bool flg){ testplay = flg; }
	int get_selectnum(){ return select_num; }
	void set_hit(int id);

	void set_selectnum(int num){ select_num = num; }
};

