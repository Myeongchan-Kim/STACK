#include "GameScene.h"
#include "Renderer.h"
#include "InputClass.h"
#include "ModelClass.h"
#include "VanishingBlock.h"
#include "Camera.h"
#include "ConstVars.h"
#include <Model.h>
#include <time.h>
#include "UIModel.h"

const float GameScene::DEFAULT_VIEW_WIDTH = 10.0f;
const float GameScene::DEFAULT_VIEW_HEIGHT = 10.0f;
const XMFLOAT3 GameScene::DEFAULT_BOXSIZE = { 4, 1.0, 4 };

void GameScene::Start(Camera& camera)
{
	srand(time(NULL));
	m_randomSeed = rand();
	XMFLOAT4 rgba = MakeCircularRGB(m_randomSeed);

	m_color = { rgba.x , rgba.y, rgba.z };

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
	m_lastBlock = MakeNewBlock(m_curPos, m_boxSize);
	AddModel(m_lastBlock);

	m_curPos.y += m_boxSize.y;

	//처음은 z축으로 블록 이동
	m_curMoveDir = { 0, 0, 4 };

	
	//현재 블록 생성
	XMFLOAT3 newPosition = { 
		m_curPos.x + m_curMoveDir.x, 
		m_curPos.y + m_curMoveDir.y, 
		m_curPos.z + m_curMoveDir.z 
	};
	m_currentBlock = MakeNewBlock(newPosition, m_boxSize);
	AddModel(m_currentBlock);


	LoadUI();
	UpdateUI(camera);
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
			m_currentBlock->AddLinearMoveToScheduler(m_curMoveDir.x * -2, m_curMoveDir.y, m_curMoveDir.z * -2, 1.0f);
			m_currentBlock->AddLinearMoveToScheduler(m_curMoveDir.x * 2, m_curMoveDir.y, m_curMoveDir.z * 2,  1.0f);
		}


		if (input.IsKeyDown(VK_SPACE))
		{
			if (IsExactFit(m_currentBlock, m_lastBlock))
			{
				//현재블럭 멈춤.
				m_currentBlock->StopMove();

				m_currentBlock->SetPosition(
					m_lastBlock->GetPosition().x, 
					m_lastBlock->GetPosition().y + dy, 
					m_lastBlock->GetPosition().z);

				//색상 변경.
				auto newRGBA = MakeCircularRGB(m_randomSeed + m_countAccumulation);
				m_color = { newRGBA.x, newRGBA.y, newRGBA.z };
				
				//새 블록 움직이는 방향 설정
				ChangeDirection();
				
				//밑단 블럭을 잘린 보이는 블록으로 지정.
				m_lastBlock = m_currentBlock;;

				//새로 만드는 박스 높이를 한단계증가.
				m_curPos.y += dy;
				
				//새 블록 생성.
				XMFLOAT3 newPosition = { 
					m_curPos.x + m_curMoveDir.x, 
					m_curPos.y + m_curMoveDir.y, 
					m_curPos.z + m_curMoveDir.z 
				};

				m_currentBlock = MakeNewBlock(newPosition, m_boxSize);
				AddModel(m_currentBlock);

				//background & camera move
				MoveCameraAndBackground(camera, dy);
				
				m_currentHeight += dy;
				m_countAccumulation++;
				UpdateUI(camera);
				//UpdateUIPos(camera);
			}
			else if (IsOn(m_currentBlock, m_lastBlock))
			{
				float deltaPositionZ = m_currentBlock->GetPosition().z - m_lastBlock->GetPosition().z;
				float deltaPositionX = m_currentBlock->GetPosition().x - m_lastBlock->GetPosition().x;
				float lengthZ = deltaPositionZ;
				float lengthX = deltaPositionX;
				if (deltaPositionZ < 0)
					lengthZ = -deltaPositionZ;
				if (deltaPositionX < 0)
					lengthX = -deltaPositionX;

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
					bool result = (model == m_currentBlock);
					if( result) delete model;
					return result;
				});

				//새로 만드는 박스 크기를 잘린 보이는 블록과 같게 지정.
				m_boxSize = visibleBlockSize;

				//잘린보이는 블럭 생성
				ModelClass* splicedBlock = MakeNewBlock(visibleBlockPos, visibleBlockSize);
				AddModel(splicedBlock);
				//밑단 블럭을 잘린블록으로 지정.
				m_lastBlock = splicedBlock;

				//잘린 사라지는 블럭 생성
				ModelClass* transModel = new VanishingBlock();
				transModel->SetToCube(vanishingBlockSize);
				transModel->SetPosition(vanishingBlockPos);
				transModel->SetRGB(m_color.x, m_color.y, m_color.z);
				transModel->SetTextureName(ConstVars::CONCREAT_TEX_FILE);
				transModel->AddGravityMoveToScheduler({deltaPositionX, 0.0f, deltaPositionZ}, 2.0f);
				AddModel(transModel);

				//새로 만드는 박스 높이를 한단계증가.
				m_curPos.y += dy;
				m_curPos.x = visibleBlockPos.x;
				m_curPos.z = visibleBlockPos.z;

				//색상 변경.
				auto newRGBA = MakeCircularRGB(m_randomSeed + m_countAccumulation);
				m_color = {newRGBA.x, newRGBA.y, newRGBA.z};
				//새 블록 움직이는 방향 설정
				ChangeDirection();
				
				//새 블록 생성.
				XMFLOAT3 newPosition = { m_curPos.x + m_curMoveDir.x, m_curPos.y + m_curMoveDir.y, m_curPos.z + m_curMoveDir.z };
				m_currentBlock = MakeNewBlock(newPosition, m_boxSize);
				AddModel(m_currentBlock);
				
				//background & camera move
				MoveCameraAndBackground(camera, dy);

				m_currentHeight += dy;
				m_countAccumulation++;
				UpdateUI(camera);
				//UpdateUIPos(camera);
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
		
		if (elapsedTime > 1.0f)
			return;

		float viewSize = 10.0f + elapsedTime * m_currentHeight * 1.5f;
		m_backGround->SetScale(viewSize/10, viewSize / 10, viewSize/10);
		camera.SetProjection(viewSize, viewSize);
		UpdateUIPos(camera);
	}

}

