#pragma once
#include "sprite.h"
#include <vector>

class Animation : public Trackable
{
public:
	Animation();
	Animation(const Animation &other);
	Animation(std::vector<Sprite*> &frames);

	~Animation();

	void cleanup();

	void changeSpeed(double deltaSpeed);

	//setters
	void setColors(const Color &color);
	void setSpeed(int newMs);

	void update(double timeElapsed);

	Sprite *getCurrentSprite();
protected:
	bool mLoop;
	int mMsPerFrame;
	long mTimeOfLastFrame;
	unsigned int mCurrentFrame;

	std::vector<Sprite*> mFrames;
};