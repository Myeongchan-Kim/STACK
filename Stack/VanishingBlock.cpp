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
	//�̵� ó��
	ModelClass::Play(dt);

	//���� ó��
	float lastTime = GetElapsedTime();
	SetElapsedTime(lastTime - dt);
	SetAlpha(m_lifeTime / MAX_LIFETIME);
}
