#include "MainScene.h"
#include "InputClass.h"
#include "GameScene.h"
#include "ConstVars.h"


void MainScene::Start(Camera& camera)
{
	camera.InitCamera();
	m_background = new ModelClass();
	//m_backGround->SetToRectangle(camera.GetViewSizeWidth(), camera.GetViewSizeHeight(), { 0.0f, 1.0f, 0.0f });
	m_background->SetPosition(0,0,0);
	m_background->SetRGB(1, 1, 1);
	m_background->SetToRectangle(camera.GetViewSizeWidth(), camera.GetViewSizeHeight(), { 0.0f, 0.0f, -1.0f });
	m_background->SetTextureName(ConstVars::MAIN_MENU_TEX_FILE);
	AddModel(m_background);
}

bool MainScene::Update(float dt, InputClass& input, Camera& camera)
{

	if (input.IsKeyDown(VK_SPACE))
	{
		SystemClass::GetInstance()->SetScene(new GameScene());
		return false;
	}
	return true;
}

void MainScene::ShutDown()
{
}
