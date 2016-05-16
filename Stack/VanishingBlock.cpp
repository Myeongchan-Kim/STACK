#include "VanishingBlock.h"

const float VanishingBlock::MAX_LIFETIME = 0.5f;

VanishingBlock::VanishingBlock() : m_lifeTime(MAX_LIFETIME)
{
	m_transparency = 1;
}


VanishingBlock::~VanishingBlock() 
{
}

void VanishingBlock::Play(float dt)
{
	//이동 처리
	ModelClass::Play(dt);

	//투명도 처리
	float lastTime = GetLifeTime();
	SetLifeTime(lastTime - dt);
	SetAlpha(m_lifeTime / MAX_LIFETIME);
}
