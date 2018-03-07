#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <mmsystem.h>
#include "FPS_COUNTER.h"

void FPS_COUNTER::Update(void)
{
	time=timeGetTime()*0.001f;
	frames++;
	if(time-lastTime>1.0f)
	{
		fps=frames/(time-lastTime);
		lastTime=time;
		frames=0;
	}
}