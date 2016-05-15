#include <windows.h>
#include "Scene.h"
#include "Renderer.h"
#include "InputClass.h"
#include "ModelClass.h"
#include "VanishingBlock.h"


void Scene::Play(float detaTime, InputClass& input, std::vector<ModelClass*>& modelsToBeRendered, Renderer& renderer)
{

	//배경 사각형 설정
	if (m_backGround == nullptr)
	{
		auto backGround = new ModelClass();
		backGround->SetPosition(5.0f, -7.0f, 5.0f);
		backGround->SetRGB(0.7f, 1.0f, 1.0f);
		backGround->SetToRectangle(15.0f, 15.0f, { 0.0f, 1.0f, 0.0f });
		m_backGround = backGround;
		modelsToBeRendered.push_back(backGround);
	}
	static float count = 0;
	float dy = 1.0f;
	if (input.IsKeyDown(VK_SPACE))
	{
		ModelClass* model = new ModelClass();
		model->SetPosition(0, count, 0);
		ModelClass* transModel = new VanishingBlock();
		transModel->SetPosition(-0.5f, count, 2);
		model->SetToCube(2, 1, 2);
		transModel->SetToCube(1, 1, 1);
		modelsToBeRendered.push_back(model);
		modelsToBeRendered.push_back(transModel);
		count += dy;
		m_backGround->AddMoveToScheduler(0.0f, dy, 0.0f, 0.3f);
		renderer.MoveCameraFor(0.0f, dy, 0.0f, 0.3f);
	}

}



Scene::Scene()
{
	
	
	if (m_backGround)
	{
		delete m_backGround;
		m_backGround = nullptr;
	}
}


Scene::~Scene()
{
}
