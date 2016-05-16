#include "VanishingBlock.h"



VanishingBlock::VanishingBlock()
{
}


VanishingBlock::~VanishingBlock()
{
}

void VanishingBlock::Play(float dt)
{
	ModelClass::Play(dt);

	//사라지는 오브젝트 처리.
	float lastTime = GetElapsedTime();
	SetElapsedTime(lastTime + dt);

	if (GetElapsedTime() > 0.1f)
	{
		UpTransparency();
		lastTime = dt;
		SetElapsedTime(0);
	}
}
