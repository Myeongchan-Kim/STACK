#pragma once
#include "ModelClass.h"
#include "Camera.h"

class UIModel :
	public ModelClass
{
public:
	UIModel();
	virtual ~UIModel() override;

	void SetUIXY(float x, float y);
	float GetUIX();
	float GetUIY();

	void SetUIPosition(Camera& cam);
	void SetToPolygon(std::string filename);
	void SetToNumber(int n);

	static const float LETTERWIDTH;
	static const float LETTERHEIGHT;

protected:
	float m_uiPosx;
	float m_uiPosy;
	float m_height;
	float m_width;
};

