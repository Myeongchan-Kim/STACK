#pragma once
#include "ModelClass.h"
class VanishingBlock :
	public ModelClass
{
public:
	VanishingBlock();
	~VanishingBlock();
	void SetElapsedTime(float time) { m_elapsedTime = time; }

	void UpTransparency() { if (m_transparency < 6) m_transparency++; };

	float GetElapsedTime() { return m_elapsedTime; };
	int GetTransparency() { return m_transparency; };

private:
	float						m_elapsedTime = 0.0f;
	int							m_transparency = 0;
};

