#include "GameScene.h"
#include "VanishingBlock.h"
#include "MainScene.h"
#include "ConstVars.h"
#include <Model.h>
#include <time.h>
#include "UIModel.h"
#include "Logger.h"

const XMFLOAT3 GameScene::DEFAULT_BOXSIZE = { 4, 1.0, 4 };

void GameScene::ShutDown()
{

	for (auto& file : m_scaleSounds)
	{
		SystemClass::GetInstance()->CloseSoundFile(file);
	}

	if (m_logger != nullptr)
	{
		m_logger->ShutDown();
		delete m_logger;
		m_logger = nullptr;
	}
}


void GameScene::Start(Camera& camera)
{
	int width = 1024;
	int height = 768;
	srand(time(NULL));
	m_randomSeed = rand();
	XMFLOAT4 rgba = MakeCircularRGB(m_randomSeed);
	m_color = {rgba.x, rgba.y, rgba.z};

	m_logger = new Logger();
	m_maxCount = m_logger->LoadRecord();

	m_color = { rgba.x , rgba.y, rgba.z };

	//카운트 초기화
	m_currentHeight = m_curPos.y + m_boxSize.y;
	m_countAccumulation = 0;

	//카메라 위치및 방향 지정.
	float viewWidth = ConstVars::DEFAULT_VIEW_WIDTH;
	float viewHeight = ConstVars::DEFAULT_VIEW_HEIGHT;
	camera.SetProjection(viewWidth, viewHeight);
	camera.SetCameraPos(8.0f, 9.0f, -8.0f);
	camera.SetCameraTarget(0.0f, -1.0f, 0.0f);

	//배경 초기화
	m_backGround = new ModelClass();
	auto dir = camera.GetVewDir(); 
	m_backGround->SetPosition(camera.GetPosition().x + dir.x * 10, camera.GetPosition().y + dir.y * 10, camera.GetPosition().z + dir.z * 10);
	m_backGround->SetRGB(m_color.x, m_color.y, m_color.z); 
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
	UpdateUIString(camera);
	SetSounds();
	m_playState = isPlaying;
}

void GameScene::SetSounds()
{
	m_scaleSounds[0] = ConstVars::SOUND_FILE1;
	m_scaleSounds[1] = ConstVars::SOUND_FILE2;
	m_scaleSounds[2] = ConstVars::SOUND_FILE3;
	m_scaleSounds[3] = ConstVars::SOUND_FILE4;
	m_scaleSounds[4] = ConstVars::SOUND_FILE5;
	m_scaleSounds[5] = ConstVars::SOUND_FILE6;
	m_scaleSounds[6] = ConstVars::SOUND_FILE7;
	m_scaleSounds[7] = ConstVars::SOUND_FILE8;

}

