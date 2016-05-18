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

	//��� �ʱ�ȭ
	m_backGround = new ModelClass();
	m_backGround->SetPosition(-7.0f, m_curPos.y - 12, 7.0f);
	m_backGround->SetRGB(m_color.x, m_color.y, m_color.z);
	m_backGround->SetTextureName(ConstVars::PLANE_TEX_FILE);
	m_backGround->SetToRectangle(20.0f, 20.0f, { 0.0f, 1.0f, 0.0f });
	AddModel(m_backGround);

	//�ʱ⿡ �ϳ� �ִ� ��� ����
	m_lastBlock = new ModelClass();
	m_lastBlock->SetToCube(m_boxSize);
	m_lastBlock->SetPosition(m_curPos);
	m_lastBlock->SetRGB(m_color.x, m_color.y, m_color.z);
	AddModel(m_lastBlock);

	m_curPos.y += 1.0f;

	//���� ��� ����
	m_currentBlock = new ModelClass();
	m_currentBlock->SetToCube(m_boxSize);
	m_currentBlock->SetPosition(m_curPos);
	m_currentBlock->SetRGB(m_color.x, m_color.y, m_color.z);
	AddModel(m_currentBlock);
	
	//ó���� z������ ��� �̵�
	m_curMoveDir = { 0, 0, 4 };

	//ī�޶� ��ġ�� ���� ����.
	camera.SetProjection(7, 7);
	camera.SetCameraPos(8.0f, 10.0f, -8.0f);
	camera.SetCameraTarget(0.0f, 0.0f, 0.0f);

}

