#pragma once
#include "ModelClass.h"
#include "Camera.h"

class UIModel :
	public ModelClass
{
public:
	UIModel();
	~UIModel();
	void SetUIXY(float x, float y);
	float GetUIX();
	float GetUIY();

	void SetUIPosition(Camera& cam);

protected:
	float m_uiPosx;
	float m_uiPosy;
};

