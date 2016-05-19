#include "GameScene.h"
#include "Renderer.h"
#include "InputClass.h"
#include "ModelClass.h"
#include "VanishingBlock.h"
#include "Camera.h"
#include "ConstVars.h"
#include <time.h>
#include "UIModel.h"

const float GameScene::DEFAULT_VIEW_WIDTH = 10.0f;
const float GameScene::DEFAULT_VIEW_HEIGHT = 10.0f;

void GameScene::Start(Camera& camera)
{
	srand(time(NULL));
	float r = (float)rand() / (RAND_MAX + 1) + 0.5f;
	float g = (float)rand() / (RAND_MAX + 1) + 0.5f;
	float b = (float)rand() / (RAND_MAX + 1) + 0.5f;

	m_color = { r, g, b };

	//카운트 초기화
	m_currentHeight = m_curPos.y + m_boxSize.y;
	m_countAccumulation = 0;

	//카메라 위치및 방향 지정.
	float viewWidth = DEFAULT_VIEW_WIDTH;
	float viewHeight = DEFAULT_VIEW_HEIGHT;
	camera.SetProjection(viewWidth, viewHeight);
	camera.SetCameraPos(8.0f, 10.0f, -8.0f);
	camera.SetCameraTarget(0.0f, 0.0f, 0.0f);

	//배경 초기화
	m_backGround = new ModelClass();
	auto dir = camera.GetVewDir();
	m_backGround->SetPosition(camera.GetPosition().x + dir.x *10, camera.GetPosition().y + dir.y * 10, camera.GetPosition().z + dir.z * 10);
	m_backGround->SetRGB(m_color.x, m_color.y, m_color.z);
	m_backGround->SetTextureName(ConstVars::PLANE_TEX_FILE);
	m_backGround->SetToRectangle(viewWidth, viewHeight, { 0.0f, 1.0f, 0.0f });
	m_backGround->RotationToCamera(camera);
	AddModel(m_backGround);
	
	//초기에 하나 있는 블록 생성
	m_lastBlock = new ModelClass();
	m_lastBlock->SetToCube(m_boxSize);
	m_lastBlock->SetPosition(m_curPos);
	m_lastBlock->SetRGB(m_color.x, m_color.y, m_color.z);
	AddModel(m_lastBlock);

	m_curPos.y += m_boxSize.y;

	//처음은 z축으로 블록 이동
	m_curMoveDir = { 0, 0, 4 };

	//현재 블록 생성
	m_currentBlock = new ModelClass();
	m_currentBlock->SetToCube(m_boxSize);
	XMFLOAT3 newPosition = { m_curPos.x + m_curMoveDir.x, m_curPos.y + m_curMoveDir.y, m_curPos.z + m_curMoveDir.z };
	m_currentBlock->SetPosition(newPosition);
	m_currentBlock->SetRGB(m_color.x, m_color.y, m_color.z);
	AddModel(m_currentBlock);
}

