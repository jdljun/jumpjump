#ifndef FPS_COUNTER_H
#define FPS_COUNTER_H

class FPS_COUNTER
{
protected:
	float fps;
	float lastTime;
	int frames;
	float time;
public:
	FPS_COUNTER()
	{
		fps = 0.0f;
		lastTime = 0.0f;
		frames = 0;
		time = 0.0f;
	}
	~FPS_COUNTER(){}
	void Update(void);
	float GetFps(void)
	{
		return fps;
	}
};
#endif