float GameScene::GetHeight()
{
	return m_currentHeight;
}

int GameScene::GetCount()
{
	return m_countAccumulation;
}

void GameScene::LoadUI()
{
	for (int i = 0; i < 10; i++)
	{
		char fileName[20] = { 0, };
		sprintf_s(fileName, "Object/number0.obj");
		fileName[13] = '0' + i;
		m_uiPool[i] = ModelClass::PreLoadFromFile(fileName);
	}
}

void GameScene::UpdateUI(Camera & camera)
{
	Scene::UpdateUI(camera); //clear

	char showString[20];
	sprintf_s(showString, "%d", m_countAccumulation);
	float scale = camera.GetViewSizeWidth() / DEFAULT_VIEW_WIDTH;
	float startPosX = camera.GetViewSizeWidth() / 2 - (strlen(showString) - 0.5) * UIModel::LETTERWIDTH * scale/ 2.0f;


	for (int i = 0; i < strlen(showString); i++)
	{
		int number = showString[i] - '0';
		auto numberModel = new UIModel();
		numberModel->SetUIXY((startPosX + i * UIModel::LETTERWIDTH * scale) / camera.GetViewSizeWidth(), 0.9f);
		numberModel->LoadFromPreLoadedData(m_uiPool[number]);
		numberModel->SetScale(scale * 0.05, scale * 0.05, scale * 0.05);
		numberModel->RotationToCamera(camera);
		AddUIModel(numberModel);
	}
}

void GameScene::UpdateUIPos(Camera & camera)
{
	float scale = camera.GetViewSizeWidth() / DEFAULT_VIEW_WIDTH;
	for (auto& model : m_UImodel)
	{
		model->RotationToCamera(camera);
		model->SetScale(scale * 0.05, scale * 0.05, scale * 0.05);
	}
}



void GameScene::MoveCameraAndBackground(Camera & camera, float dy)
{
	camera.MoveCameraFor(0.0f, dy, 0.0f, 0.3f);
	XMFLOAT3 v = camera.GetVewDir();
	auto r = dy / v.y;
	m_backGround->MoveBy(-v.x * r, -v.y * r, -v.z * r);
	m_backGround->AddLinearMoveToScheduler(0.0f, dy, 0.0f, 0.3f);
	m_backGround->SetRGB(m_color.x - 0.2f, m_color.y - 0.2f, m_color.z - 0.2f);
	m_backGround->SetToRectangle(camera.GetViewSizeWidth(), camera.GetViewSizeHeight(), { 0.0f, 1.0f, 0.0f });
}

ModelClass * GameScene::MakeNewBlock(XMFLOAT3 Position, XMFLOAT3 boxSize)
{
	ModelClass* newBlock = new ModelClass();
	newBlock->SetToCube(boxSize);
	newBlock->SetPosition(Position);
	newBlock->SetRGB(m_color.x, m_color.y, m_color.z);
	newBlock->SetTextureName(ConstVars::CONCREAT_TEX_FILE);
	return newBlock;
}

void GameScene::ChangeDirection()
{
	float moveVelocity = log(m_countAccumulation + 1) + 3;
	if (m_curMoveDir.x < 0)
	{	
		m_curMoveDir = { 0, 0, moveVelocity };
	}
	else
	{
		m_curMoveDir = { -moveVelocity, 0, 0 };
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
		return true;
	else
		return false;
}

bool GameScene::IsExactFit(ModelClass * ubox, ModelClass * dbox)
{
	float allowDelta = GameScene::DEFAULT_BOXSIZE.x / 30.0f;
	if (
		ubox->GetPosition().x > dbox->GetPosition().x - allowDelta &&
		ubox->GetPosition().x < dbox->GetPosition().x + allowDelta &&
		ubox->GetPosition().z > dbox->GetPosition().z - allowDelta &&
		ubox->GetPosition().z < dbox->GetPosition().z + allowDelta 
		)
		return true;
	else
		return false;
}
