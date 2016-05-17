#include "GameScene.h"
#include "Renderer.h"
#include "InputClass.h"
#include "ModelClass.h"
#include "VanishingBlock.h"
#include "Camera.h"
#include "ConstVars.h"
#include <time.h>

void GameScene::Start(Camera& camera)
{
	srand(time(NULL));
	float r = (float)rand() / (RAND_MAX + 1);
	float g = (float)rand() / (RAND_MAX + 1);
	float b = (float)rand() / (RAND_MAX + 1);

	m_color = { r, g, b };
	m_backGround = new ModelClass();
	m_backGround->SetPosition(5.0f, -7.0f, 5.0f);
	m_backGround->SetRGB(m_color.x, m_color.y, m_color.z);
	m_backGround->SetTextureName(ConstVars::PLANE_TEX_FILE);
	m_backGround->SetToRectangle(20.0f, 20.0f, { 0.0f, 1.0f, 0.0f });
	AddModel(m_backGround);

	m_currentBlock = new ModelClass();
	m_currentBlock->SetToCube(2, 1, 2);
	m_currentBlock->SetPosition(m_curPos);
	AddModel(m_currentBlock);
	camera.SetProjection(7, 7);
	camera.SetCameraPos(8.0f, 10.0f, -8.0f);
	camera.SetCameraTarget(0.0f, 0.0f, 0.0f);

}

void GameScene::Update(InputClass& input, Camera& camera)
{
	float dy = 1.0f;
	//배경 사각형 설정

	if (!m_currentBlock->IsOnMove()) {
		m_currentBlock->AddMoveToScheduler(0, 0, 8, 1.0f);
		m_currentBlock->AddMoveToScheduler(0, 0, -8, 1.0f);
	}

	
	if (input.IsKeyDown(VK_SPACE))
	{
		m_curPos.y += dy;
		m_currentBlock->StopMove();

		ModelClass* model = new ModelClass();
		model->SetToCube(2, 1, 2);
		model->SetPosition(m_curPos);
		model->SetRGB(m_color.x, m_color.y, m_color.z);
		model->SetTextureName(ConstVars::CONCREAT_TEX_FILE);
		model->SetPosition(m_curPos);

		AddModel(model);
		m_currentBlock = model;


		ModelClass* transModel = new VanishingBlock();
		transModel->SetToCube(1, 1, 1);
		transModel->SetPosition(-0.5f, m_curPos.y, 2);
		transModel->SetRGB(m_color.x, m_color.y, m_color.z);
		transModel->SetTextureName(ConstVars::CONCREAT_TEX_FILE);
		AddModel(transModel);

		m_backGround->AddMoveToScheduler(0.0f, dy, 0.0f, 0.3f);
		m_backGround->SetRGB(m_color.x, m_color.y, m_color.z);
		m_backGround->SetToRectangle(20.0f, 20.0f, { 0.0f, 1.0f, 0.0f });
		camera.MoveCameraFor(0.0f, dy, 0.0f, 0.3f);
		m_color.x = m_color.x + ((((float)rand()) / (RAND_MAX + 1)) * 2 - 1) * 0.05f;
		m_color.y = m_color.y + ((((float)rand()) / (RAND_MAX + 1)) * 2 - 1) * 0.05f;
		m_color.z = m_color.z + ((((float)rand()) / (RAND_MAX + 1)) * 2 - 1) * 0.05f;
	
	}
	
}