//false를 반환하면 이 씬을 더이상 실행 하지 않음.
bool GameScene::Update(float dt, InputClass& input, Camera& camera)
{
	bool result;
	//배경 사각형 설정
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

void GameScene::UpdateRecord(float dy)
{
	m_currentHeight += dy;
	m_countAccumulation++;

	if (m_countAccumulation >= m_maxCount)
		m_maxCount = m_countAccumulation;
	if (m_currentHeight > m_maxHeight)
		m_maxHeight = m_currentHeight;
}

void GameScene::UpdateUIString(Camera & camera)
{
	Scene::UpdateUIString(camera); //clear

	char showString[20];
	sprintf_s(showString, "%d", m_countAccumulation);
	float scale = camera.GetViewSizeWidth() / ConstVars::DEFAULT_VIEW_WIDTH;
	float letterInvterval = 0.00f;

	for (int i = 0; i < strlen(showString); i++)
	{
		int number = showString[i] - '0';
		float startPosX = -((int)strlen(showString) - 1) * (UISprite::LETTERWIDTH + letterInvterval) / 2.0f;

		// UI sprite
		auto uiCharacter = new UISprite();
		uiCharacter->SetUIXY(startPosX + (UISprite::LETTERWIDTH + letterInvterval) * i, 0.85f);
		uiCharacter->SetToNumber(number);
		uiCharacter->SetTextureName(ConstVars::CHAR_TEX_FILE);
		m_UISprites.emplace_back(uiCharacter);
	}

	char maxString[20];
	sprintf_s(maxString, "%d", m_maxCount);
	for (int i = 0; i < strlen(maxString); i++)
	{
		int number = maxString[i] - '0';
		float startPosX = 0.8f -((int)strlen(maxString) - 1) * (UISprite::LETTERWIDTH + letterInvterval) / 2.0f;

		// UI sprite
		auto uiCharacter = new UISprite();
		uiCharacter->SetUIXY(startPosX + (UISprite::LETTERWIDTH + letterInvterval) * i, 0.85f);
		uiCharacter->SetToNumber(number);
		uiCharacter->SetTextureName(ConstVars::CHAR_TEX_FILE);
		m_UISprites.emplace_back(uiCharacter);
	}

}

void GameScene::UpdateUIPos(Camera & camera)
{
	float scale = camera.GetViewSizeWidth() / ConstVars::DEFAULT_VIEW_WIDTH;
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
	m_backGround->SetToBackground(camera.GetViewSizeWidth(), camera.GetViewSizeHeight(), { 0.0f, 1.0f, 0.0f });
}

ModelClass * GameScene::MakeNewBlock(XMFLOAT3 Position, XMFLOAT3 boxSize)
{
	ModelClass* newBlock = new ModelClass();
	newBlock->SetToCube(boxSize);
	newBlock->SetPosition(Position);
	newBlock->SetRGB(m_color.x, m_color.y, m_color.z);
	newBlock->SetTextureName(ConstVars::VISIBLE_TEX_FILE);
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

	if (!m_currentBlock->IsOnMove()) { //반복해서 상자가 왔다갔다 하게 함.
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

			UpdateRecord(dy);
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

			//현재블럭 멈춤.
			m_currentBlock->StopMove();

			//잘린 후 두 블럭의 크기, 위치 지정.
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
			transModel->SetTextureName(ConstVars::VANISHING_TEX_FILE);
			transModel->AddGravityMoveToScheduler({ deltaPositionX * 3, 0.0f, deltaPositionZ * 3}, 2.0f);
			AddModel(transModel);

			//새로 만드는 박스 높이를 한단계증가.
			m_curPos.y += dy;
			m_curPos.x = visibleBlockPos.x;
			m_curPos.z = visibleBlockPos.z;

			//색상 변경.
			auto newRGBA = MakeCircularRGB(m_randomSeed + m_countAccumulation);
			m_color = { newRGBA.x, newRGBA.y, newRGBA.z };
			//새 블록 움직이는 방향 설정
			ChangeDirection();

			//현재 블록, 투명블록 지우기.
			RemoveModel([=](ModelClass* model) -> bool {
				bool curBlock = (model == m_currentBlock);
				if (curBlock)
					delete model;
				bool vaniBlock = (model->GetTransparency() && !model->IsOnMove());
				if(vaniBlock)
					delete model;
				return curBlock || vaniBlock;
			});

			//새 블록 생성.
			XMFLOAT3 newPosition = { m_curPos.x + m_curMoveDir.x, m_curPos.y + m_curMoveDir.y, m_curPos.z + m_curMoveDir.z };
			m_currentBlock = MakeNewBlock(newPosition, m_boxSize);
			AddModel(m_currentBlock);

			//background & camera move
			MoveCameraAndBackground(camera, dy);


			UpdateRecord(dy);
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
			transModel->SetTextureName(ConstVars::VANISHING_TEX_FILE);
			AddModel(transModel);
			RemoveModel([=](ModelClass* model) -> bool {
				return model == m_currentBlock;
			});

			m_logger->SaveRecord(m_maxCount);
			m_logger->SavePlayLog(m_countAccumulation, m_currentHeight);

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
		float viewWidth = ConstVars::DEFAULT_VIEW_WIDTH + elapsedTime * m_currentHeight * 1.3f;
		float viewHeight = ConstVars::DEFAULT_VIEW_HEIGHT + elapsedTime * m_currentHeight * 1.3f;
		m_backGround->SetScale(viewWidth / ConstVars::DEFAULT_VIEW_WIDTH, viewHeight / ConstVars::DEFAULT_VIEW_HEIGHT, 1);
		camera.SetProjection(viewWidth, viewHeight);
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
		SystemClass::GetInstance()->SetScene(new MainScene());
		//씬을 종료.
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
	float allowDelta = GameScene::DEFAULT_BOXSIZE.x / 20.0f;
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
