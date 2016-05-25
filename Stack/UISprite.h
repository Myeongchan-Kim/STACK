#pragma once
#include "ModelClass.h"

class UISprite : public ModelClass
{
public:
	UISprite();
	~UISprite();

	void SetToNumber(int number);
	void SetUIXY(float x, float y);
	float GetUIX() { return m_uiPosx; };
	float GetUIY() { return m_uiPosy; };

	void SetUIPosition(float x, float y) {
		SetUIXY(x, y);
	};
	
	static const float LETTERWIDTH;
	static const float LETTERHEIGHT;

protected:
	float m_uiPosx;
	float m_uiPosy;
	float m_height;
	float m_width;
};

