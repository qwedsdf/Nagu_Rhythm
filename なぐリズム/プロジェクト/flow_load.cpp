#include "flow_load.h"


flow_load::flow_load()
{
	load_image = LoadGraph("image/move_scean/Kobusi.png");
}


flow_load::~flow_load()
{
}

void flow_load::draw(){
	DrawRotaGraph(WINDOW_MAX_X / 2, WINDOW_MAX_Y/2,0.5f,0.0f,load_image,false);
}
