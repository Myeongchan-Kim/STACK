#include "ExactFitEffect.h"

const float ExactFitEffect::MAX_LIFETIME = 0.5f;

ExactFitEffect::ExactFitEffect() : m_lifeTime(MAX_LIFETIME)
{
	m_transparency = 1;
}


ExactFitEffect::~ExactFitEffect()
{
}

void ExactFitEffect::Play(float dt)
{
	//�̵� ó��
	ModelClass::Play(dt);

	//���� ó��
	float lastTime = GetLifeTime();
	if (m_lifeTime > 0)
	{
		SetLifeTime(lastTime - dt);
		SetScale(sqrt((MAX_LIFETIME + m_lifeTime) / MAX_LIFETIME), sqrt((MAX_LIFETIME + m_lifeTime) / MAX_LIFETIME), 1);
		SetAlpha(sqrt(m_lifeTime / MAX_LIFETIME));
	}
}