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

GameScene::~GameScene()
{
	for (auto& file : m_scaleSounds)
	{
		SystemClass::GetInstance()->CloseSoundFile(file);
	}
}

void GameScene::Start(Camera& camera)
{
	srand(time(NULL));
	m_randomSeed = rand();
	XMFLOAT4 rgba = MakeCircularRGB(m_randomSeed);

	m_color = { rgba.x , rgba.y, rgba.z };

	//ī��Ʈ �ʱ�ȭ
	m_currentHeight = m_curPos.y + m_boxSize.y;
	m_countAccumulation = 0;

	//ī�޶� ��ġ�� ���� ����.
	float viewWidth = DEFAULT_VIEW_WIDTH;
	float viewHeight = DEFAULT_VIEW_HEIGHT;
	camera.SetProjection(viewWidth, viewHeight);
	camera.SetCameraPos(8.0f, 10.0f, -8.0f);
	camera.SetCameraTarget(0.0f, 0.0f, 0.0f);

	//��� �ʱ�ȭ
	m_backGround = new ModelClass();
	auto dir = camera.GetVewDir();
	m_backGround->SetPosition(camera.GetPosition().x + dir.x *10, camera.GetPosition().y + dir.y * 10, camera.GetPosition().z + dir.z * 10);
	m_backGround->SetRGB(m_color.x, m_color.y, m_color.z);
	m_backGround->SetToRectangle(viewWidth, viewHeight, { 0.0f, 1.0f, 0.0f });
	m_backGround->RotationToCamera(camera);
	AddModel(m_backGround);
	
	//�ʱ⿡ �ϳ� �ִ� ��� ����
	m_lastBlock = MakeNewBlock(m_curPos, m_boxSize);
	AddModel(m_lastBlock);

	m_curPos.y += m_boxSize.y;

	//ó���� z������ ��� �̵�
	m_curMoveDir = { 0, 0, 4 };

	
	//���� ��� ����
	XMFLOAT3 newPosition = { 
		m_curPos.x + m_curMoveDir.x, 
		m_curPos.y + m_curMoveDir.y, 
		m_curPos.z + m_curMoveDir.z 
	};
	m_currentBlock = MakeNewBlock(newPosition, m_boxSize);
	AddModel(m_currentBlock);


	LoadUI();
	UpdateUIString(camera);
	SetSounds();
	m_playState = isPlaying;
}

void GameScene::SetSounds()
{
	m_scaleSounds[0] = "sound/do.mp3";
	m_scaleSounds[1] = "sound/re.mp3";
	m_scaleSounds[2] = "sound/mi.mp3";
	m_scaleSounds[3] = "sound/fa.mp3";
	m_scaleSounds[4] = "sound/sol.mp3";
	m_scaleSounds[5] = "sound/la.mp3";
	m_scaleSounds[6] = "sound/ti.mp3";
	m_scaleSounds[7] = "sound/do2.mp3";

	for (auto file : m_scaleSounds)
	{
		SystemClass::GetInstance()->PreLoadSoundFile(file);
	}
}

