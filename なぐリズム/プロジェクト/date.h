#pragma once
#include "DxLib.h"
#include<string>
#include <iostream>
#include<vector>
#include <stdio.h>
#include <fstream>
#include <stdlib.h>
#include"Struct.h"
#include <string>
#include <sstream>
#include <stdlib.h>

//�����Ԃ𑪂�̂Ɏg��
#include<windows.h>
#include<time.h>

using namespace std;
#pragma warning(disable:4996)

#define PERFECT 50
#define GOOD 150

#define WINDOW_MAX_X 960
#define WINDOW_MAX_Y 720

#define SPD 15 //�m�[�c���~��Ă��鑬�x

#define NOTES_SIZEX 80
#define NOTES_SIZEY 10

#define FLOW_TITLE 0
#define FLOW_GAME  1
#define FLOW_MEMRY_PUSH 2
#define FLOW_MEMRY_ON 3
#define FLOW_SELECT 4
#define FLOW_RESULT 5
#define FLOW_LOAD 6

#define LINE_POS_UP 580

#define MARGIN 180

#define NOTES_MAX 30

#define NOTES_DOWN_TIME 648 //�m�[�c���~��Ă���܂ł̂���镪�̎���

//�ǂ��̃{�^�����������̂�(taocheck�Ŏg�p)
#define LEFT_SELECT 0
#define CENTER_SELECT 1
#define RIGHT_SELECT 2
#define NONE_SELECT 3

//�m�[�c�̃^�C�v����
#define TYPE_NOMAL 0
#define TYPE_DOUBLE 1
#define TYPE_BARRAGE 2

//�X�R�A������ƂɎg��
#define SCORE 0
#define MAX_COMBO 1
#define MAX_PARFECT 2
#define MAX_GOOD 3
#define MAX_BAD 4

//�R�}���h
#define COMMAND_LEVEL "level"
#define COMMAND_TIME "time"
#define COMMAND_MUSIC_START_TIME "start_time"
#define COMMAND_MUSIC_FINISH_TIME "finish_time"

//�~����
#define PI 3.14159265359


class date
{
private:
	int music;
	//////////*******�摜********//////////

	//�m�[�c
	int notes;
	int notes1;
	int note_double;
	int note_barrage;
	int note_barrage_face;
	int note_barrage_tail;
	int note_barrage_boko;

	int pusheffect[5];
	int back;
	int music_score;
	int noum;
	int b_next;
	int b_back;
	int waku;
	int titleback;
	int selectback;
	int numbers[10];
	int comment[5];
	int illustration[5];
	int stamp[5];
	int perfect;
	int nice;
	int select_waku;

	//����
	int score_numbers[10];
	int combo_numbers[10];
	int play_combo_numbers[10];
	int kcal_numbers[10];

	//�����_
	int dot;

	vector<notedates> testl;
	vector<notedates> notedate;

	//���y
	int kanasi;
	int decided;
	int music_now;
	int sean_change;

	//SE
	int select_start;
	int think_voice;
	int push[3];

public:
	date();
	~date();

	void load_inf(char *command, char *code, notedates *note);
	void sort_date(vector<notedates> *notedate_copy);

	//notedate�̒l��Ԃ�
	int Getmusic(int num)			{ return notedate[num].music; }
	int Gettitle(int num)			{ return notedate[num].title; }
	int Getresult_title(int num)	{ return notedate[num].result_title; }
	int Getillustration(int num)	{ return notedate[num].illustration; }
	int Gettime(int num,int num1)	{ return notedate[num].time[num1]; }
	int Getid(int num, int num1)	{ return notedate[num].id[num1]; }
	int Gettype(int num, int num1)	{ return notedate[num].type[num1]; }
	int Getlength(int num, int num1){ return notedate[num].length[num1]; }
	int Getsize()					{ return notedate.size(); }
	int Gettimesize(int num)		{ return notedate[num].time.size(); }
	int Getscore_standard(int num)	{ return notedate[num].score_standard; }
	int Getlevel(int num)			{ return notedate[num].level; }
	int Getstart_time(int num)		{ return notedate[num].music_start_time; }
	int Getwidth_time(int num)		{ return notedate[num].width_time; }

	//test�̒l��Ԃ�
	int t_Getsize(){ return testl[0].time.size(); }
	int t_Gettime(int num)	{ return testl[0].time[num]; }
	int t_Getid(int num)	{ return testl[0].id[num]; }
	int t_Gettype(int num)	{ return testl[0].type[num]; }
	int t_Getlength(int num){ return testl[0].length[num]; }

	//�摜��Ԃ�
	//�m�[�c
	int g_Getnotes()				{ return notes; }
	int g_Getnote_double()			{ return note_double; }
	int g_Getnote_barrage()			{ return note_barrage; }
	int g_Getnote_barrage_face()	{ return note_barrage_face; }
	int g_Getnote_barrage_tail()	{ return note_barrage_tail; }
	int g_Getnote_barrage_boko()	{ return note_barrage_boko; }
	int g_Getnotes1()				{ return notes1; }

	int g_pusheffect(int num)		{ return pusheffect[num]; }
	int g_Getback()					{ return back; }
	int g_Getmusic_score()			{ return music_score; }
	int g_Getb_next()				{ return b_next; }
	int g_Getb_back()				{ return b_back; }
	int g_Getwaku()					{ return waku; }
	int g_Gettitleback()			{ return titleback; }
	int g_Getselectback()			{ return selectback; }
	int g_Getperfect()				{ return perfect; }
	int g_Getnice()					{ return nice; }
	int g_Getselect_waku()			{ return select_waku; }
	int g_Getcomment(int num)		{ return comment[num]; }
	int g_Getillustration(int num)	{ return illustration[num]; }
	int g_Getstamp(int num)			{ return stamp[num]; }
	int g_Getdot()					{ return dot; }
	
	//����
	int g_Getnumbers(int i)		{ return numbers[i]; }
	int g_Getcombo_numbers(int i){ return combo_numbers[i]; }
	int g_Getscore_numbers(int i){ return score_numbers[i]; }
	int g_Getplay_combo_numbers(int i){ return play_combo_numbers[i]; }
	int g_Getkcal_numbers(int i){ return kcal_numbers[i]; }

	//��
	int s_pushSE(int id)			{ return push[id]; }
	int s_noum()			{ return noum; }
	int s_think_voice()		{ return think_voice; }

	//����炷	
	void s_sean_change()	{ PlaySoundMem(sean_change, DX_PLAYTYPE_BACK); }

	//���y
	int s_kanasi()			{ return kanasi; }
	int s_decided()			{ return decided; }

	//SE
	int s_select_start()	{ return select_start; }

	int s_music_now()		{ return music_now; }
};