void GameScene::Update(float dt, InputClass& input, Camera& camera)
{
	float dy = m_boxSize.y;
	//배경 사각형 설정

	if(!m_isEnd)
	{
		if (!m_currentBlock->IsOnMove()) {
			//m_curMoveDir = { 0, 0, 4 };
			//m_curMoveDir = { 4, 0, 0 };
			m_currentBlock->AddMoveToScheduler(m_curMoveDir.x * -2, m_curMoveDir.y, m_curMoveDir.z * -2, 1.0f);
			m_currentBlock->AddMoveToScheduler(m_curMoveDir.x * 2, m_curMoveDir.y, m_curMoveDir.z * 2,  1.0f);
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

				//현재블럭 멈춤.
				m_currentBlock->StopMove();

				//잘린 후 두 블럭의 크기, 위치 지정.
				XMFLOAT3 visibleBlockSize = { m_boxSize.x - lengthX, m_boxSize.y, m_boxSize.z - lengthZ };
				XMFLOAT3 visibleBlockPos = { 
					m_lastBlock->GetPosition().x + (deltaPositionX) / 2 , 
					m_currentBlock->GetPosition().y, 
					m_lastBlock->GetPosition().z + (deltaPositionZ)/ 2 };

				XMFLOAT3 vanishingBlockSize;
				XMFLOAT3 vanishingBlockPos = {
					m_lastBlock->GetPosition().x,
					m_currentBlock->GetPosition().y,
					m_lastBlock->GetPosition().z };

				if (m_curMoveDir.x < 0)
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

				//현재 블록 지우기.
				RemoveModel([=](ModelClass* model) -> bool {
					return model == m_currentBlock;
				});

				//잘린보이는 블럭 생성
				ModelClass* splicedBlock = new ModelClass();
				splicedBlock->SetToCube(visibleBlockSize);
				splicedBlock->SetPosition(visibleBlockPos);
				splicedBlock->SetRGB(m_color.x, m_color.y, m_color.z);
				splicedBlock->SetTextureName(ConstVars::CONCREAT_TEX_FILE);
				AddModel(splicedBlock);

				//잘린 사라지는 블럭 생성
				ModelClass* transModel = new VanishingBlock();
				transModel->SetToCube(vanishingBlockSize);
				transModel->SetPosition(vanishingBlockPos);
				transModel->SetRGB(m_color.x, m_color.y, m_color.z);
				transModel->SetTextureName(ConstVars::CONCREAT_TEX_FILE);
				transModel->AddMoveToScheduler(0.0f, -1.0f, 0.0f, 0.5f);
				AddModel(transModel);

				//밑단 블럭을 잘린 보이는 블록으로 지정.
				m_lastBlock = splicedBlock;

				//새로 만드는 박스 크기를 잘린 보이는 블록과 같게 지정.
				m_boxSize = visibleBlockSize;

				//새로 만드는 박스 높이를 한단계증가.
				m_curPos.y += dy;
				m_curPos.x = visibleBlockPos.x;
				m_curPos.z = visibleBlockPos.z;

				//색상 변경.
				m_color.x = m_color.x + ((((float)rand()) / (RAND_MAX + 1)) * 2 - 1) * 0.05f;
				m_color.y = m_color.y + ((((float)rand()) / (RAND_MAX + 1)) * 2 - 1) * 0.05f;
				m_color.z = m_color.z + ((((float)rand()) / (RAND_MAX + 1)) * 2 - 1) * 0.05f;

				//새 블록 움직이는 방향 설정
				ChangeDirection();
				
				//새 블록 생성.
				ModelClass* newBlock = new ModelClass();
				newBlock->SetToCube(m_boxSize);
				XMFLOAT3 newPosition = 
					{ m_curPos.x + m_curMoveDir.x, m_curPos.y + m_curMoveDir.y, m_curPos.z + m_curMoveDir.z };
				newBlock->SetPosition(newPosition);
				newBlock->SetRGB(m_color.x, m_color.y, m_color.z);
				newBlock->SetTextureName(ConstVars::CONCREAT_TEX_FILE);
				AddModel(newBlock);

				m_currentBlock = newBlock;
				
				//background & camera move
				camera.MoveCameraFor(0.0f, dy, 0.0f, 0.3f);
				XMFLOAT3 v = camera.GetVewDir();
				auto r = dy / v.y;
				m_backGround->MoveBy(-v.x * r, -v.y * r, -v.z * r); 
				m_backGround->AddMoveToScheduler(0.0f, dy, 0.0f, 0.3f);
				m_backGround->SetRGB(m_color.x, m_color.y, m_color.z);
				m_backGround->SetToRectangle(camera.GetViewSizeWidth(), camera.GetViewSizeHeight(), { 0.0f, 1.0f, 0.0f });

				m_currentHeight += dy;
				m_countAccumulation++;
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
		static float elapsedTime = 1.0f;
		elapsedTime += dt;
		
		if (elapsedTime > 2)
			return;

		float viewSize = elapsedTime * m_currentHeight;
		m_backGround->SetScale(viewSize/10, 1, viewSize/10);
		camera.SetProjection(viewSize, viewSize);
	}

	UpdateUI(camera);
}

float GameScene::GetHeight()
{
	return m_currentHeight;
}

int GameScene::GetCount()
{
	return m_countAccumulation;
}

void GameScene::UpdateUI(Camera & camera)
{
	Scene::UpdateUI(camera); //clear

	char showString[20];
	sprintf_s(showString, "%d", m_countAccumulation);
	float scale = camera.GetViewSizeWidth() / DEFAULT_VIEW_WIDTH;
	float startPosX = camera.GetViewSizeWidth() / 2 - strlen(showString) * UIModel::LETTERWIDTH * scale/ 2.0f;


	for (int i = 0; i < strlen(showString); i++)
	{
		char tmp[2] = {0,0};
		tmp[0] = showString[i];
		int num = atoi(tmp);
		auto numberExample = new UIModel();
		numberExample->SetUIXY( (startPosX + i * UIModel::LETTERWIDTH * scale )/ camera.GetViewSizeWidth() , 0.9f);
		numberExample->SetUIPosition(camera);
		numberExample->SetToNumber(num);
		numberExample->SetScale(scale, scale, scale);
		numberExample->RotationToCamera(camera);
		AddUIModel(numberExample);
	}
}

void GameScene::ChangeDirection()
{
	if (m_curMoveDir.x < 0)
		m_curMoveDir = { 0, 0, 4 };
	else
		m_curMoveDir = { -4, 0, 0 };
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