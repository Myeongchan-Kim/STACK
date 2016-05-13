#pragma once

#include <Windows.h>

class CMyTime
{
	bool		m_bUseQPF;
	float		m_fElapsedTime;
	LONGLONG	m_llQPFTicksPerSec;
	LONGLONG	m_llLastElapsedTime;
public:
	CMyTime(void);
	virtual ~CMyTime(void);

	inline float GetElapsedTime() const
	{ return m_fElapsedTime; };

	void Init();
	void ProcessTime();
};
