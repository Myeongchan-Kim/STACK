#pragma once
#include <map>
#include "ModelClass.h"

class UISprite : public ModelClass
{
public:
	UISprite();
	~UISprite();

	void SetToRect();

	void SetToNumber(int number);
	void SetToChar(char c);

	void SetUIXY(float x, float y);
	float GetUIX() { return m_uiPosx; };
	float GetUIY() { return m_uiPosy; };
	float GetWidth() { return m_width; };
	float GetHeight() { return m_width; };

	void SetUIPosition(float x, float y) {
		SetUIXY(x, y);
	};

	void SetUISize(float width, float height) { m_width = width; m_height = height; };
	
	static const float LETTERWIDTH;
	static const float LETTERHEIGHT;
	
	struct LetterWidth
	{
		float startPos;
		float finishPos;
	};

	static std::map<char, LetterWidth> DEFAULT_LETTER_WIDTH;

protected:
	float m_uiPosx;
	float m_uiPosy;
	float m_height;
	float m_width;

};

