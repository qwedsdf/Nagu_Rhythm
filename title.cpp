#include "title.h"


title::title(KEY *gkey, date *alldate, flow_game *gm, selectf *se, flow_move *fmv)
{
	key = gkey;
	dt = alldate;
	fgm = gm;
	sel = se;
	fmove = fmv;
	once = false;
	g_push_start = LoadGraph("image/title/pushstart.png");
	pushSE = LoadSoundMem("music/SE/voice/titlecal.wav");
}


title::~title()
{
}

int title::action(){
	if (fgm->tapcheck() == CENTER_SELECT&&!once){
		dt->s_sean_change();
		StopSoundMem(dt->s_think_voice());
		PlaySoundMem(pushSE, DX_PLAYTYPE_BACK);
		pushtime = clock();
		once = true;
	}
	if (fmove->getExtRate() > MAX_CHANGE&&!fmove->getchangeflg()){
		fmove->setchangeflg(true);
		once = false;
		PlaySoundMem(dt->s_select_start(),DX_PLAYTYPE_BACK);
		return FLOW_SELECT;
	}
	return FLOW_TITLE;
}

void title::draw(){
	int spd = 500;
	if (once)spd = 100;

	DrawGraph(0, 0, dt->g_Gettitleback(), false);
	if (clock() % (spd*2) < spd)DrawGraph(330, 427, g_push_start, false);
	if (clock() - pushtime > 1000 && once)fmove->setExflg(true);
}