bool GameScene::Update(float dt, InputClass& input, Camera& camera)
{
	bool result;
	//��� �簢�� ����
	switch (m_playState)
	{
	case GameScene::isPlaying:
		result = UpdatePlayState( dt, input,camera);
		break;
	case GameScene::isEnding:
		result = UpdateEndingState(dt, input, camera);
		break;
	case GameScene::isEndingDone:
		result = UpdateEndDoneState(dt, input, camera);
		break;
	case GameScene::isRestarting:
		result = UpdateRestarting(dt, input, camera);
		break;
	default:
		break;
	}
	return result;
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

void GameScene::UpdateUIString(Camera & camera)
{
	Scene::UpdateUIString(camera); //clear

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
		numberModel->SetRGB(3.0f, 3.0f, 3.0f);
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

bool GameScene::UpdatePlayState(float dt, InputClass & input, Camera & camera)
{
	float dy = m_boxSize.y;

	if (!m_currentBlock->IsOnMove()) { //�ݺ��ؼ� ���ڰ� �Դٰ��� �ϰ� ��.
		//m_curMoveDir = { 0, 0, 4 };
		//m_curMoveDir = { 4, 0, 0 };
		m_currentBlock->AddLinearMoveToScheduler(m_curMoveDir.x * -2, m_curMoveDir.y, m_curMoveDir.z * -2, 1.0f);
		m_currentBlock->AddLinearMoveToScheduler(m_curMoveDir.x * 2, m_curMoveDir.y, m_curMoveDir.z * 2, 1.0f);
	}

	if (input.IsKeyDown(VK_SPACE))
	{
		if (IsExactFit(m_currentBlock, m_lastBlock))
		{
			std::string sound = m_scaleSounds[m_exactFitCount++ % 8];
			SystemClass::GetInstance()->PlaySoundFile(sound);
			//����� ����.
			m_currentBlock->StopMove();

			m_currentBlock->SetPosition(
				m_lastBlock->GetPosition().x,
				m_lastBlock->GetPosition().y + dy,
				m_lastBlock->GetPosition().z);

			//���� ����.
			auto newRGBA = MakeCircularRGB(m_randomSeed + m_countAccumulation);
			m_color = { newRGBA.x, newRGBA.y, newRGBA.z };

			//�� ��� �����̴� ���� ����
			ChangeDirection();

			//�ش� ���� �߸� ���̴� ������� ����.
			m_lastBlock = m_currentBlock;;

			//���� ����� �ڽ� ���̸� �Ѵܰ�����.
			m_curPos.y += dy;

			//�� ��� ����.
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
			UpdateUIString(camera);
		}
		else if (IsOn(m_currentBlock, m_lastBlock))
		{
			m_exactFitCount = 0;
			float deltaPositionZ = m_currentBlock->GetPosition().z - m_lastBlock->GetPosition().z;
			float deltaPositionX = m_currentBlock->GetPosition().x - m_lastBlock->GetPosition().x;
			float lengthZ = deltaPositionZ;
			float lengthX = deltaPositionX;
			if (deltaPositionZ < 0)
				lengthZ = -deltaPositionZ;
			if (deltaPositionX < 0)
				lengthX = -deltaPositionX;

			//����� ����.
			m_currentBlock->StopMove();

			//�߸� �� �� ���� ũ��, ��ġ ����.
			XMFLOAT3 visibleBlockSize = { m_boxSize.x - lengthX, m_boxSize.y, m_boxSize.z - lengthZ };
			XMFLOAT3 visibleBlockPos = {
				m_lastBlock->GetPosition().x + (deltaPositionX) / 2 ,
				m_currentBlock->GetPosition().y,
				m_lastBlock->GetPosition().z + (deltaPositionZ) / 2 };

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

			//���� ��� �����.
			RemoveModel([=](ModelClass* model) -> bool {
				bool result = (model == m_currentBlock);
				if (result) delete model;
				return result;
			});

			//���� ����� �ڽ� ũ�⸦ �߸� ���̴� ��ϰ� ���� ����.
			m_boxSize = visibleBlockSize;

			//�߸����̴� �� ����
			ModelClass* splicedBlock = MakeNewBlock(visibleBlockPos, visibleBlockSize);
			AddModel(splicedBlock);
			//�ش� ���� �߸�������� ����.
			m_lastBlock = splicedBlock;

			//�߸� ������� �� ����
			ModelClass* transModel = new VanishingBlock();
			transModel->SetToCube(vanishingBlockSize);
			transModel->SetPosition(vanishingBlockPos);
			transModel->SetRGB(m_color.x, m_color.y, m_color.z);
			transModel->SetTextureName(ConstVars::CONCREAT_TEX_FILE);
			transModel->AddGravityMoveToScheduler({ deltaPositionX, 0.0f, deltaPositionZ }, 2.0f);
			AddModel(transModel);

			//���� ����� �ڽ� ���̸� �Ѵܰ�����.
			m_curPos.y += dy;
			m_curPos.x = visibleBlockPos.x;
			m_curPos.z = visibleBlockPos.z;

			//���� ����.
			auto newRGBA = MakeCircularRGB(m_randomSeed + m_countAccumulation);
			m_color = { newRGBA.x, newRGBA.y, newRGBA.z };
			//�� ��� �����̴� ���� ����
			ChangeDirection();

			//�� ��� ����.
			XMFLOAT3 newPosition = { m_curPos.x + m_curMoveDir.x, m_curPos.y + m_curMoveDir.y, m_curPos.z + m_curMoveDir.z };
			m_currentBlock = MakeNewBlock(newPosition, m_boxSize);
			AddModel(m_currentBlock);

			//background & camera move
			MoveCameraAndBackground(camera, dy);

			m_currentHeight += dy;
			m_countAccumulation++;
			UpdateUIString(camera);
			//UpdateUIPos(camera);
		}
		else
		{
			m_exactFitCount = 0;
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
			m_playState = isEnding;
		}
	}
	return DONTKILL;
}

bool GameScene::UpdateEndingState(float dt, InputClass & input, Camera & camera)
{
	static float elapsedTime = 0.0f;
	elapsedTime += dt;

	if (elapsedTime > 1.0f)
	{
		elapsedTime = 0.0f;
		m_playState = isEndingDone;
	}
	else 
	{
		float viewSize = 10.0f + elapsedTime * m_currentHeight * 1.5f;
		m_backGround->SetScale(viewSize / 10, viewSize / 10, viewSize / 10);
		camera.SetProjection(viewSize, viewSize);
		UpdateUIPos(camera);
	}
	return DONTKILL;
}

bool GameScene::UpdateEndDoneState(float dt, InputClass & input, Camera & camera)
{
	if (input.IsKeyDown(VK_SPACE))
		m_playState = isRestarting;
	return DONTKILL;
}

bool GameScene::UpdateRestarting(float dt, InputClass & input, Camera & camera)
{
	m_playState = playState::isRestarting;
	float destructionTime = 0.5f;;
	for (auto& model : m_modelsToBeRendered)
	{
		if (model == m_backGround)
			continue;
		model->AddGravityMoveToScheduler({ float(rand() % 20) - 10.0f, float(rand() % 5), float(rand() % 20) - 10.0f }, destructionTime * 2);
	}
	static float elapsedTime = 0.0f;
	elapsedTime += dt;
	if (elapsedTime > destructionTime)
	{
		elapsedTime = 0.0f;
		return KILLME;
	}
	return DONTKILL;
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
