#include "MyTime.h"

CMyTime::CMyTime(void) : m_bUseQPF(false)
						, m_fElapsedTime(0.f)
						, m_llQPFTicksPerSec(0)
						, m_llLastElapsedTime(0)
{
}

CMyTime::~CMyTime(void)
{
}

void CMyTime::Init()
{
	LARGE_INTEGER qwTicksPerSec, qwTime;

	m_bUseQPF = (bool)(QueryPerformanceFrequency(
		&qwTicksPerSec ) != 0 );

	if( !m_bUseQPF )	return;

	m_llQPFTicksPerSec = qwTicksPerSec.QuadPart;

	QueryPerformanceCounter( &qwTime );
	m_llLastElapsedTime = qwTime.QuadPart;
}

void CMyTime::ProcessTime()
{
	if( !m_bUseQPF )
	{
		// �ý����� QP�� �������ϴ� ���� �ٸ� �ð�ó���� �ʿ�.
		// timeGetTime ��..
		return;
	}	

	LARGE_INTEGER qwTime;
	QueryPerformanceCounter( &qwTime );

	m_fElapsedTime = (float)
		((double)(qwTime.QuadPart - m_llLastElapsedTime)
		/ (double) m_llQPFTicksPerSec );

	m_llLastElapsedTime = qwTime.QuadPart;
}