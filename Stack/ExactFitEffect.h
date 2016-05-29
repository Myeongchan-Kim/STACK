#pragma once
#include "ModelClass.h"
class ExactFitEffect :
	public ModelClass
{
public:
	ExactFitEffect();
	~ExactFitEffect();
	void Play(float dt);
	void SetLifeTime(float time) { m_lifeTime = time; };

private:
	float GetLifeTime() { return m_lifeTime; };
	static const float MAX_LIFETIME;
	float m_lifeTime;

};

