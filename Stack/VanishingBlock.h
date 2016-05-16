#pragma once
#include "ModelClass.h"
class VanishingBlock :
	public ModelClass
{
public:
	VanishingBlock();
	~VanishingBlock();
	void SetElapsedTime(float time) { m_lifeTime = time; }
	virtual void Play(float dt) override;
	void UpTransparency() { if (m_transparency < 5) m_transparency++; };

	float GetElapsedTime() { return m_lifeTime; };

private:
	static const float MAX_LIFETIME;
	float		m_lifeTime;
};

