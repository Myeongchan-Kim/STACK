#pragma once
#include "ModelClass.h"
class VanishingBlock :
	public ModelClass
{
public:
	VanishingBlock();
	~VanishingBlock();
	void SetLifeTime(float time) { m_lifeTime = time; }
	virtual void Play(float dt) override;

	float GetLifeTime() { return m_lifeTime; };

private:
	static const float MAX_LIFETIME;
	float		m_lifeTime;
};

