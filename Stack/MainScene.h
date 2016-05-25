#pragma once
#include "Scene.h"
class UISprite;

class MainScene :
	public Scene
{
public:

	virtual void Start(Camera& camera) override;


	virtual bool Update(float dt, InputClass& input, Camera& camera) override;

	virtual void ShutDown() override;

private:
	float m_elasedTime;
	UISprite* m_background = nullptr;

};

