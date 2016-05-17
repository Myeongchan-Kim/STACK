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
	float r = (float)rand() / (RAND_MAX + 1) + 0.5f;
	float g = (float)rand() / (RAND_MAX + 1) + 0.5f;
	float b = (float)rand() / (RAND_MAX + 1) + 0.5f;

	m_color = { r, g, b };

	m_backGround = new ModelClass();
	m_backGround->SetPosition(-7.0f, m_curPos.y - 12, 7.0f);
	m_backGround->SetRGB(m_color.x, m_color.y, m_color.z);
	m_backGround->SetTextureName(ConstVars::PLANE_TEX_FILE);
	m_backGround->SetToRectangle(20.0f, 20.0f, { 0.0f, 1.0f, 0.0f });
	AddModel(m_backGround);

	
	m_lastBlock = new ModelClass();
	m_lastBlock->SetToCube(m_boxSize);
	m_lastBlock->SetPosition(m_curPos);
	m_lastBlock->SetRGB(m_color.x, m_color.y, m_color.z);
	AddModel(m_lastBlock);

	m_curPos.y += 1.0f;


	m_currentBlock = new ModelClass();
	m_currentBlock->SetToCube(m_boxSize);
	m_currentBlock->SetPosition(m_curPos);
	m_currentBlock->SetRGB(m_color.x, m_color.y, m_color.z);
	AddModel(m_currentBlock);
	camera.SetProjection(7, 7);
	camera.SetCameraPos(8.0f, 10.0f, -8.0f);
	camera.SetCameraTarget(0.0f, 0.0f, 0.0f);

}

void GameScene::Update(float dt, InputClass& input, Camera& camera)
{
	float dy = 1.0f;
	//배경 사각형 설정

	if(!m_isEnd)
	{
		if (!m_currentBlock->IsOnMove()) {

			m_currentBlock->AddMoveToScheduler(0, 0, 4, 0.5f);
			m_currentBlock->AddMoveToScheduler(0, 0, -8, 1.0f);
			m_currentBlock->AddMoveToScheduler(0, 0, 4, 0.5f);
		}


		if (input.IsKeyDown(VK_SPACE))
		{
			m_curPos.y += dy;
			m_currentBlock->StopMove();

			if (IsOn(m_currentBlock, m_lastBlock))
			{
				ModelClass* model = new ModelClass();
				model->SetToCube(m_boxSize);
				model->SetPosition(m_curPos);
				model->SetRGB(m_color.x, m_color.y, m_color.z);
				model->SetTextureName(ConstVars::CONCREAT_TEX_FILE);

				AddModel(model);
				m_lastBlock = m_currentBlock;
				m_currentBlock = model;


				m_backGround->AddMoveToScheduler(0.0f, dy, 0.0f, 0.3f);
				m_backGround->SetRGB(m_color.x, m_color.y, m_color.z);
				m_backGround->SetToRectangle(20.0f, 20.0f, { 0.0f, 1.0f, 0.0f });
				camera.MoveCameraFor(0.0f, dy, 0.0f, 0.3f);


				m_color.x = m_color.x + ((((float)rand()) / (RAND_MAX + 1)) * 2 - 1) * 0.05f;
				m_color.y = m_color.y + ((((float)rand()) / (RAND_MAX + 1)) * 2 - 1) * 0.05f;
				m_color.z = m_color.z + ((((float)rand()) / (RAND_MAX + 1)) * 2 - 1) * 0.05f;
			}
			else
			{
				ModelClass* transModel = new VanishingBlock();
				transModel->SetToCube(m_boxSize);
				transModel->SetPosition(-0.5f, m_curPos.y, 2);
				transModel->SetRGB(m_color.x, m_color.y, m_color.z);
				transModel->SetTextureName(ConstVars::CONCREAT_TEX_FILE);
				AddModel(transModel);
				RemoveModel([=](ModelClass* model) -> bool {
					return model == m_currentBlock;
				});

				m_isEnd = true;
			}


		}
	}

	else
	{
		static float elapsedTime = 0.0f;
		if (elapsedTime > 3)
			return;
		elapsedTime += dt;
		float moveratio = elapsedTime / 3;

		camera.SetCameraTarget(m_curPos.x * moveratio, (m_curPos.y / 2) * moveratio, m_curPos.z * moveratio);
		camera.SetProjection(20 * moveratio, 20 * moveratio);
	}
	
}

bool GameScene::IsOn(ModelClass* b1, ModelClass* b2)
{
	if (!b1 || !b2)
		return false;

	if (
		b1->GetPosition().x > b2->GetPosition().x - m_boxSize.x &&
		b1->GetPosition().x < b2->GetPosition().x + m_boxSize.x &&
		b1->GetPosition().z > b2->GetPosition().z - m_boxSize.z &&
		b1->GetPosition().z < b2->GetPosition().z + m_boxSize.z
	)
	{
		return true;
	}
	return false;
}