void GameScene::Update(float dt, InputClass& input, Camera& camera)
{
	float dy = 1.0f;
	//��� �簢�� ����

	if(!m_isEnd)
	{
		if (!m_currentBlock->IsOnMove()) {
			//m_curMoveDir = { 0, 0, 4 };
			//m_curMoveDir = { 4, 0, 0 };
			m_currentBlock->AddMoveToScheduler(m_curMoveDir.x, m_curMoveDir.y, m_curMoveDir.z, 0.5f);
			m_currentBlock->AddMoveToScheduler(m_curMoveDir.x * -2, m_curMoveDir.y, m_curMoveDir.z * -2, 1.0f);
			m_currentBlock->AddMoveToScheduler(m_curMoveDir.x, m_curMoveDir.y, m_curMoveDir.z,  0.5f);
		}


		if (input.IsKeyDown(VK_SPACE))
		{

			if (IsOn(m_currentBlock, m_lastBlock))
			{
				float lengthZ = m_currentBlock->GetPosition().z - m_lastBlock->GetPosition().z;
				float lengthX = m_currentBlock->GetPosition().x - m_lastBlock->GetPosition().x;
				float deltaPositionZ = lengthZ;
				float deltaPositionX = lengthX;
				if (lengthZ < 0)
					lengthZ = -lengthZ;
				if (lengthX < 0)
					lengthX = -lengthX;

				//����� ����.
				m_currentBlock->StopMove();

				//�߸� �� �� ���� ũ��, ��ġ ����.
				Vector3 visibleBlockSize = { m_boxSize.x - lengthX, m_boxSize.y, m_boxSize.z - lengthZ };
				Vector3 visibleBlockPos = { 
					m_lastBlock->GetPosition().x + (deltaPositionX) / 2 , 
					m_currentBlock->GetPosition().y, 
					m_lastBlock->GetPosition().z + (deltaPositionZ)/ 2 };

				Vector3 vanishingBlockSize;
				Vector3 vanishingBlockPos = {
					m_lastBlock->GetPosition().x,
					m_currentBlock->GetPosition().y,
					m_lastBlock->GetPosition().z };

				if (m_curMoveDir.x > 0)
				{
					vanishingBlockSize = { lengthX, m_boxSize.y,  m_boxSize.z };

					if (deltaPositionX < 0)
						vanishingBlockPos.x -= (visibleBlockSize.x / 2 + vanishingBlockSize.x);
					else
						vanishingBlockPos.x += (visibleBlockSize.x / 2 + vanishingBlockSize.x);
				}
				else
				{
					vanishingBlockSize = { m_boxSize.x, m_boxSize.y,  lengthZ };

					if (deltaPositionZ < 0)
						vanishingBlockPos.z -= (visibleBlockSize.z / 2 + vanishingBlockSize.z);
					else
						vanishingBlockPos.z += (visibleBlockSize.z / 2 + vanishingBlockSize.z);
				}

				//���� ��� �����.
				RemoveModel([=](ModelClass* model) -> bool {
					return model == m_currentBlock;
				});

				//�߸����̴� �� ����
				ModelClass* splicedBlock = new ModelClass();
				splicedBlock->SetToCube(visibleBlockSize);
				splicedBlock->SetPosition(visibleBlockPos);
				splicedBlock->SetRGB(m_color.x, m_color.y, m_color.z);
				splicedBlock->SetTextureName(ConstVars::CONCREAT_TEX_FILE);
				AddModel(splicedBlock);

				//�߸� ������� �� ����
				ModelClass* transModel = new VanishingBlock();
				transModel->SetToCube(vanishingBlockSize);
				transModel->SetPosition(vanishingBlockPos);
				transModel->SetRGB(m_color.x, m_color.y, m_color.z);
				transModel->SetTextureName(ConstVars::CONCREAT_TEX_FILE);
				transModel->AddMoveToScheduler(0.0f, -1.0f, 0.0f, 0.5f);
				AddModel(transModel);

				//�ش� ���� �߸� ���̴� ������� ����.
				m_lastBlock = splicedBlock;

				//���� ����� �ڽ� ũ�⸦ �߸� ���̴� ��ϰ� ���� ����.
				m_boxSize = visibleBlockSize;

				//���� ����� �ڽ� ���̸� �Ѵܰ�����.
				m_curPos.y += dy;
				m_curPos.x = visibleBlockPos.x;
				m_curPos.z = visibleBlockPos.z;

				//���� ����.
				m_color.x = m_color.x + ((((float)rand()) / (RAND_MAX + 1)) * 2 - 1) * 0.05f;
				m_color.y = m_color.y + ((((float)rand()) / (RAND_MAX + 1)) * 2 - 1) * 0.05f;
				m_color.z = m_color.z + ((((float)rand()) / (RAND_MAX + 1)) * 2 - 1) * 0.05f;

				//�� ��� ����.
				ModelClass* newBlock = new ModelClass();
				newBlock->SetToCube(m_boxSize);
				newBlock->SetPosition(m_curPos);
				newBlock->SetRGB(m_color.x, m_color.y, m_color.z);
				newBlock->SetTextureName(ConstVars::CONCREAT_TEX_FILE);
				AddModel(newBlock);

				m_currentBlock = newBlock;
				
				//�� ��� �����̴� ���� ����
				if (m_curMoveDir.x > 0)
					m_curMoveDir = { 0, 0, 4 };
				else
					m_curMoveDir = { 4, 0, 0 };

				m_backGround->AddMoveToScheduler(0.0f, dy, 0.0f, 0.3f);
				m_backGround->SetRGB(m_color.x, m_color.y, m_color.z);
				m_backGround->SetToRectangle(20.0f, 20.0f, { 0.0f, 1.0f, 0.0f });
				camera.MoveCameraFor(0.0f, dy, 0.0f, 0.3f);


			}
			else
			{
				
				ModelClass* transModel = new VanishingBlock();
				transModel->SetToCube(m_boxSize);
				transModel->SetPosition(m_currentBlock->GetPosition().x, m_curPos.y, m_currentBlock->GetPosition().z);
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
		elapsedTime += dt;
		if (elapsedTime > 0 && elapsedTime < 1)
			return;

		if (elapsedTime > 10)
			return;
		float moveratio = (elapsedTime-1) / 3 + 0.3;

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