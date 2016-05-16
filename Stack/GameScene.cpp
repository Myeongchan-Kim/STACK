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
	auto backGround = new ModelClass();
	backGround->SetPosition(5.0f, -7.0f, 5.0f);
	backGround->SetRGB(m_color.x, m_color.y, m_color.z);
	backGround->SetTextureName(ConstVars::PLANE_TEX_FILE);
	backGround->SetToRectangle(20.0f, 20.0f, { 0.0f, 1.0f, 0.0f });
	m_backGround = backGround;
	AddModel(backGround);
	camera.SetCameraPos(-8.0f, 10.0f, -8.0f);
	camera.SetCameraTarget(0.0f, 0.0f, 0.0f);

}

void GameScene::Update(InputClass& input, Camera& camera)
{
	static float count = 0;
	float dy = 1.0f;
	//배경 사각형 설정
	if (input.IsKeyDown(VK_SPACE))
	{
		ModelClass* model = new ModelClass();
		model->SetToCube(2, 1, 2);
		model->SetPosition(0, count, 0);
		model->SetRGB(m_color.x, m_color.y, m_color.z);
		model->SetTextureName(ConstVars::CONCREAT_TEX_FILE);
		AddModel(model);

		ModelClass* transModel = new VanishingBlock();
		transModel->SetToCube(1, 1, 1);
		transModel->SetPosition(-0.5f, count, 2);
		transModel->SetRGB(m_color.x, m_color.y, m_color.z);
		transModel->SetTextureName(ConstVars::CONCREAT_TEX_FILE);
		AddModel(transModel);
		count += dy;

		m_backGround->AddMoveToScheduler(0.0f, dy, 0.0f, 0.3f);
		m_backGround->SetRGB(m_color.x, m_color.y, m_color.z);
		m_backGround->SetToRectangle(20.0f, 20.0f, { 0.0f, 1.0f, 0.0f });
		camera.MoveCameraFor(0.0f, dy, 0.0f, 0.3f);
		m_color.x = m_color.x + ((((float)rand()) / (RAND_MAX + 1)) * 2 - 1) * 0.05f;
		m_color.y = m_color.y + ((((float)rand()) / (RAND_MAX + 1)) * 2 - 1) * 0.05f;
		m_color.z = m_color.z + ((((float)rand()) / (RAND_MAX + 1)) * 2 - 1) * 0.05f;
		
//      	RemoveModel(std::function<bool(ModelClass* model)>([&] (ModelClass* model) -> bool {
// 			return true;
// 		}));
	}

}