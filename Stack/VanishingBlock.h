#pragma once
#include "ModelClass.h"
class VanishingBlock :
	public ModelClass
{
public:
	VanishingBlock();
	~VanishingBlock();
	void SetElapsedTime(float time) { m_elapsedTime = time; }
	virtual void Play(float dt) override;
	void UpTransparency() { if (m_transparency < 5) m_transparency++; };

	float GetElapsedTime() { return m_elapsedTime; };

private:
	float						m_elapsedTime = 0.0f;
};

